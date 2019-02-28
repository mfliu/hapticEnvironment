import socket
import time 
import struct 
import messageDefinitions as md
import Messenger as MR

UDP_IP = "127.0.0.1"
UDP_PORT = 10000

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))
while True:
  data, addr = sock.recvfrom(1024)
  msg_data = md.M_HAPTIC_DATA_STREAM()
  MR.readMessage(data, msg_data)
  print(msg_data.header.serial_no)
  print(msg_data.header.msg_type)
  print(msg_data.posX)
  time.sleep(0.05)
