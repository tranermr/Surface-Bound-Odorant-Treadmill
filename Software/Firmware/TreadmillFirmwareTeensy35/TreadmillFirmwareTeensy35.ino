#include "Arduino.h"
#include <FlexiTimer2.h>
#include "QuadDecode_def.h"

#include <stdbool.h>
#include "treadmillPinDefs.h"

#include <PID_v1.h>

#define HWSERIAL Serial2

const bool verboseMode = false;
double printLastPosY = 0;
double printLastPosX = 0;
//const int printSolenoid = 8; //NUMBER
double dropSpacing = 750; //1500; //250;
bool extendedPrint = false;
unsigned long printSolDur = 145;//145; 21Dec2022           //110; // 12.3 * 10; //12;
unsigned long printSolDurLong = 150;//115; //12.4 * 10; //12;
unsigned long printTimeThresh = 15000;
volatile bool resetPrintDur = false;
unsigned long lastPrintTime;
unsigned long keepWetPeriod = 20 * 1000; //10 * 1000;

#define MAX_SAMPLES_FLASH 10
#define PIN_isr 8
#define tripleLED 6
const unsigned long flashPer_us = 500000;
const double xPWMStallThresh = 0;

unsigned long PD_update_interval = 15000; // 25 msec update interval (PID interval will be double this)
unsigned long PX_update_interval = 2000; // 2msec update interval

unsigned long lastUpdate = 0; // Temporary debug variable

// Dynamic Variables ------------------------------------------------------------
volatile uint32_t tFlash[MAX_SAMPLES_FLASH];
volatile bool sFlash[MAX_SAMPLES_FLASH];
volatile int tFlashIter = 0;
volatile int flashUpdateLast = 0;
volatile bool flashUpdatePend = false;
unsigned long flashSyncLast = 0;

unsigned long curTimeMicros = 0;

const int strLength = 99;
const int outStrLength = 50;
char inputString[strLength];
char outputString[strLength];
char tempChar = '0';
int serPos = 0;
char checkStr[3];
char tempStr[3];
size_t checkVal = 0;

// Linear trail calc variables and constants
double PD_EQ_m = 0;
double PD_EQ_m_next = 0;

// Sine trail calc variables and constants
const double YSCALE = 5000;
double PD_EQ_a = 0;
double PD_EQ_T = 1;
double PD_EQ_ps = 0;
double PD_EQ_a_next = 0;
double PD_EQ_T_next = 1;
double PD_EQ_ps_next = 0;

// Shared trail calc variables
bool nextSin;
bool currSin;
double PD_EQ_start = 999999.01;
double PD_EQ_b = 0;
double PD_EQ_b_next = 0;

// Treadmill variables
double PD_SpeedRot = 0;
double PD_setSpeedRot = 0;
double PD_setPWM = 0;
double PD_Encoder_count = 0;
double PD_lastPos = 0;

// Print head variables
volatile bool PX_moveRightEnable  = false;
volatile bool PX_moveLeftEnable   = false;
double PX_pos = 0;
double PX_pos_exact = 0;
double PX_setPos = 0;
double PX_setPWM = 0;


// PID Settings
double PD_Kp = 2, PD_Ki = 4, PD_Kd = 0;
//kp: 0.32 ki: 0.01 kd: 0.00
PID PD_speed_PID(&PD_SpeedRot, &PD_setPWM, &PD_setSpeedRot, PD_Kp, PD_Ki, PD_Kd, DIRECT);

bool PX_round = false;
int PX_round_factor = 70;//10;

//double PX_Kp=1.5, PX_Ki=0.4, PX_Kd=.2;
//double PX_Kp = 2.5, PX_Ki = 0.4, PX_Kd = .2;
//double PX_Kp = 2.5, PX_Ki = 0.4, PX_Kd = 2.0;
double PX_Kp = 2.5, PX_Ki = 0.2, PX_Kd = 1.0;
PID PX_pos_PID(&PX_pos, &PX_setPWM, &PX_setPos, PX_Kp, PX_Ki, PX_Kd, P_ON_M, DIRECT);

elapsedMicros usTime;

