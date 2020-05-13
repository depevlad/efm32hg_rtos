#!/usr/bin/python3

import serial
import sys

# Output is little endian.
def toBytes(x):
    if x == 0:
        return bytearray([0])
    bytesList = []
    while x > 0:
        bytesList.append(x % 256)
        x //= 256
    return bytearray(bytesList)

MAGIC_NR = int(0x01234567)

serialPort = serial.Serial('/dev/cu.usbmodem14101')

def serialOut(message):
    serialPort.write(message)
    serialPort.close()
    serialPort.open()

#   Layout of a program in flash:
#      0-3  : MAGIC NR
#      4-7  : ENTRY
#      8-23 : NAME
#      24-25: RAM SPACE
#      26-27: STK SPACE
#      28-31: RESERVED

#  ------------------------------

#   Layout of a header:
#      0-3  : MAGIC NR
#      4-7  : ENTRY
#      8-23 : NAME
#      24-25: RAM SPACE
#      26-27: STK SPACE
#      28-28: PAGE ID
#      29-31: RESERVED

def buildHeader(name, entry, ramSpace, stkSpace, pageId):
    print(toBytes(pageId))
    return (
        toBytes(MAGIC_NR) +
        entry +
        bytearray(name.ljust(16), 'ascii') + 
        toBytes(ramSpace) + 
        toBytes(stkSpace) + 
        toBytes(pageId)
    )

def sendBinary(name, ramSpace, stkSpace, pageId):
    path = name + '/' + name + '.bin'
    with open(path, 'rb') as f:
        contents  = f.read()
        length    = len(contents)
        blockSize = 32

        # Send binary start frame.
        entry  = contents[0 : 4]
        header = buildHeader(name, entry, ramSpace, stkSpace, pageId)
        print(entry)
        print(header)
        serialOut(header)

        # Send binary.
        cntBlocks = length // blockSize
        if length % blockSize != 0:
            cntBlocks += 1
        for i in range(cntBlocks):
            left  = i * blockSize
            right = min(length, left + blockSize)
            message = contents[left : right]
            print(message.hex())
            serialOut(message)
        
        # Send binary end frame.
        serialOut(toBytes(MAGIC_NR))


name     = sys.argv[1]
pageId   = int(sys.argv[2])
ramSpace = int(sys.argv[3])
stkSpace = int(sys.argv[4])
sendBinary(name, ramSpace, stkSpace, pageId)
