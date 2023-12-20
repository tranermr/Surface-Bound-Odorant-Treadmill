// Settings
double defSpeed = 150; // deg/s
double sensitivity = .1; // convert speed difference into PWM difference
const double xTolerance = 5; // Number of increments past target before reversing direction to correct.
const int prSpeed = 120;
int checkThresh = 10;

// Definitions ------------------------------------------------------------
QuadDecode<2> yPosn;  // Template using FTM1
QuadDecode<1> xPosn;  // Template using FTM2

// Paper Drive
//int beltMotorFW   = 21; // Paper Drive Motor Forwards
int beltMotorFW = 22; // changed during march 2022 pcb update... 36; // Changed during belt motor transfer repair Oct 2021
int beltMotorRev  = 20; // reverse

const int PX_LimitLeft  = 11;  // Printer X Limit Switch Left
const int PX_LimitRight = 12;
const int printMotorLeft  = 36; // changed during march 2022 pcb update...22; // Printer X Motor left
const int printMotorRight = 23; // right

const int printSolenoid = 8;

const int left = -1;
const int right = 1;
const int forwards = 1;
const int backwards = -1;
