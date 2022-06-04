#!/usr/bin/env python

# This script is responsible for: 
# 1. Reading the force data from the K3D40 3-axis force sensor (using the GSV8 library from MEMessysteme)
# 2. Sending the force data to the python-MIOS-interface
# 3. Receiving the telemetry data from the robot (T-matrix of end-effector)
# 4. Recording this data in a .csv file
#
# It saves the x,y,z Force of the end effector, a t value for debugging (not real time time!), the real time value, the end effector global x,y,z position and the T-Matrx of the end effector

import socket
import time
from datetime import datetime
import sys
import os
import csv
import serial

import GSV8 as gsv
from exp_config import HOST_IP, NUC_IP, UDP_PORT_RECEIVE_TEL, UDP_PORT_SEND_FORCE


sock_receive = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # INTERNET, UDP
sock_send = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # INTERNET, UDP

sock_receive.bind((HOST_IP, UDP_PORT_RECEIVE_TEL))

FORCE_THRESHOLD = 0.04             #Data is recorded once this thresshold is reached
BASE_DIR = "./new_measurements/"    #Directory for saved data
FIELDNAMES = ["t_value","x_value","y_value", "z_value", "real_t_value", "x_pos", "y_pos", "z_pos", "T"]

record_flag = 0     #Indicates that the recording has started
timer_flag = 0      #Indicates that timer has started
t = 0               #Not real t value, for debugging purposes

if not os.path.exists(BASE_DIR):
    os.makedirs(BASE_DIR)

#This function opens the connection to the force sensor
def openSerial():
    serialConnection = serial.Serial("/dev/ttyACM0", 230400, timeout=1)
    serialConnection.isOpen()
    serialConnection.write(gsv.StopTransmission())
    time.sleep(0.1)
    serialConnection.reset_input_buffer()
    gsv.SetZero(serialConnection, Channel=0) # SetZero all channels
    return serialConnection

#This function parses the incoming telemetry data from the interface
def parseTel(message):
        data = message[12:-4].split(",")

        T = []
        for val in data:
            T.append(float(val))
        
        x_pos = float(T[-4])
        y_pos = float(T[-3])
        z_pos = float(T[-2])

        return x_pos, y_pos, z_pos, T

#This function initiates the .csv file that holds the experiment data
def initFile(fileName):

    fileNames = os.listdir(BASE_DIR)
    fileNames = [file for file in fileNames if fileName in file]

    counter = len(fileNames) + 1 

    fileName = BASE_DIR + fileName + "#" + str(counter) + ".csv"

    if fileName in fileNames:
        fileName = BASE_DIR + fileName + "#" + str(counter) + "_new" + ".csv"

    with open(fileName, 'w') as csv_file:
        csv_writer = csv.DictWriter(csv_file, fieldnames=FIELDNAMES)
        csv_writer.writeheader()

    return fileName

#This function writes the current experiment data into the .csv file
def writeExpData(t,x,y,z,real_t, x_pos, y_pos, z_pos, T, filename):

        with open(fileName, 'a') as csv_file:
            csv_writer = csv.DictWriter(csv_file, fieldnames=FIELDNAMES)

            info = {
            "t_value": t,
            "x_value": x,
            "y_value": y,
            "z_value": z,
            "real_t_value": real_t,
            "x_pos": x_pos,
            "y_pos": y_pos,
            "z_pos": z_pos,
            "T": T,
            }
            csv_writer.writerow(info)




if __name__ == '__main__':

    sensor = openSerial()

    args = sys.argv                 #Read arguments from commandline argument 1 is the duration that the experiment records the data, argument 2 is the filename
    if(len(args)>=2):
        duration = int(args[1])
        fileName = args[2]
    else:
        fileName = "data"
        duration = 100


    fileName = initFile(fileName)

    timer_init = time.time()

    while(True):
        elapsed_time = time.time() - timer_init

        x = gsv.getMeasValues(sensor)[0]        #Get current x-Force
        time.sleep(0.000005)
        y = gsv.getMeasValues(sensor)[1]        #Get current y-Force
        time.sleep(0.000005)
        z = gsv.getMeasValues(sensor)[2]        #Get current z-Force

        message = str(float(elapsed_time)) + " " + str(float(x)) + " " + str(float(y)) + " " + str(float(z))

        sock_send.sendto(message.encode(), (NUC_IP, UDP_PORT_SEND_FORCE))       #Send force data to python-MIOS-interface
        print(message)

        tel_data, addr = sock_receive.recvfrom(1024)            #Receive telemetry from robot 

        x_pos, y_pos, z_pos, T = parseTel(tel_data.decode())    # Parse telemetry        


        if(z > FORCE_THRESHOLD or x > FORCE_THRESHOLD or y > FORCE_THRESHOLD):
            record_flag = 1     #Start recording once threshold is reached
            if not timer_flag:
                timer_flag = 1
                print("timer started")
                timer_init = time.time()
                elapsed_time = time.time() - timer_init

        if(record_flag == 1 and elapsed_time < duration):
            print("Measuring")
            real_t = round(time.time() - timer_init,2)
            writeExpData(t,x,y,z,real_t, x_pos, y_pos, z_pos,T, fileName)

            t += 0.001  

        if((elapsed_time >= duration) and record_flag):
           break

        time.sleep(0.001)

