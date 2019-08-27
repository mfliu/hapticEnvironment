import socket
import time
import struct
import sys 
sys.path.append("/home/mfl24/Documents/chaiProjects/hapticEnvironment/python")
import messageDefinitions as md
import Messenger as MR
from ctypes import * 
import msgpackrpc
import Globals
#RPC_IP = "127.0.0.1"
#RPC_PORT = 8080

client = Globals.getClient()
#client.call("testMessage", 9000)
client.call("addModule", 1, "127.0.0.1", 7000)
time.sleep(10)
testPacket = md.M_TEST_PACKET()
testPacket.header.msg_type = md.TEST_PACKET
testPacket.a = c_int(10)
testPacket.b = c_int(-1)
print(testPacket.header.msg_type)
testPacket = bytes(MR.makeMessage(testPacket))
print(len(testPacket))
client.call("sendMessage", testPacket, len(testPacket), 1)

"""
UDP_IP = "127.0.0.1"
UDP_PORT = 7000

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
sock.connect((UDP_IP, UDP_PORT))


center = md.M_GRAPHICS_SHAPE_BOX()
center.header.msg_type = c_int(md.GRAPHICS_SHAPE_BOX)
name = create_string_buffer(bytes("center", 'utf-8'), md.MAX_STRING_LENGTH)
namePtr = (c_char_p) (addressof(name))
center.objectName = namePtr.value 
center.sizeX = c_double(0)
center.sizeY = c_double(50)
center.sizeZ = c_double(50)
center.localPosition = (c_double*3) (-200.0, 0.0, 0.0)
center.color = (c_float*4) (0.83, 0.83, 0.83, 1.0)
packet = MR.makeMessage(center)
sock.sendto(packet, (UDP_IP, UDP_PORT))

##centerColor = md.M_GRAPHICS_CHANGE_OBJECT_COLOR()
#centerColor.header.msg_type = md.GRAPHICS_CHANGE_OBJECT_COLOR
#name = create_string_buffer(b"center", md.MAX_STRING_LENGTH)
#namePtr = (c_char_p)(addressof(name))
#centerColor.objectName = namePtr.value 
#centerColor.color = (c_float * 4) (1.0, 0.0, 0.0, 1.0)
#packet = MR.makeMessage(centerColor)
#MR.sendMessage(packet)


cst = md.M_CST_CREATE()
cst.header.msg_type = md.CST_CREATE 
name = create_string_buffer(b"cst", md.MAX_STRING_LENGTH)
namePtr = (c_char_p) (addressof(name))
cst.cstName = namePtr.value
cst.lambdaVal = c_double(2)
cst.forceMagnitude = c_double(0.5)
cst.visionEnabled = c_int(1)
cst.hapticEnabled = c_int(1)
packet = MR.makeMessage(cst)
sock.sendto(packet, (UDP_IP, UDP_PORT))


cstStart = md.M_CST_START()
cstStart.header.msg_type = md.CST_START 
name = create_string_buffer(b"cst", md.MAX_STRING_LENGTH)
namePtr = (c_char_p) (addressof(name))
cstStart.cstName = namePtr.value
packet = MR.makeMessage(cstStart)
sock.sendto(packet, (UDP_IP, UDP_PORT))
"""
