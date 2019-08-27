from ctypes import *
import Globals

def readMessage(data, message):
  memmove(addressof(message), data, sizeof(message))

def makeMessage(message):
  client = Globals.getClient()
  message.header.serial_no = client.call_async("getMsgNum").get()
  message.header.timestamp = client.call_async("getTimestamp").get()
  return message 

def sendMessage(packet):
  client = Globals.getClient()
  client.call("sendMessage", bytes(packet), sizeof(packet), Globals.MODULE_NUM) 
  #Globals.getSenderSocket().sendto(packet, (Globals.SENDER_IP, Globals.SENDER_PORT))
