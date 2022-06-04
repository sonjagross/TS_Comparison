#
#Basic description found in "exp_linear_normal.py"
#
#Specific description:
#This script combines different commands to create the quasistatic force curve.
#Note that in addition to the defined pause time between steps another 10-13s are added due to the python-MIOS-interface constraints regarding commands. 

import socket
import time
import subprocess

from exp_config import *



SOCK_RECEIVE = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # INTERNET, UDP
SOCK_SEND = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # INTERNET, UDP

SOCK_RECEIVE.bind((HOST_IP, UDP_PORT_RECEIVE_OK))

############################# Modify this as required

#Experiment Parameters
REPETITIONS = 3         #Amount of Cycles
FEED_RATE = 0.00001     #Target Velocity in m/s / 250(Interface timing) | 0.00001 * 250 * 1000 =  2.5 mm/s
FORCE_VAL = 10          #Target Force in Newton
STEP_WAIT = 20          #Dwell time in seconds 
PAUSE_TIME = 5          #Sleep time between steps (+ 10-13s)
INITIAL_WAIT = 1        #Time in Seconds
DATA_RECORD_TIME = 120  #Duration for Data Record Loop in Seconds (Better too high than too low)

#############################

time_short = time.strftime("%b%d", time.localtime())
dataName = f"{time_short}_quasiStatic_{FORCE_VAL}N_FR_{FEED_RATE}_{DATA_RECORD_TIME}s"


if __name__ == '__main__':


        while True:

            p = subprocess.Popen(f"python3 ./send_record_forcedata_experiment.py {DATA_RECORD_TIME} {dataName}", shell=True)       #Execute script for sending force data, receive telemetry and record data
            
            print('Initial wait...')
            time.sleep(INITIAL_WAIT)


            for step in [FORCE_VAL] * REPETITIONS :
                cmd = f"F {step} R {FEED_RATE} Stay"

                SOCK_SEND.sendto(cmd.encode(), (NUC_IP, UDP_PORT_SEND_CMD))                                 #Send the command string to the Realtime-PC
                waitForResponse(SOCK_RECEIVE)                                                               #Wait for Robot to finish task

                print(f"Holding {step} N for {STEP_WAIT} seconds...")
                time.sleep(STEP_WAIT)                                                                       #Waiting for the set dwell time
    
                SOCK_SEND.sendto(f"F {step} R {FEED_RATE} Back".encode(), (NUC_IP, UDP_PORT_SEND_CMD))      #Send command to return back to initial position
                waitForResponse(SOCK_RECEIVE)                                                               #Wait for Robot to finish task

                time.sleep(PAUSE_TIME)                                                                      #Pause time between steps


            p.wait()                                                            #Wait for data recording to end

            rep = repeatExp()                                                   #Ask if experiment should be repeated

            if not rep:
                break


        print("----------")
        print("Now terminating.")
        






               
          


