import sys

import numpy as np
import cv2
import math
import serial
import time
import os
import re
import multiprocessing as mp

import tiffile
import imagecodecs

# Settings
width = 1920  # Range is 0-1438 at far right
scale = 16
verbose = True
serMaxSize = 50
serInterval = 0.1  # 10Hz update rate on speed
nTiffIms = 200
saveDirectory = "C:/Users/grays/Documents/TreadmillData/"  # Directory where folder of images and data should be stored

# Sub-function storage for now ---------------------------------------------------

# !/usr/bin/python3 Excellent wrapper from @skoehler (https://github.com/pyserial/pyserial/issues/216)
class ReadLine:
    def __init__(self, s):
        self.buf = bytearray()
        self.s = s

    def readline(self):
        i = self.buf.find(b"\n")
        if i >= 0:
            r = self.buf[:i + 1]
            self.buf = self.buf[i + 1:]
            return r
        while True:
            i = max(1, min(2048, self.s.in_waiting))
            data = self.s.read(i)
            i = data.find(b"\n")
            if i >= 0:
                r = self.buf + data[:i + 1]
                self.buf[0:] = data[i + 1:]
                return r
            else:
                self.buf.extend(data)

def calcSpeed(verbose, width, cap, savedir, imDatafile, imCounter, outIter, nTiffIms, centDatafile):
    # Capture frame
    frame = cap.read()[1]

    timestamp = time.time()  # temporary until better frame timestamp is figured out
    # frame = frame[:, 240:1680]
    # frame2 = frame[170:865, 260:1670]
    # gray = cv2.cvtColor(frame2, cv2.COLOR_BGR2GRAY)
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    # bw = cv2.threshold(gray, 80, 255, cv2.THRESH_BINARY_INV)[1] # Backlight off
    bw = cv2.threshold(gray, 20, 255, cv2.THRESH_BINARY_INV)[1] # Backlight on
    # bw = cv2.threshold(gray, 80, 255, cv2.THRESH_BINARY_INV)[1]  # IR Backlight
    # bw = cv2.threshold(gray, 40, 255, cv2.THRESH_BINARY_INV)[1]  # IR Backlight

    # Erode image
    # kernel = np.ones((5, 5), np.uint8)
    # erosion = cv2.erode(bw, kernel, iterations=1)

    # calculate x,y coordinate of center
    try:
        contours = cv2.findContours(bw, 1, 1)[0]
        # contours = cv2.findContours(erosion, 1, 1)[0]
        c = max(contours, key=cv2.contourArea)
        M = cv2.moments(c)  # calculate moments of binary image

        # M = cv2.moments(erosion)  # calculate moments of binary image
        cX2 = int(M["m10"] / M["m00"])
        cY2 = int(M["m01"] / M["m00"])
        center = (cX2, cY2)
        # print(center)
        # Calculate speed setting
        if cX2 > round(width / 2):
            speed = 0
        else:
            normalX = scale * (width / 2 - cX2) / width
            speed = math.exp(normalX) - math.exp(0)
        # print(speed)

    except:
        print("Locust not seen")
        cX2 = 0
        cY2 = 0
        center = (cX2, cY2)
        speed = 0 # Stop treadmill if locust disappeared

    # if verbose:
        # Edit frame and show on screen
        # masked = cv2.bitwise_and(frame, frame, mask=erosion)
        # drawn = cv2.circle(frame, center, 5, (0, 255, 0), 2)
        # cv2.imshow("windowName", drawn)

    drawn = cv2.circle(frame, center, 5, (0, 255, 0), 2)
    cv2.imshow("windowName", drawn)
    # cv2.imshow("windowName", frame)
    # cv2.imshow("windowName", bw)
    cv2.waitKey(1)

    # Save image to file
    # cv2.imwrite(savedir + '/' + str(timestamp) + '.jpg', frame)  # more compact

    # tiff version
    imCounter = imCounter+1
    if imCounter == nTiffIms:
        outIter = outIter+1
        imCounter = 0
    # print(imCounter)

    try:
        tiffile.imwrite(savedir + '/images' + str(outIter) + '.tif', frame, photometric='rgb', compression='jpeg', append=True)
        imDatafile.write(str(timestamp) + '\n')
        imDatafile.flush()
    except:
        try:
            print('savefile permission denied, try 2')
            tiffile.imwrite(savedir + '/images' + str(outIter) + '.tif', frame, photometric='rgb', compression='jpeg',
                            append=True)
            imDatafile.write(str(timestamp) + '\n')
            imDatafile.flush()
        except:
            print('savefile permission denied, skipping')

    try:
        centDatafile.write(str(center) + '\n')
        centDatafile.flush()
    except:
        try:
            print('savefile permission denied, try 2')
            centDatafile.write(str(center) + '\n')
            centDatafile.flush()
        except:
            print('savefile permission denied, skipping')




    return (speed, imCounter, outIter)


def serSpeed(speed, ser, timeLast, serInterval):
    temp = time.time()
    if (temp - timeLast) > serInterval:
        tempString = ',1,' + str(round(speed))
        strLen = len(tempString) + 2  # hardcoded to two digits max length
        if (strLen < 10):
            tempString = '0' + str(strLen) + tempString
        else:
            tempString = str(strLen) + tempString
        ser.write(bytes('<' + tempString + '>\r\n', 'utf8'))
        return temp
    else:
        return timeLast

