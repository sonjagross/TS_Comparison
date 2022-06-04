#
#Basic description found in "exp_linear_normal.py"
#
#Specific description:

import socket
import time
import subprocess

from exp_config import *

SOCK_RECEIVE = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # INTERNET, UDP

SOCK_SEND = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # INTERNET, UDP
SOCK_RECEIVE.bind((HOST_IP, UDP_PORT_RECEIVE_OK))

############################# Modify this as required

#Experiment Parameters

FORCE_VAL = 1            #Target Force in Newton
SLIP_VEL = 0.0001        #Target Velocity in m/s / 250(Interface timing) | 0.0001 * 250 * 1000 =  25 mm/s
INITIAL_WAIT = 0         #Time in Seconds
DATA_RECORD_TIME = 10    #Duration for Data Record Loop in Seconds (Better too high than too low)
DWELL_ON_CONTACT = 3     #Duration that contact force should be held before sliding in Seconds
#############################


time_short = time.strftime("%b%d", time.localtime())
dataName = f"{time_short}_Slip_{FORCE_VAL}N_FR_{SLIP_VEL}_{DATA_RECORD_TIME}s"


if __name__ == '__main__':



        while True:
            record_timer = time.time()

            cmd = f"S {FORCE_VAL} Stay" 
            SOCK_SEND.sendto(cmd.encode(), (NUC_IP, UDP_PORT_SEND_CMD))

            time.sleep(3)                                                                                                        #Wait for robot to move into position
            p = subprocess.Popen(f"python3 ./send_record_forcedata_experiment.py {DATA_RECORD_TIME} {dataName}", shell=True)     #Start recording        

            waitForResponse(SOCK_RECEIVE)

            time.sleep(DWELL_ON_CONTACT)

            cmd = f"S {SLIP_VEL} Slip"                                                                                           #Start sliding
            SOCK_SEND.sendto(cmd.encode(), (NUC_IP, UDP_PORT_SEND_CMD))
            waitForResponse(SOCK_RECEIVE)


            p.wait()    #Wait for recording to end
            rep = repeatExp()

            if not rep:
                break



        print("----------")
        print("Now terminating.")
        




               
          


