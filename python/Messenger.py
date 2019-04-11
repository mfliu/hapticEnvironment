import ctypes
import Globals

def readMessage(data, message):
  ctypes.memmove(ctypes.addressof(message), data, ctypes.sizeof(message))

def makeMessage(message):
  client = Globals.getClient()
  message.header.serial_no = client.call("getMsgNum")
  message.header.timestamp = client.call("getTimestamp")
  data = ctypes.create_string_buffer(ctypes.sizeof(message))
  ctypes.memmove(ctypes.addressof(data), ctypes.addressof(message), ctypes.sizeof(data))
  return data
