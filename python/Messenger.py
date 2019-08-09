from ctypes import *
import Globals

def readMessage(data, message):
  memmove(addressof(message), data, sizeof(message))

def makeMessage(message):
  client = Globals.getClient()
  message.header.serial_no = client.call_async("getMsgNum").get()
  message.header.timestamp = client.call_async("getTimestamp").get()
  #data = create_string_buffer(sizeof(message))
  #memmove(addressof(data), addressof(message), sizeof(data))
  return message #data

def sendMessage(packet):
  Globals.getSenderSocket().sendto(packet, (Globals.SENDER_IP, Globals.SENDER_PORT))