// Functions  ------------------------------------------------------------
void LimitStopLeft() {
  //  if (verboseMode) {
  Serial.println("Left Limit Stop");
  //  }
  PX_moveLeftEnable = false;
  PX_moveRightEnable = true;
  analogWrite(printMotorLeft, 0);
  xPosn.zeroFTM(); //Zero Xposition counter
  Serial.println("X pos zeroed 1");
}

void LimitStopRight() {
  //  if (verboseMode) {
  Serial.println("Right Limit Stop");
  //  }
  PX_moveLeftEnable = true;
  PX_moveRightEnable = false;
  analogWrite(printMotorRight, 0);
}

void PX_ZeroPosition() {
  int testVal = digitalRead(PX_LimitLeft);
  if (testVal) {
    PX_moveLeftEnable = true;
    analogWrite(printMotorLeft, 110);
  } else {
    Serial.println("X pos zeroed 2");
    xPosn.zeroFTM(); //Zero Xposition counter
    PX_moveLeftEnable = false;
    PX_moveRightEnable = true;
  }
}

void PX_Output(double PX_setPWM) {
  if (micros() % 1000000 < 100) {
    Serial.print("PX_pos: ");
    Serial.print(PX_pos_exact);
    Serial.print("  PX_setPWM: ");
    Serial.print(PX_setPWM);
    Serial.print("  PX_moveLeftEnable: ");
    Serial.print(PX_moveLeftEnable);
    Serial.print("  PX_moveRightEnable: ");
    Serial.println(PX_moveRightEnable);
  }

  // Movement enable check
  if (!PX_moveLeftEnable) {
    if (!digitalRead(PX_LimitLeft)) {
      PX_moveLeftEnable = true;
    } else {
      Serial.println("X pos zeroed 3");
      xPosn.zeroFTM(); //Zero Xposition counter
    }
  }
  if (!PX_moveRightEnable) {
    if (!digitalRead(PX_LimitRight)) {
      PX_moveRightEnable = true;
    }
  }

  // PX Movement control
  if (PX_setPWM == 0) {
    analogWrite(printMotorLeft, 0);
    analogWrite(printMotorRight, 0);
  } else if (abs(PX_setPWM) > xPWMStallThresh) {
    if (PX_setPWM < 0 && PX_moveLeftEnable) {
      analogWrite(printMotorLeft, abs(PX_setPWM));
      analogWrite(printMotorRight, 0);
      //      Serial.print("Move Left set to "); Serial.println(PX_setPWM);
      PX_moveRightEnable = true;
    } else if (PX_setPWM > 0 && PX_moveRightEnable) {
      analogWrite(printMotorLeft, 0);
      analogWrite(printMotorRight, abs(PX_setPWM));
      //      Serial.print("Move Right set to "); Serial.println(PX_setPWM);
      PX_moveLeftEnable = true;
    }
  }
}

void PX_updateSetPos(bool updateNow = false) {
  double curY = -yPosn.calcPosn();
  double curX = xPosn.calcPosn();
  if (updateNow || curY >= PD_EQ_start) { // IF position cond. to use next print function is met
    // Log actual switchover PD value
    HWprint(254);
    HWprint(curY);
    HWprintln(usTime);

    // Zero PD Position
    yPosn.zeroFTM();

    // Update previous position so PD speed estimate is correct
    double temp_offset = curY - PD_lastPos;
    PD_lastPos = -temp_offset;
    Serial.print("Before Y: "); Serial.println(printLastPosY);
    printLastPosY = curY - printLastPosY;
    Serial.print("After Y: "); Serial.println(printLastPosY);
    Serial.print("Before X: "); Serial.println(printLastPosX);
    printLastPosX = curX - printLastPosX;
    Serial.print("After X: "); Serial.println(printLastPosX);


    // Update position function to new values
    if (nextSin) {
      currSin = true;
      PD_EQ_a = PD_EQ_a_next;
      PD_EQ_T = PD_EQ_T_next;
      PD_EQ_ps = PD_EQ_ps_next;
      PD_EQ_b = PD_EQ_b_next;
    } else {
      currSin = false;
      PD_EQ_m = PD_EQ_m_next;
      PD_EQ_b = PD_EQ_b_next;
    }
  }
  // Regardless, update xPosn goal
  if (currSin) {
    PX_setPos = round(PD_EQ_a * sin(2 * M_PI * 1 / PD_EQ_T * (curY + PD_EQ_ps))) + PD_EQ_b;
  } else {
    PX_setPos = round(PD_EQ_m * curY) + PD_EQ_b;
  }

  if (PX_round) {
    PX_setPos = round(PX_setPos / PX_round_factor);
  }
}