def funcUpdate(ser, datafile, route, startup):
    # Check whether sin or lin
    if route[0][0]==0: # Linear
        # Assemble string from lin route settings
        tempString = ',2,' + str(route[0][1]) + ',' + str(route[0][2]) + ',' + str(route[0][3])
    else: # Sin
        # Assemble string from sin route settings
        tempString = ',6,' + str(route[0][1]) + ',' + str(route[0][2]) + ',' + str(route[0][3]) + ',' + str(route[0][4]) + ',' + str(route[0][5])

    # If startup, activate route immediately by appending immediate-activation flag
    if (startup):
        tempString = tempString + ',1'
    else:
        tempString = tempString + ',0'

    # Compute string send length for checking
    strLen = len(tempString) + 2  # hardcoded to two digits max length
    if (strLen < 10):
        tempString = '0' + str(strLen) + tempString
    else:
        tempString = str(strLen) + tempString

    # Write next print function to arduino
    print("Function Update: " + tempString)
    timestamp = time.time()
    ser.write(bytes('<' + tempString + '>\r\n', 'utf8'))

    # Save string to file
    datafile.write(str(timestamp) + ',' + tempString + '\n')

    # Move sent condition to end of route list
    route.append(route[0])
    route.remove(route[0])

def startup(ser, datafile, route):
    print("Startup called")

    # Call funcUpdate with startup flag set
    funcUpdate(ser, datafile, route, True)

# Main functions -----------------------------------------------------------------
def camMain(savedir, date, speedWrite):
    # Initialize Camera settings
    cap = cv2.VideoCapture(0)
    cv2.VideoCapture.set(cap, 3, 1920)
    cv2.VideoCapture.set(cap, cv2.CAP_PROP_AUTOFOCUS, 0)
    cv2.VideoCapture.set(cap, cv2.CAP_PROP_FOCUS, 10)

    # tiffile version
    filename = savedir + '/imdata_' + date + '.csv'
    imDatafile = open(filename, 'a')
    imCounter = 0
    outIter = 0

    filename = savedir + '/centdata_' + date + '.csv'
    centDatafile = open(filename, 'a')

    while (True):
        # Image stuff
        # speed = calcSpeed(verbose, width, cap, savedir, datafile, imCounter)
        speed, imCounter, outIter = calcSpeed(verbose, width, cap, savedir, imDatafile, imCounter, outIter, nTiffIms, centDatafile)
        # print(speed)
        speedWrite.send(speed)

    # When everything is done, release the capture
    cap.release()
    cv2.destroyAllWindows()
    imDatafile.close()


def serMain(savedir, date, speedRead):
    # Initialize serial communication
    ser = serial.Serial('COM4', 2000000, writeTimeout=0)

    # Initialize wrapper object for efficient line reading
    readObj = ReadLine(ser)

    # Data Saving stuff
    # dataStore = DataStoreClass()  # Initialize data structure
    filename = savedir + '/data_' + date + '.csv'
    datafile = open(filename, 'a')

    # Route format:
    # sin: [sinFlag=1, a, T, ps, b, startPos]
    # lin: [sinFlag=0, m, b, startPos]

    # Zigzag route
    # route = [[0.077, 1500, 40000],
    #          [-0.077, 4600, 40000]]

    # Straight route
    # route = [[0, 0, 3000, 40000],
    #          [0, 0, 3000, 40000]]

    # Sinusoidal route
    # route = [[1, 1500, 40000, 0, 3100, 40000]]

    # Complex route
    route = [[0, 0, 3100, 8915],
             [1, 1500, 40000, 0, 3100, 10000],
             [0, 0.25, 3100, 40000],
             [0, -0.175, 4600, 6000],
             [0, 0, 3620, 5600],
             [0, -0.175, 3620, 10000],
             [0, 0, 2640, 5600],
             [0, -0.175, 2640, 10000],
             [0, 0, 3160, 5600],
             [1, 1500, 40000, 30000, 3160, 5600],
             [0, 0, 4660, 20000],
             [0, -0.175, 4660, 5000],
             ]

    # Initialize variables
    timeLast = 0
    startup(ser, datafile, route)

    while(True):
        # Get speed setting from camera process, send to serial
        if speedRead.poll(): # only update speed if new speed is available
            speed = speedRead.recv()
            timeLast = serSpeed(speed, ser, timeLast, serInterval)

        # Get serial input from treadmill
        if(ser.inWaiting()>0):
            timestamp = time.time()
            try:
                line = readObj.readline()
                line = line.decode('utf-8')

                # Check if length is correct
                try:
                    if(len(line)- 4 == int(line[1:3])):
                        # Check whether new function should be sent
                        if(line[3:6] == '254'):
                            funcUpdate(ser, datafile, route, False)

                        # Save Serial Data to file
                        datafile.write(str(timestamp) + ',' + line[1:3] + ',' + line[3:-3] + '\n')
                except:
                    print("Failed parsing line:" + line)
            except:
                print('Unable to read serial input')
        # Debugging check to ensure that all messages are being processed:
        # print("Ser Buffer: " + str(ser.inWaiting()))



if __name__ == "__main__":
    # Initialize save directory
    date = re.sub('[: ]', '_', time.ctime())
    savedir = saveDirectory + 'Data/' + date
    os.makedirs(savedir)

    # # create tiff output file
    # data = np.random.randint(0, 255, (1080, 1920, 3), 'uint8')
    # tiffile.imwrite(savedir + '/images.tif', data, photometric='rgb', compression='jpeg')

    # Set up communication
    speedRead, speedWrite = mp.Pipe(False)

    # Start camera process
    pCam = mp.Process(target=camMain, args=(savedir, date, speedWrite))
    pCam.start()

    # Start serial comm. and control process
    pSer = mp.Process(target=serMain, args=(savedir, date, speedRead))
    pSer.start()
