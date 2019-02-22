import socket
import time 

UDP_IP = "127.0.0.1"
UDP_PORT = 10000

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))
count = 0
while True:
  data, addr = sock.recvfrom(1024)
  count = count+1
  print("Received packet #: ", count)
  time.sleep(0.5)