void readSerial() {
  if (HWSERIAL.available() > 0) {
    tempChar = HWSERIAL.read();
    if (tempChar == '<') { // Beginning of new message
      serPos = 0;
      for (int iter = 0; iter < strLength; iter++) {
        inputString[iter] = '\0';
      }
    } else if (tempChar == '>') { // End of message - process
      //      Serial.print("AAAAARGH:  ");
      //      Serial.println(inputString);

      // Check if received message is complete
      checkStr[0] = inputString[0];
      checkStr[1] = inputString[1];
      checkStr[2] = '\0';
      checkVal = atoi(checkStr);



      if (checkVal == strlen(inputString)) {
        // If message is not missing parts process message
        if (inputString[3] == '1') {      // Motor speed update
          //          Serial.println("Speed update received");
          parseSerialSpeed(inputString, strLength, &PD_setSpeedRot);
        } else if (inputString[3] == '2') { // Print function update
          //          Serial.println("Lin func received");
          parseSerialFunc(inputString, strLength, &PD_EQ_m_next, &PD_EQ_b_next, &PD_EQ_start, &nextSin);
        } else if (inputString[3] == '3') { // PID Tuning update
          //          Serial.println("PID Update received");
          parseSerialPX_PID(inputString, strLength, &PX_Kp, &PX_Ki, &PX_Kd);
          //        } else if (inputString[3] == '4') { // Print function update - IMMEDIATE UPDATE
          //          parseSerialFuncImm(inputString, strLength, &PD_EQ_m_next, &PD_EQ_b_next, &PD_EQ_start);
        } else if (inputString[3] == '6') { // Sin function serial
          //          Serial.println("Sin func received");
          parseSerialFuncSin(inputString, strLength, &PD_EQ_a_next, &PD_EQ_T_next, &PD_EQ_ps_next, &PD_EQ_b_next, &PD_EQ_start, &nextSin);
        } else {
          Serial.print("ERROR: Command unknown: ");// ignore - command is not known
          Serial.println(inputString);
        }
      } else {
        Serial.println("ERROR: Message corrupted: ");
        Serial.println(inputString);
        // If message IS missing parts send error message and timestamp
        HWprint(250);
        HWprint(checkVal);
        HWprint("(");
        HWprint(inputString);
        HWprint(")");
        HWprintln(usTime);
      }
    } else { // Continue adding to message
      inputString[serPos] = tempChar;
      if (serPos < (strLength - 1)) {
        serPos += 1;
      }
    }
  }
}

void parseSerialSpeed(char inputString[], const int strLength, double * PD_setSpeedRot) {
  char * temp;
  char delim = ',';
  temp = strtok(inputString, &delim); // Remove length checksum & com flag
  temp = strtok(NULL, &delim);        // call again to retrieve second str token
  temp = strtok(NULL, &delim);        // call again to retrieve third str token
  *PD_setSpeedRot = atoi(temp);
  if (verboseMode) {
    Serial.print("Speed set to ");
    Serial.println(*PD_setSpeedRot);
  }
}

void parseSerialFunc(char inputString[], int strLength, double * PD_EQ_m_next, double * PD_EQ_b_next, double * PD_EQ_start, bool * nextSin) {
  //  Serial.print("parseSerialFunc: ");
  //  Serial.println(inputString);

  // Set next function transition to linear
  *nextSin = false;

  // Parse
  char * temp;
  char delim = ',';
  temp = strtok(inputString, &delim); // Remove length checksum & com flag
  temp = strtok(NULL, &delim);
  temp = strtok(NULL, &delim);        // call again to retrieve third str token
  *PD_EQ_m_next = atof(temp);
  temp = strtok(NULL, &delim);
  *PD_EQ_b_next = atof(temp);
  temp = strtok(NULL, &delim);
  *PD_EQ_start = atof(temp);

  // Check whether to set immediately
  temp = strtok(NULL, &delim);
  int startImm = atoi(temp);

  if (startImm == 1) {
    Serial.println("Start immediately");
    PX_updateSetPos(true);
  }

  if (verboseMode) {
    Serial.print("  setM: ");
    Serial.print(*PD_EQ_m_next);
    Serial.print("  setB: ");
    Serial.println(*PD_EQ_b_next);
  }
}

