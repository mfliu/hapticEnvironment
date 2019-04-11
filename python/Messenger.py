import ctypes
import Globals
#from mprpc import RPCClient

def readMessage(data, message):
  ctypes.memmove(ctypes.addressof(message), data, ctypes.sizeof(message))

def makeMessage(message):
  #client = Globals.getClient()
  message.header.serial_no = ctypes.c_int(1) #client.call("getMsgNum")
  message.header.timestamp = ctypes.c_double(0.5) #client.call("getTimestamp")
  data = ctypes.create_string_buffer(ctypes.sizeof(message))
  ctypes.memmove(ctypes.addressof(data), ctypes.addressof(message), ctypes.sizeof(data))
  return data
