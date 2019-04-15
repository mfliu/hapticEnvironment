import socket
import time
import struct
import messageDefinitions as md
import Messenger as MR
from ctypes import * 

UDP_IP = "127.0.0.1"
UDP_PORT = 7000

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
sock.connect((UDP_IP, UDP_PORT))

arrow = md.M_GRAPHICS_ARROW()
arrow.header.msg_type = md.GRAPHICS_ARROW
arrowName = create_string_buffer(b"arrow", md.MAX_STRING_LENGTH)
arrowNamePtr = (c_char_p) (addressof(arrowName))
arrow.objectName = arrowNamePtr.value
arrow.aLength = c_double(1.0)
arrow.shaftRadius = c_double(0.01)
arrow.lengthTip = c_double(0.02)
arrow.radiusTip = c_double(0.02)
arrow.bidirectional = c_int(1)
arrow.direction = (c_double * 3) (0.0, 0.0, 1.0)
arrow.position = (c_double * 3) (0.0, 0.0, -0.5) 
arrow.color = (c_float * 4) (255.0, 0.0, 0.0, 1.0)
packet = MR.makeMessage(arrow)
sock.sendto(packet, (UDP_IP, UDP_PORT))
