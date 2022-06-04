#
# This file is required to configure the network parameters required for the experimental setup.
# It also includes two common functions used in every experiment script.
#

import socket
import time

#Network Parameters
HOST_IP = "10.162.15.66"     #IP of the computer running this script
NUC_IP = "10.162.15.46"      #IP of NUC that runs MIOS-Interface   

UDP_PORT_RECEIVE_OK = 6665   #Port where script receives answer if robot has finished
UDP_PORT_RECEIVE_TEL = 6666  #Port where script receives telemetry

UDP_PORT_SEND_CMD = 3332     #Port where MIOS-Interface listens for commands
UDP_PORT_SEND_FORCE = 3333   #Port where MIOS-Interface listens for force data 

END_STRING = "Task"          #Is sent back by the python-mios-interface once the robot finished executing a command

#This function puts the experiments on hold until the robot returns the "finished" signal
def waitForResponse(sock):
     while True:
          print("Waiting for robot to finish...")

          data, addr = sock.recvfrom(1024)
          message = data.decode()

          if END_STRING in message:
               print("Robot has finished.\n")
               break

#This function asks the operator if the experiment should be repeated.
def repeatExp():
    print("Repeat Experiment? Type 'y' to repeat, else 'n'")
    answer = input()

    if answer.lower() == 'y':
        print("Now repeating...")
        repeat = 1
    else:
        repeat = 0
    
    return repeat
