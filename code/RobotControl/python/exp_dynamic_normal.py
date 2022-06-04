#
#Basic description found in "exp_linear_normal.py"
#
#Specific description:
#Different rates can be used in this experiment by changing the RATE2 parameter and the amount of specific repetitions


import socket
import time
import subprocess

from exp_config import *



SOCK_RECEIVE = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # INTERNET, UDP
SOCK_SEND = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # INTERNET, UDP
SOCK_RECEIVE.bind((HOST_IP, UDP_PORT_RECEIVE_OK))

############################# Modify this as required
#Don't do a rate higher than 0.0001


# #Experiment Parameters
# RATE1 = 0.00005
# RATE2 = 0.0001
# FEED_RATES = [RATE1] * 3 + [RATE2] * 3    #Different force rates in N/s


#Experiment Parameters
#RATE1 = 0.0001         #Target Velocity in m/s / 250(Interface timing) | 0.00001 * 250 * 1000 =  2.5 mm/s
RATE1 = 0.00001         #
REPETITIONS_1 = 5      #Amount of cycles with RATE1

RATE2 = 0
REPETITIONS_2 = 0       

FORCE_VAL = 10           #Target Force in Newton
INITIAL_WAIT = 1        #Time in Seconds
DATA_RECORD_TIME = 50    #Duration for Data Record Loop in Seconds (Better too high than too low)

FEED_RATES = [RATE1] * REPETITIONS_1 + [RATE2] * REPETITIONS_2   #Force rates in N/s, creates the sequence of different force rates that the robot applies

#############################


time_short = time.strftime("%b%d", time.localtime())
dataName = f"{time_short}_dynamicNormal_{FORCE_VAL}N_FR_{RATE1}_{RATE2}_{DATA_RECORD_TIME}s"

if __name__ == '__main__':



        while True:

            p = subprocess.Popen(f"python3 ./send_record_forcedata_experiment.py {DATA_RECORD_TIME} {dataName}", shell=True)   #Execute script for sending force data, receive telemetry and record data
            
            time.sleep(INITIAL_WAIT)

            for rate in FEED_RATES:

                cmd = f"F {FORCE_VAL} R {rate} C Back"                          #Command for dynamic cyclic testing
                SOCK_SEND.sendto(cmd.encode(), (NUC_IP, UDP_PORT_SEND_CMD))     #Send the command string to the Realtime-PC
                waitForResponse(SOCK_RECEIVE)


            p.wait()
            rep = repeatExp()

            if not rep:
                break



        print("----------")
        print("Now terminating.")
        






               
          


