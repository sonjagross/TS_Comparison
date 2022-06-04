#
#Basic description found in "exp_linear_normal.py"
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
FORCE_VAL_Z = 5          #Force in Newton in Z direction
FORCE_VAL_XY = 2         #Force in Newton in XY direction

ANGLE = 180              #Angle of Target Force
FEED_RATE = 0.00001      #Target Velocity in m/s / 250(Interface timing) | 0.00001 * 250 * 1000 =  2.5 mm/s
INITIAL_WAIT = 0         #Time in Seconds

DATA_RECORD_TIME = 25    #Duration for Data Record Loop in Seconds (Better too high than too low)

#############################


time_short = time.strftime("%b%d", time.localtime()).lower()
dataName = f"{time_short}_normalXYZ_Z{FORCE_VAL_Z}_YX{FORCE_VAL_XY}Deg{ANGLE}_FR_{FEED_RATE}_{DATA_RECORD_TIME}s"


if __name__ == '__main__':

  

        while True:

            print('Initial wait...')
            time.sleep(INITIAL_WAIT)

            p = subprocess.Popen(f"python3 ./send_record_forcedata_experiment.py {DATA_RECORD_TIME} {dataName}", shell=True)     


            cmd = f"F {FORCE_VAL_Z} R {FEED_RATE} Stay"                    

            SOCK_SEND.sendto(cmd.encode(), (NUC_IP, UDP_PORT_SEND_CMD))         #Move down to z target force
            waitForResponse(SOCK_RECEIVE)


            cmd = f"F {FORCE_VAL_XY} Angle {ANGLE} Back"

            SOCK_SEND.sendto(cmd.encode(), (NUC_IP, UDP_PORT_SEND_CMD))         #Apply target force in x or y direction
            waitForResponse(SOCK_RECEIVE)

            #time.sleep(2)

            cmd = f"F {FORCE_VAL_Z} R {FEED_RATE} Back"

            SOCK_SEND.sendto(cmd.encode(), (NUC_IP, UDP_PORT_SEND_CMD))         #Deload normal force
            waitForResponse(SOCK_RECEIVE)

            p.wait()


            rep = repeatExp()

            if rep:
                pass
            else:
                break


        print("----------")
        print("Now terminating.")
        






               
          


