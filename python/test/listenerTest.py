import socket
import time 
import struct 
import sys 
sys.path.append("/home/mfl24/Documents/chaiProjects/hapticEnvironment/python")
import messageDefinitions as md
import Messenger as MR
from ctypes import * 
import Globals

client = Globals.getClient()
client.call("addModule", 2, "127.0.0.1", 9000)
client.call("subscribeTo", 2, 1)
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
sock.bind(("127.0.0.1", 9000))
while True:
  data, addr = sock.recvfrom(md.MAX_PACKET_LENGTH)
  header = md.MSG_HEADER()
  MR.readMessage(data, header)
  if header.msg_type == md.HAPTIC_DATA_STREAM:
    testData = md.M_HAPTIC_DATA_STREAM()
    MR.readMessage(data, testData)
    print(testData.header.serial_no, testData.header.msg_type, testData.header.timestamp, testData.posX, testData.posY, testData.posZ)
  else:
    print(header.msg_type)

"""
#startRecording= md.M_START_RECORDING()
#startRecording.header.msg_type = c_int(md.START_RECORDING)
#fileName = create_string_buffer(b"/home/mfl24/data/RnelShare/users/mfl24/test.csv", md.MAX_STRING_LENGTH)
#fileNamePtr = (c_char_p) (addressof(fileName))
#startRecording.filename = fileNamePtr.value
#packet = MR.makeMessage(startRecording)
#sock.sendto(packet, (Globals.MESSAGING_IP, Globals.MESSAGING_PORT))

while True:
  data, addr = sock.recvfrom(md.MAX_PACKET_LENGTH)
  header = md.MSG_HEADER()
  MR.readMessage(data, header)
  if header.msg_type == md.CST_DATA:
    cstData = md.M_CST_DATA()
    MR.readMessage(data, cstData)
    cursorY = cstData.cursorY
    print(cursorY)
  else:
    print(header.msg_type)
    #keyName = keypress.keyname.decode('utf-8')
    #print(keyName)
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
"""
