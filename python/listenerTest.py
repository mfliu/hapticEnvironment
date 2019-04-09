import socket
import time 
import struct 
import sys 
import messageDefinitions as md
import Messenger as MR
from ctypes import * 
UDP_IP = "127.0.0.1"
UDP_PORT = 20000

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
sock.bind((UDP_IP, UDP_PORT))


while True:
  data, addr = sock.recvfrom(md.MAX_PACKET_LENGTH)
  msg_data = md.M_HAPTIC_DATA_STREAM()
  MR.readMessage(data, msg_data)
  print("Serial No:", msg_data.header.serial_no)
  print("Msg Type:", msg_data.header.msg_type)
  print("Timestamp:", msg_data.header.timestamp)
  print("Data")
  print(msg_data.posX, msg_data.posY, msg_data.posZ)
  for i in range(0, len(msg_data.collisions)):
    collision = c_char_p(addressof(msg_data.collisions[i]))
    collisionName = struct.unpack(str(len(collision.value)) + 's', collision.value)
    print(collisionName)
  time.sleep(0.001)
