#
#Basic description found in "exp_linear_normal.py"
#
#Specific description:
#Time sleep time after sending the command is added such that the recording of the force starts only once the end effector is in position (Robot movement may cause the force threshold for recording to be exceeded). Can be removed if it is not relevant.

import socket
import time
import subprocess

from exp_config import *



SOCK_RECEIVE = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # INTERNET, UDP

SOCK_SEND = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # INTERNET, UDP
SOCK_RECEIVE.bind((HOST_IP, UDP_PORT_RECEIVE_OK))

############################# Modify this as required


#Experiment Parameters
FORCE_VAL = 2           #Target Force in Newton
ANGLE = 90             #Angle of Target Force
FEED_RATE = 0.00001     #Target Velocity in m/s / 250(Interface timing) | 0.00001 * 250 * 1000 =  2.5 mm/s
INITIAL_WAIT = 0        #Time in Seconds
DATA_RECORD_TIME = 25   #Duration for Data Record Loop in Seconds (Better too high than too low)

#############################


time_short = time.strftime("%b%d", time.localtime()).lower()
dataName = f"{time_short}_normalXY_F{FORCE_VAL}_Deg{ANGLE}_FR_{FEED_RATE}_{DATA_RECORD_TIME}s"


if __name__ == '__main__':

        print('Initial wait...')
        time.sleep(INITIAL_WAIT)
 

        while True:

            cmd = f"FN {FORCE_VAL} R {FEED_RATE} Angle {ANGLE} Back"                                                         #Send the command string to the Realtime-PC
            SOCK_SEND.sendto(cmd.encode(), (NUC_IP, UDP_PORT_SEND_CMD))     
            time.sleep(8)                                                                                                    #Wait with recording until robot is in position, Needs adjustment
            p = subprocess.Popen(f"python3 ./send_record_forcedata_experiment.py {DATA_RECORD_TIME} {dataName}", shell=True)            
            waitForResponse(SOCK_RECEIVE)                                                                                    #Wait for Robot to finish task

            p.wait()

            rep = repeatExp()

            if not rep:
                break


        print("----------")
        print("Now terminating.")
        






               
          


