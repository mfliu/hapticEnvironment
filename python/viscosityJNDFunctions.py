import random 
from ctypes import *
import struct 
import Globals 
import time 
import messageDefinitions as md
import Messenger as MR
from ctypes import * 
import socket 
import numpy as np 
import csv 
from mprpc import RPCClient 

UDP_IP = "127.0.0.1"
UDP_PORT = 2000

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
client = RPCClient("127.0.0.1", 8080)

def enableGraphics(objectName, setVal):
    namePtr = (c_char_p) (addressof(objectName))
    message = md.M_GRAPHICS_SET_ENABLED()
    message.header.serialNo = c_int(client.call("getMsgNum"))
    message.header.msg_type = c_int(md.GRAPHICS_SET_ENABLED)
    message.header.timestamp = c_double(client.call("getTimestamp")) 
    message.objectName = namePtr.value 
    message.enabled = c_int(setVal)
    packet = MR.makeMessage(message)
    sock.sendto(packet, (UDP_IP, UDP_PORT))

def removeEffect(effectName):
  rmField = md.M_HAPTICS_REMOVE_FIELD_EFFECT()
  rmField.header.serialNo = c_int(client.call("getMsgNum"))
  rmField.header.msg_type = c_int(md.HAPTICS_REMOVE_FIELD_EFFECT)
  rmField.header.timestamp = c_double(client.call("getTimestamp"))
  fieldNamePtr = (c_char_p) (addressof(effectName))
  rmField.effectName = fieldNamePtr.value
  packet = MR.makeMessage(rmField)
  sock.sendto(packet, (UDP_IP, UDP_PORT)) 

def setup():
  
  refVisc = np.arange(1.5, 3.5, 0.5)
  stepViscUp = np.arange(0.1, 1.5, 0.1)
  stepViscDown = -1*np.arange(0.05, 1.5, 0.1)
  steps = np.sort(np.concatenate([stepViscUp, stepViscDown]))
  fileName = "viscosityJND-Monica-" + time.ctime(time.time()).replace(" ", "_").replace(":", "-") + ".csv"
  logFilePath = "/home/mfl24/data/RnelShare/users/mfl24/Test/" + fileName
  trialNum = 0
  field1Visc = 0.0
  field2Visc = 0.0 
  choice = ""

  taskVars = { "refVisc": refVisc, "steps": steps,\
              "filepath": logFilePath, "trialNum": trialNum,\
              "field1Visc": field1Visc, "field2Visc": field2Visc, "choice": choice }

  with open(logFilePath, 'w') as f:
    fwrite = csv.writer(f)
    fwrite.writerow(["Trial", "Reference Viscosity", "Compare Viscosity", "Choice"])

  sameObj = md.M_GRAPHICS_SHAPE_SPHERE()
  sameObj.header.serial_no = c_int(client.call("getMsgNum"))
  sameObj.header.msg_type = md.GRAPHICS_SHAPE_SPHERE
  sameObj.header.timestamp = c_double(client.call("getTimestamp"))
  sameName = create_string_buffer(b"sameTarget", 128)
  sameNamePtr = (c_char_p) (addressof(sameName))
  sameObj.objectName = sameNamePtr.value
  sameObj.radius = c_double(0.1)
  sameObj.localPosition = (c_double * 3) (0.0, 1.1, 0.0)
  sameObj.color = (c_float * 4) (0, 102/250, 51/250, 1)
  packet = MR.makeMessage(sameObj)
  sock.sendto(packet, (UDP_IP, UDP_PORT))
  enableGraphics(sameName, 0)

  diffObj = md.M_GRAPHICS_SHAPE_SPHERE()
  diffObj.header.serial_no = c_int(client.call("getMsgNum"))
  diffObj.header.msg_type = md.GRAPHICS_SHAPE_SPHERE
  diffObj.header.timestamp = c_double(client.call("getTimestamp"))
  diffName = create_string_buffer(b"diffTarget", 128)
  diffNamePtr = (c_char_p) (addressof(diffName))
  diffObj.objectName = diffNamePtr.value
  diffObj.radius = c_double(0.1)
  diffObj.localPosition = (c_double * 3) (0.0, -1.1, 0.0)
  diffObj.color = (c_float * 4) (152/250, 0, 0, 1)
  packet = MR.makeMessage(diffObj)
  sock.sendto(packet, (UDP_IP, UDP_PORT))
  enableGraphics(diffName, 0)
  
  return taskVars

def setBackground(red, green, blue):
  bg = md.M_GRAPHICS_CHANGE_BG_COLOR()
  bg.header.serialNo = c_int(client.call("getMsgNum"))
  bg.header.msg_type = c_int(md.GRAPHICS_CHANGE_BG_COLOR)
  bg.header.timestamp = c_double(client.call("getTimestamp"))
  bg.color = (c_float * 4) (red, green, blue, 1.0)
  packet = MR.makeMessage(bg)
  sock.sendto(packet, (UDP_IP, UDP_PORT))

