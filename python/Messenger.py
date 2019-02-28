import ctypes

def readMessage(data, message):
  ctypes.memmove(ctypes.addressof(message), data, ctypes.sizeof(message))

def makeMessage(message):
  data = ctypes.create_string_buffer(ctypes.sizeof(message))
  ctypes.memmove(ctypes.addressof(data), ctypes.addressof(message), ctypes.sizeof(data))
  return data