void parseSerialFuncSin(char inputString[], int strLength, double * PD_EQ_a_next, double * PD_EQ_T_next, double * PD_EQ_ps_next, double * PD_EQ_b_next, double * PD_EQ_start, bool * nextSin) {
  // PX_setPos = round(PD_EQ_a*sin(2*M_PI*1/PD_EQ_T*(curY/YSCALE + PD_EQ_ps))) + PD_EQ_b;
  /* Parse out:
    // - Sin function stuff
    //   + amplitude: PD_EQ_a_next
    //   + frequency: PD_EQ_T_next
    //   + phase shift: PD_EQ_ps_next
    //   + offset: PD_EQ_b_next
    // - change point eq-start: PD_EQ_start
  */
  //  Serial.print("parseSerialFuncSin: ");
  //  Serial.println(inputString);

  // Set next transition to sin mode
  *nextSin = true;

  // Parse
  char * temp;
  char delim = ',';
  temp = strtok(inputString, &delim); // Remove length checksum & com flag
  temp = strtok(NULL, &delim);
  temp = strtok(NULL, &delim);        // call again to retrieve third str token
  *PD_EQ_a_next = atof(temp);
  temp = strtok(NULL, &delim);
  *PD_EQ_T_next = atof(temp);
  temp = strtok(NULL, &delim);
  *PD_EQ_ps_next = atof(temp);
  temp = strtok(NULL, &delim);
  *PD_EQ_b_next = atof(temp);
  temp = strtok(NULL, &delim);
  *PD_EQ_start = atof(temp);

  // Check whether to set immediately
  temp = strtok(NULL, &delim);
  int startImm = atoi(temp);

  if (startImm == 1) {
    Serial.println("Start immediately");
    PX_updateSetPos(true);
  }

  if (verboseMode) {
    Serial.print("  setA: ");
    Serial.print(*PD_EQ_a_next);
    Serial.print("  setF: ");
    Serial.print(*PD_EQ_T_next);
    Serial.print("  setPS: ");
    Serial.print(*PD_EQ_ps_next);
    Serial.print("  setB: ");
    Serial.print(*PD_EQ_b_next);
  }
}


void parseSerialFuncImm(char inputString[], int strLength, double * PD_EQ_m_next, double * PD_EQ_b_next, double * PD_EQ_start) {
  //  Serial.println(inputString);

  char * temp;
  char delim = ',';
  temp = strtok(inputString, &delim); // Remove length checksum & com flag
  temp = strtok(NULL, &delim);
  temp = strtok(NULL, &delim);        // call again to retrieve third str token
  *PD_EQ_m_next = atof(temp);
  temp = strtok(NULL, &delim);
  *PD_EQ_b_next = atof(temp);
  temp = strtok(NULL, &delim);
  *PD_EQ_start = atof(temp);
  //  Serial.print("PD_EQ_m_next: ");Serial.print(*PD_EQ_m_next);
  //  Serial.print("   PD_EQ_b_next: ");Serial.print(*PD_EQ_b_next);
  //  Serial.print("   PD_EQ_start: ");Serial.println(*PD_EQ_start);

  if (verboseMode) {
    Serial.print("  setM: ");
    Serial.print(*PD_EQ_m_next);
  }
  PX_updateSetPos(true);
}

void parseSerialPX_PID(char inputString[], int strLength, double * PX_Kp, double * PX_Ki, double * PX_Kd) {
  char * temp;
  char delim = ',';
  temp = strtok(inputString, &delim); // Remove length checksum & com flag
  temp = strtok(NULL, &delim);
  temp = strtok(NULL, &delim);        // call again to retrieve third str token
  *PX_Kp = atof(temp);
  temp = strtok(NULL, &delim);
  *PX_Ki = atof(temp);
  temp = strtok(NULL, &delim);
  *PX_Kd = atof(temp);
  PX_pos_PID.SetTunings(*PX_Kp, *PX_Ki, *PX_Kd);
}

