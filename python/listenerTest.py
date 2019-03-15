import socket
import time 
import struct 
import messageDefinitions as md
import Messenger as MR
from ctypes import * 
UDP_IP = "127.0.0.1"
UDP_PORT = 10000

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))
while True:
  data, addr = sock.recvfrom(1024)
  msg_data = md.M_HAPTIC_DATA_STREAM()
  MR.readMessage(data, msg_data)
  collisions = c_char_p(addressof(msg_data.collisions))
  collisionData = struct.unpack(str(len(collisions.value)) + 's', collisions.value)
  time.sleep(0.001)
