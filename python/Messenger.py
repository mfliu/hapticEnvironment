from ctypes import *
import Globals

def readMessage(data, message):
  memmove(addressof(message), data, sizeof(message))

def makeMessage(message):
  client = Globals.getClient()
  message.header.serial_no = client.call("getMsgNum")
  message.header.timestamp = client.call("getTimestamp")
  data = create_string_buffer(sizeof(message))
  memmove(addressof(data), addressof(message), sizeof(data))
  return data

def sendMessage(packet):
  Globals.getSenderSocket().sendto(packet, (Globals.SENDER_IP, Globals.SENDER_PORT))