void PD_SpeedRegulator(int motor) { // call twice as often as update interval - ensures sample is taken at half periods to min effects of lagging output
  PD_Encoder_count = -yPosn.calcPosn();
  //  static double PD_lastPos = 0;
  static unsigned long lastTime = 0;

  PD_SpeedRot = 1000000 * ((PD_Encoder_count - PD_lastPos) * 360 / (4 * 1200)) / (usTime - lastTime); // Calculate speed in deg/s

  if (verboseMode) {
    if (micros() % 1000000 < 100) {
      Serial.print("Measured PD_SpeedRot: ");
      Serial.print(PD_SpeedRot);
      Serial.print("  PD_setPWM: ");
      Serial.println(PD_setPWM);
    }
  }

  PD_speed_PID.Compute();

  analogWrite(beltMotorFW, PD_setPWM);

  PD_lastPos = PD_Encoder_count;
  lastTime = usTime;
}

FASTRUN void flashRec() {
  tFlash[tFlashIter] = usTime;
  flashUpdatePend = true;
  if ( tFlashIter < (MAX_SAMPLES_FLASH - 1) ) {
    tFlashIter++;
  } else {
    tFlashIter = 0;
  }
}

void flashUpdate() {
  if (flashUpdateLast <= tFlashIter) {
    for (int iter = flashUpdateLast; iter < tFlashIter; iter++) {
      HWprint(253);
      HWprint(tFlash[iter]);
      HWprintln(sFlash[iter]);
    }
  } else if (tFlashIter == 0) {
    for (int iter = flashUpdateLast; iter < MAX_SAMPLES_FLASH; iter++) {
      HWprint(253);
      HWprint(tFlash[iter]);
      HWprintln(sFlash[iter]);
    }
  } else {
    for (int iter = flashUpdateLast; iter < MAX_SAMPLES_FLASH; iter++) {
      // HWserial update part 1
      HWprint(253);
      HWprint(tFlash[iter]);
      HWprintln(sFlash[iter]);
    }
    for (int iter = 0; iter <= tFlashIter; iter++) {
      // HWserial update part 2
      HWprint(253);
      HWprint(tFlash[iter]);
      HWprintln(sFlash[iter]);
    }
  }
  flashUpdatePend = false;
  flashUpdateLast = tFlashIter;
}

void flashSync(unsigned long curTimeMicros) {
  if (curTimeMicros - flashSyncLast > flashPer_us) {
    flashSyncLast = curTimeMicros;
    if (tFlashIter < MAX_SAMPLES_FLASH - 1) {
      sFlash[tFlashIter + 1] = !digitalReadFast(tripleLED);
    } else {
      sFlash[0] = !digitalReadFast(tripleLED);
    }
    digitalWriteFast(tripleLED, sFlash[tFlashIter + 1]);
  }
}

void timedSerialUpdate() {
  HWprint("255");
  HWprint(',');
  HWprint(PD_Encoder_count);
  HWprint(',');
  HWprint(PX_pos_exact);
  HWprint(',');
  HWprintln(usTime);
  //  Serial.print("PX Pos: ");
  //  Serial.println(PX_pos_exact);
  //  Serial.print("PY Pos: ");
  //  Serial.println(-yPosn.calcPosn());
}

void HWprint(char input[]) {
  strcat(outputString, input);
  strcat(outputString, ",");
}

void HWprint(double input) {
  // Get end of current output string
  static char * beg;
  for (char * iter = outputString; *iter != '\0'; iter++) {
    beg = iter + 1;
  }

  // Get length of input
  static int cOffset;
  if (input == 0) {
    cOffset = 1;
  } else {
    cOffset = ceil(log10(input));
  }

  // Append input to output string
  if ((cOffset + beg - outputString) < 99) {
    dtostrf(input, cOffset, 0, beg);
  }
  strcat(outputString, ",");
}

