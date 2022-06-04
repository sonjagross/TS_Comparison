# 
# Module for communicating with the GSV8
# This module is provided by MEmesssysteme
# https://www.me-systeme.de/de/software/programmierung/python
# 

import serial
import struct



def bytesTofloat(data):
    return struct.unpack('>f', data)[0]

def bytesTouint32(data):
    return struct.unpack('>I', data)[0]

def floatTobytes(value):
    return struct.pack('>f', value)

def uint32Tobytes(value):
    return struct.pack('>I', value)


def GetValue():
    return b'\xAA\x90\x3B\x85'

def StopTransmission():
    return b'\xAA\x90\x23\x85'

def StartTransmission():
    return b'\xAA\x90\x24\x85'

def ReadUserScale(Channel):
    return b'\xAA\x91\x14' + Channel.to_bytes(1, byteorder='big') + b'\x85'

def WriteUserScale(Channel, newUserScale):
    return b'\xAA\x95\x15' + Channel.to_bytes(1, byteorder='big') + floatTobytes(newUserScale) + b'\x85'

def ReadUserOffset(Channel):
    return b'\xAA\x91\x9A' + Channel.to_bytes(1, byteorder='big') + b'\x85'

def WriteUserOffset(Channel, newUserOffset):
    return b'\xAA\x95\x9B' + Channel.to_bytes(1, byteorder='big') + floatTobytes(newUserOffset) + b'\x85'

def ReadDataRate():
    return b'\xAA\x90\x8A\x85'

def WriteDataRate(DataRate):
    return b'\xAA\x94\x8B' + floatTobytes(DataRate) + b'\x85'

def ValSetZero(Channel):
    return b'\xAA\x91\x0C' + Channel.to_bytes(1, byteorder='big') + b'\x85'



def getDataRate(serialConnection):
    serialConnection.write(ReadDataRate())
    AnswFrame = serialConnection.read(8)
    DataRate = bytesTofloat(AnswFrame[3:7])
    return DataRate

def setDataRate(serialConnection,DataRate):
    serialConnection.write(WriteDataRate(DataRate))
    AnswFrame = serialConnection.read(4)
    print("AnswFrame: ", AnswFrame.hex())

def getUserScale(serialConnection,Channel):
    serialConnection.write(ReadUserScale(Channel))
    AnswFrame = serialConnection.read(8)
    UserScale = bytesTofloat(AnswFrame[3:7])
    return UserScale

def setUserScale(serialConnection,Channel, UserScale):
    serialConnection.write(WriteUserScale(Channel, UserScale))
    AnswFrame = serialConnection.read(4)
    print("AnswFrame: ", AnswFrame.hex())

def SetZero(serialConnection, Channel):
    serialConnection.write(ValSetZero(Channel))
    AnswFrame = serialConnection.read(4)
    print("AnswFrame: ", AnswFrame.hex())

def getMeasValues(serialConnection):
    serialConnection.write(GetValue())
    MeasFrame = serialConnection.read(36)
    MeasValues = MeasFrameToMeasValues(MeasFrame)
    return MeasValues

def MeasFrameToMeasValues(MeasFrame):
    MeasValues = []
    for i in range(8):
        MeasValues.append(bytesTofloat(MeasFrame[i*4+3:i*4+7]))
    return MeasValues


