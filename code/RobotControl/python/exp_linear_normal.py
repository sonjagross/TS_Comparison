#
# This script is used for executing a linear normal force experiment with the experimental setup.
# Initially some parameters relevant for the experiment can be edited such as force, rate, amount of repetitions.. etc
# The main loop first starts the recording by executing a subprocess that runs the respective file.
# Thereby the filename and data_record_time are forwarded
# If defined, a certain time will be awaited before the experiment starts
# The loop will iterate through the array of force values that depends on the specified repitition count
# The command is created and sent towards the robot
# After receiving a response that the command is executed the loop will continue
# Once the loop is finished the code waits for the data recording to terminate
# The operator is asked if they want to repeat the experiment with the same parameters
# Depending on the input, the script will either continue or terminate
# This description also applies for the other experiment scripts (with some small differences)
#

import socket
import time
import subprocess

from exp_config import *


SOCK_RECEIVE = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # INTERNET, UDP
SOCK_SEND = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # INTERNET, UDP

SOCK_RECEIVE.bind((HOST_IP, UDP_PORT_RECEIVE_OK))

############################# Modify this as required

#Experiment Parameters
REPETITIONS = 1         #Amount of Cycles
FORCE_VAL = 10          #Target Force in Newton
#FEED_RATE = 0.00001    #Target Velocity in m/s / 250(Interface timing) | 0.00001 * 250 * 1000 =  2.5 mm/s
FEED_RATE = 0.000001    #ca. 70s
INITIAL_WAIT = 0        #Waiting Time in Seconds
#DATA_RECORD_TIME = 8   #Duration for Data Record Loop in Seconds (Better too high than too low)
DATA_RECORD_TIME = 100

#############################

time_short = time.strftime("%b%d", time.localtime())
dataName = f"{time_short}_linearNormal_{FORCE_VAL}N_FR_{FEED_RATE}_{DATA_RECORD_TIME}s"

forceValues = [FORCE_VAL] * REPETITIONS

if __name__ == '__main__':

    while True:

        p = subprocess.Popen(f"python3 ./send_record_forcedata_experiment.py {DATA_RECORD_TIME} {dataName}", shell=True)    #Execute script for sending force data, receive telemetry and record data

        print('Initial wait...')
        time.sleep(INITIAL_WAIT)


        for val in forceValues:

            cmd = f"F {val} R {FEED_RATE} Back"

            SOCK_SEND.sendto(cmd.encode(), (NUC_IP, UDP_PORT_SEND_CMD))     #Send the command string to the Realtime-PC
            waitForResponse(SOCK_RECEIVE)                                   #Wait for Robot to finish task

        p.wait()                                                            #Wait for data recording to end

        rep = repeatExp()                                                   #Ask if experiment should be repeated

        if not rep:
            break
                                                                                                        #End loop and terminate script


    print("----------")
    print("Now terminating.")
    






               
          