void HWprintln(char input[]) {
  // Append input to output string
  HWprint(input);

  // Perform length checksum calculations before sending
  // Note: <> endpoints and newlines are NOT counted as characters
  static int checkLen = strlen(outputString) - 2;
  if (checkLen < 100) {
    outputString[1] = floor(checkLen / 10) + 48;
    outputString[2] = (checkLen % 10) + 48;
  } else {
    Serial.println("Error - output too long");
    return;
  }

  // Add ending >
  static char * beg;
  for (char * iter = outputString; *iter != '\0'; iter++) {
    beg = iter;
  }
  *beg = '>';

  // Send output string
  HWSERIAL.println(outputString);

  // Reset output string for next use
  for (int iter = 0; iter < outStrLength; iter++) {
    outputString[iter] = '\0';
  }
  outputString[0] = '<';
  outputString[1] = '0';
  outputString[2] = '0';
}

void HWprintln(double input) {
  // Append input to output string
  HWprint(input);

  // Perform length checksum calculations before sending
  // Note: <> endpoints and newlines are NOT counted as characters
  static int checkLen;
  checkLen = strlen(outputString) - 2;
  if (checkLen < 100) {
    outputString[1] = floor(checkLen / 10) + 48;
    outputString[2] = (checkLen % 10) + 48;
  }

  // Add ending >
  static char * beg;
  for (char * iter = outputString; *iter != '\0'; iter++) {
    beg = iter;
  }
  *beg = '>';

  // Send output string
  HWSERIAL.println(outputString);

  // Reset output string for next use
  for (int iter = 0; iter < outStrLength; iter++) {
    outputString[iter] = '\0';
  }
  outputString[0] = '<';
  outputString[1] = '0';
  outputString[2] = '0';
}

void printer(bool forcePrint = false) {
  static double curPosX;
  static double curPosY;
  curPosY = -yPosn.calcPosn();
  curPosX = xPosn.calcPosn();
  double diffY = curPosY - printLastPosY;
  double diffX = curPosX - printLastPosX;
  unsigned long curtime = millis();
  if (forcePrint) {
    //    Serial.println("Print");
    FlexiTimer2::start();
    digitalWriteFast(printSolenoid, HIGH); // Write ON to solenoid
    printLastPosY = curPosY;
    printLastPosX = curPosX;
    lastPrintTime = curtime;
  } else {
    if (!extendedPrint && (curtime - lastPrintTime) > printTimeThresh) {
      //    Serial.println("extending print dur");
      extendedPrint = true;
      FlexiTimer2::time_units = printSolDurLong;
    }
    // TEMP ------------------------------------------------------
    // Update printhead firing spacing
    // Need to determine relative distance of x and y dimensions to actually calculate this...
    // Treadmill Distance:      153.18 mm <-> 14468 increments   ==>   0.01058750345 mm/yinc
    // Print carriage distance  709.3 mm <-> 3000 increments   ==>   0.23643333333 mm/xinc
    // ydist/xdist = 0.01058750345 mm/yinc / 0.23643333333 mm/xinc = 0.044780079 xinc/yinc
    // Scale x to match y
    const double xyscale = 0.2; //0.044780079;
    diffX = diffX * 1/xyscale;
    double diff = sqrt(diffX*diffX + diffY*diffY);

    // Check if diff is greater
    if (diff >= dropSpacing) {
      FlexiTimer2::start();
      digitalWriteFast(printSolenoid, HIGH); // Write ON to solenoid
      printLastPosY = curPosY;
      printLastPosX = curPosX;
      lastPrintTime = curtime;
    }
  }
}

void printTimer() {
  FlexiTimer2::stop();
  digitalWriteFast(printSolenoid, LOW); // Write OFF to solenoid
  if (extendedPrint) {
    extendedPrint = false;
    resetPrintDur = true;
    //    FlexiTimer2::time_units = printSolDur; // This causes issues because when it is changed during an interrupt it messes up other values in memory
    // Well, I think that is what is happening. It doesn't cause issues on my test code, but perhaps that is because the code uses less memory and there is reduced chance of overwriting an important area?
  }
}

