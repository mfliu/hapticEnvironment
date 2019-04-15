import sys 
sys.path.append("/home/mfl24/Documents/chaiProjects/hapticControl/python/")
import numpy as np
import messageDefinitions as md
from ctypes import *
import Messenger as MR
import Globals 

def enableGraphics(objectName, setVal):
  name = create_string_buffer(bytes(objectName, 'utf-8'), md.MAX_STRING_LENGTH)
  namePtr = (c_char_p) (addressof(name))
  display = md.M_GRAPHICS_SET_ENABLED()
  display.header.msg_type = c_int(md.GRAPHICS_SET_ENABLED)
  display.objectName = namePtr.value
  display.enabled = c_int(setVal)
  packet = MR.makeMessage(display)
  MR.sendMessage(packet)

def makeSphere(objectName, radius, position, color, enabled):
  sphereObj = md.M_GRAPHICS_SHAPE_SPHERE()
  sphereObj.header.msg_type = c_int(md.GRAPHICS_SHAPE_SPHERE)
  name = create_string_buffer(bytes(objectName, 'utf-8'), md.MAX_STRING_LENGTH)
  namePtr = (c_char_p) (addressof(name))
  sphereObj.objectName = namePtr.value
  sphereObj.radius = c_double(radius)
  sphereObj.localPosition = (c_double * 3) (position[0], position[1], position[2])
  sphereObj.color = (c_float * 4) (color[0], color[1], color[2], color[3])
  packet = MR.makeMessage(sphereObj)
  MR.sendMessage(packet)

def setBackground(red, green, blue):
  bg = md.M_GRAPHICS_CHANGE_BG_COLOR()
  bg.header.msg_type = c_int(md.GRAPHICS_CHANGE_BG_COLOR)
  bg.color = (c_float * 4) (red, green, blue, 1.0)
  packet = MR.makeMessage(bg)
  MR.sendMessage(packet)


