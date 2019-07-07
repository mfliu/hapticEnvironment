import socket
import time
import struct
import sys 
sys.path.append("/home/mfl24/Documents/chaiProjects/hapticEnvironment/python")
import messageDefinitions as md
import Messenger as MR
from ctypes import * 

UDP_IP = "127.0.0.1"
UDP_PORT = 7000

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
sock.connect((UDP_IP, UDP_PORT))

cst = md.M_CST_CREATE()
cst.header.msg_type = md.CST_CREATE 
name = create_string_buffer(b"cst", md.MAX_STRING_LENGTH)
namePtr = (c_char_p) (addressof(name))
cst.cstName = namePtr.value
cst.lambdaVal = c_double(1.01)
cst.forceMagnitude = c_double(1e-20)
cst.visionEnabled = c_int(0)
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