// Setup ------------------------------------------------------------
void setup()
{
  pinMode(PX_LimitLeft, INPUT);
  pinMode(PX_LimitRight, INPUT);

  pinMode(beltMotorFW, OUTPUT);
  pinMode(beltMotorRev, OUTPUT);
  pinMode(printMotorLeft, OUTPUT);
  pinMode(printMotorRight, OUTPUT);

  pinMode(tripleLED, OUTPUT);

  //  Serial.begin (9600);
  HWSERIAL.begin(2000000);

  attachInterrupt(PX_LimitLeft, LimitStopLeft, FALLING);
  attachInterrupt(PX_LimitRight, LimitStopRight, FALLING);

  //  pinMode(PIN_isr, INPUT_PULLUP);
  //  attachInterrupt(digitalPinToInterrupt(PIN_isr), flashRec, CHANGE);

  xPosn.setup();      // Start Quad Decode position count
  yPosn.setup();      // Start Quad Decode position count

  xPosn.start();      // Start Quad Decode position count
  yPosn.start();      // Start Quad Decode position count

  PD_speed_PID.SetSampleTime(2 * PD_update_interval / 1000); // Set sample time to double PD_update_interval and convert from us to ms
  PD_speed_PID.SetOutputLimits(0, 255); //120); // 255);
  PD_speed_PID.SetMode(AUTOMATIC); // Turn on PID control

  if (PX_round) {
    PX_Kp = PX_Kp * PX_round_factor;
    PX_Ki = PX_Ki * PX_round_factor;
    PX_Kd = PX_Kd * PX_round_factor;
  }
  PX_pos_PID.SetSampleTime(PX_update_interval / 1000);
  //  PX_pos_PID.SetOutputLimits(-255, 255);
  //  PX_pos_PID.SetOutputLimits(-80, 80);
  PX_pos_PID.SetOutputLimits(-100, 100);
  PX_pos_PID.SetMode(AUTOMATIC); // Turn on PID control

  pinMode(printSolenoid, OUTPUT);
  FlexiTimer2::set(printSolDur, 1.0 / 10000, printTimer);
  //  FlexiTimer2::set(25, 1.0/1000, printTimer); // increased to try and reduce clogging

  // Prep output string
  for (int iter = 0; iter < outStrLength; iter++) {
    outputString[iter] = '\0';
  }
  outputString[0] = '<';
  outputString[1] = '0';
  outputString[2] = '0';

  PX_ZeroPosition();
  delay(5000);
  PX_ZeroPosition();

  Serial.println("Setup");

  delay(2000);

  Serial.println("Disabling Limit Switches");
  detachInterrupt(digitalPinToInterrupt(PX_LimitLeft));
  detachInterrupt(digitalPinToInterrupt(PX_LimitRight));
}

// Main Loop ------------------------------------------------------------
void loop()
{
  // Check serial commands
  readSerial();

  // Update time
  curTimeMicros = usTime;

  // Handle Printhead position
  PX_updateSetPos();
  //  PX_checkPos();
  PX_pos_exact = xPosn.calcPosn();
  if (PX_round) {
    PX_pos = round(xPosn.calcPosn() / PX_round_factor);
  } else {
    PX_pos = PX_pos_exact;
  }

  //  if (micros() % 1000000 < 50) {
  //    Serial.print("PX_setPos: ");
  //    Serial.print(PX_setPos);
  //    Serial.print("  PX_actual: ");
  //    Serial.println(PX_pos);
  //  }

  //  Serial.print("PX: "); Serial.println(PX_pos);
  //  Serial.print("  PX_Set: "); Serial.print(PX_setPos);
  //  Serial.print("  PX_PWM: "); Serial.println(PX_setPos);
  PX_pos_PID.Compute();
  PX_Output(PX_setPWM);

  // Print function
  printer();

  if (millis() - lastPrintTime > keepWetPeriod) {
    printer(true);
  }

  // print update every 10 ms
  if (curTimeMicros > lastUpdate + 10000) {
    timedSerialUpdate();
    lastUpdate = curTimeMicros;
  }

  // Moved here to see if adjustment outside interrupt fixes problem... It did.
  if (resetPrintDur) {
    //    Serial.println("resetting print duration");
    FlexiTimer2::time_units = printSolDur;
    resetPrintDur = false;
  }

  // Handle paper speed
  PD_SpeedRegulator(beltMotorFW);

  //  Serial.println(xPosn.calcPosn());

  //  // Syncronization lights
  //  flashSync(curTimeMicros);
  //  if(flashUpdatePend == true){
  //    flashUpdate();
  //  }
}