def startEntry(options, taskVars):
  taskVars["trialNum"] = taskVars["trialNum"] + 1
  print("Trial " + str(taskVars["trialNum"]))
  setBackground(0.0, 0.0, 0.0)
  sameName = create_string_buffer(b"sameTarget", 128)
  diffName = create_string_buffer(b"diffTarget", 128)
  enableGraphics(sameName, 0)
  enableGraphics(diffName, 0)
  time.sleep(2)
  return "next"

def field1Entry(options, taskVars):
  setBackground(4.0, 133.0, 209.0)
  field1Visc = random.choice(taskVars["refVisc"])
  taskVars["field1Visc"] = field1Visc
  #print("Reference Viscosity: ", field1Visc)
  
  field1 = md.M_HAPTICS_VISCOSITY_FIELD()
  field1.header.serial_no = c_int(client.call("getMsgNum"))
  field1.header.msg_type = c_int(md.HAPTICS_VISCOSITY_FIELD)
  field1.timestamp = c_double(client.call("getTimestamp"))
  field1Name = create_string_buffer(b"field1", md.MAX_STRING_LENGTH)
  field1NamePtr = (c_char_p) (addressof(field1Name))
  field1.effectName = field1NamePtr.value 
  field1.viscosityMatrix = (c_double * 9) (-1*field1Visc, 0.0, 0.0,\
                                           0.0, -1*field1Visc, 0.0,\
                                           0.0, 0.0, -1*field1Visc)
  packet = MR.makeMessage(field1)
  sock.sendto(packet, (UDP_IP, UDP_PORT))
  startTime = time.time()
  red = 4.0 
  green = 133.0
  blue = 209.0 
  while time.time() - startTime < 3:
    increment = round((3.0-(time.time()-startTime))/3.0, 2)
    redF = round(red*increment, 2)
    greenF = round(green*increment, 2)
    blueF = round(blue*increment, 2)
    setBackground(redF, greenF, blueF)
    time.sleep(0.1)
  return "next"

def intermediateEntry(options, taskVars):
  setBackground(0.0, 0.0, 0.0)
  fieldName = create_string_buffer(b"field1", 128)
  removeEffect(fieldName)
  time.sleep(0.5)
  return "next"

def field2Entry(options, taskVars):
  setBackground(250.0, 194.0, 5.0)
  same = np.random.uniform(0, 1) #random.choice([0, 1])
  if same < 0.2:
    field2Visc = taskVars["field1Visc"]
  else:
    field2Visc = round(taskVars["field1Visc"] + random.choice(taskVars["steps"]), 2)
  taskVars["field2Visc"] = field2Visc 
  #print("Field Viscosity: ", taskVars["field2Visc"])
  
  field2 = md.M_HAPTICS_VISCOSITY_FIELD()
  field2.header.serial_no = c_int(client.call("getMsgNum"))
  field2.header.msg_type = c_int(md.HAPTICS_VISCOSITY_FIELD)
  field2.timestamp = c_double(client.call("getTimestamp"))
  field2Name = create_string_buffer(b"field2", md.MAX_STRING_LENGTH)
  field2NamePtr = (c_char_p) (addressof(field2Name))
  field2.effectName = field2NamePtr.value 
  field2.viscosityMatrix = (c_double * 9) (-1*field2Visc, 0.0, 0.0,\
                                           0.0, -1*field2Visc, 0.0,\
                                           0.0, 0.0, -1*field2Visc)
  packet = MR.makeMessage(field2)
  sock.sendto(packet, (UDP_IP, UDP_PORT))
  red = 250.0 
  green = 194.0 
  blue = 5.0 
  startTime = time.time()
  while time.time() - startTime < 3:
    increment = round((3.0-(time.time()-startTime))/5.0, 3)
    redF = round(red*increment, 2)
    greenF = round(green*increment, 2)
    blueF = round(blue*increment, 2)
    setBackground(redF, greenF, blueF)
    time.sleep(0.1)
  return "next"

def decisionEntry(options, taskVars):
  setBackground(0.0, 0.0, 0.0)
  fieldName = create_string_buffer(b"field2", 128)
  removeEffect(fieldName)
  time.sleep(0.5)
  sameName = create_string_buffer(b"sameTarget", 128)
  diffName = create_string_buffer(b"diffTarget", 128)
  enableGraphics(sameName, 1)
  enableGraphics(diffName, 1)
  time.sleep(0.5)
  decisionMade = False
  while decisionMade == False:
    if np.abs(Globals.CHAI_DATA.posY - 1.1) < 0.15 and np.abs(Globals.CHAI_DATA.posZ) < 0.15:
      decisionMade = True
      taskVars["choice"] = 1 #"same"
    elif np.abs(Globals.CHAI_DATA.posY + 1.1) < 0.15 and np.abs(Globals.CHAI_DATA.posZ) < 0.15:
      decisionMade = True
      taskVars["choice"] = 0 #"different"
  #print("Decision: ", taskVars["choice"])
  with open(taskVars["filepath"], 'a') as f:
    fwrite = csv.writer(f)
    fwrite.writerow([taskVars["trialNum"], taskVars["field1Visc"],\
                    taskVars["field2Visc"], taskVars["choice"]])
  return "next"
