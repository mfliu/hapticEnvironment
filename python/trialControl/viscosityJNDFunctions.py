import sys 
import Globals
sys.path.append(Globals.UTILS_PATH)
from graphics import *
from haptics import * 
import random 
import struct 
import time 
import messageDefinitions as md
import Messenger as MR
from ctypes import * 
import numpy as np 
import csv 

def setup(saveFilePrefix):
  refVisc = np.arange(1.5, 3.0, 0.5)
  stepViscUp = np.array([0.05, 0.1, 0.3, 0.5, 0.75, 1.0, 1.5]) 
  stepViscDown = -1*stepViscUp #np.array([0.03, 0.05, 0.1, 0.25, 0.3, 0.6, 1.0])
  steps = np.sort(np.concatenate([stepViscUp, stepViscDown]))
  logFilePath = saveFilePrefix + "_data.csv"
  msgFilePath = saveFilePrefix + "_msg.log"
  trialNum = 0
  field1Visc = 0.0
  field2Visc = 0.0 
  choice = -1

  fmsg = open(msgFilePath, 'ab')
  csvFile = open(logFilePath, 'a')
  fdata = csv.writer(csvFile)
  fmsg.flush()
  taskVars = { "refVisc": refVisc, "steps": steps, "choiceReady": False, \
              "filepath": logFilePath, "msgFilePath": msgFilePath, "trialNum": trialNum,\
              "field1Visc": field1Visc, "field2Visc": field2Visc, "choice": choice,\
              "msgFile": fmsg, "logFile": fdata, "logFilePtr": csvFile, \
              "GUITree": ["field1Visc", "field2Visc", "choice"]}
  
  fdata.writerow(["Trial", "Reference Viscosity", "Compare Viscosity", "Choice"])

  makeSphere("field1Target", 0.1, [0.0, 1.0, 0.0], [4/256, 133/256, 209/256, 1], 0)
  makeSphere("field2Target", 0.1, [0.0, -1.0, 0.0], [250/256, 194/256, 5/256, 1], 0)
  makeSphere("center", 0.1, [0.0, 0.0, 0.0], [1.0/256, 50.0/256, 32.0/256, 1], 0)
  makeSphere("field1", 0.1, [0.0, 1.0, 0.0], [4/256, 133/256, 209/256, 1], 0)
  makeSphere("field2", 0.1, [0.0, 0.0, 0.0], [250/256, 194/256, 5/256, 1], 0)
  return taskVars

def readMessage(message, options, taskVars):
  header = md.MSG_HEADER()
  MR.readMessage(message, header)
  print(header.msg_type)
#  if header.msg_type == md.KEYPRESS:
#    keypress = md.M_KEYPRESS()
#    MR.readMessage(message, keypress)
#    keyName = keypress.keyname.decode('utf-8')
#    if keyName == "space":
#      taskVars["choiceReady"] = True 
#      taskVars["msgFile"].flush()
#      taskVars["msgFile"].write(keypress)

def startEntry(options, taskVars):
  trialStart = md.M_TRIAL_START()
  trialStart.header.msg_type = c_int(md.TRIAL_START)
  packet = MR.makeMessage(trialStart)
  MR.sendMessage(packet)

  taskVars["msgFile"].flush()
  taskVars["msgFile"].write(packet)
  taskVars["trialNum"] = taskVars["trialNum"] + 1
  taskVars["choice"] = -1
  taskVars["choiceReady"] = False
  
  ref = random.choice(taskVars["refVisc"])
  step = round(ref + random.choice(taskVars["steps"]), 2)
  getsRef = random.choice([1, 2])
  if getsRef == 1:
    field1Visc = ref
    field2Visc = step 
  else:
    field1Visc = step
    field2Visc = ref
  
  taskVars["field1Visc"] = field1Visc
  taskVars["field2Visc"] = field2Visc 

  enableGraphics("field1Target", 0)
  enableGraphics("field2Target", 0)
  enableGraphics("center", 0)
  enableGraphics("field1", 0)
  enableGraphics("field2", 0)
  #time.sleep(1.0)
  
  enableGraphics("center", 1)
  reachedTarget = False
  while reachedTarget == False:
    if np.abs(Globals.CHAI_DATA.posY) < 0.15 and np.abs(Globals.CHAI_DATA.posZ) < 0.15:
      reachedTarget = True
      enableGraphics("center", 0)
  return "next"

def field1Entry(options, taskVars):
  #setBackground(4.0, 133.0, 209.0)
  field1Visc = taskVars["field1Visc"]  
  viscosityMatrix = [-1*field1Visc, 0.0, 0.0,\
                     0.0, -1*field1Visc, 0.0,\
                     0.0, 0.0, -1*field1Visc]
  packet = viscousField("field1", viscosityMatrix)

  taskVars["msgFile"].flush()
  taskVars["msgFile"].write(packet)
  enableGraphics("field1", 1)

  reachedTarget = False
  while reachedTarget == False:
    if np.abs(Globals.CHAI_DATA.posY - 1.0) < 0.15 and np.abs(Globals.CHAI_DATA.posZ) < 0.15:
      enableGraphics("field1", 0)
      reachedTarget = True
      return "next" 
  return "next"

#def intermediateEntry(options, taskVars):
#  removeEffect("field1")
#  setBackground(132.0, 11.0, 11.0)
#  time.sleep(1.5)
#  return "next"

def field2Entry(options, taskVars):
  removeEffect("field1")
  #setBackground(250.0, 194.0, 5.0)
  field2Visc = taskVars["field2Visc"]
  viscosityMatrix = [-1*field2Visc, 0.0, 0.0,\
                    0.0, -1*field2Visc, 0.0,\
                    0.0, 0.0, -1*field2Visc]
  packet = viscousField("field2", viscosityMatrix)

  taskVars["msgFile"].flush()
  taskVars["msgFile"].write(packet)
  
  enableGraphics("field2", 1)
  reachedTarget = False
  while reachedTarget == False:
    if np.abs(Globals.CHAI_DATA.posY) < 0.15 and np.abs(Globals.CHAI_DATA.posZ) < 0.15:
      enableGraphics("field2", 0)
      reachedTarget = True
      return "next"
  return "next"

def decisionEntry(options, taskVars):
  removeEffect("field2")
  #setBackground(0.0, 0.0, 0.0) 
  enableGraphics("field1Target", 1)
  enableGraphics("field2Target", 1)

  while taskVars["choice"] == -1:
    if np.abs(Globals.CHAI_DATA.posY - 1.0) < 0.15 and np.abs(Globals.CHAI_DATA.posZ) < 0.15:
      taskVars["choice"] = 2 
    elif np.abs(Globals.CHAI_DATA.posY + 1.0) < 0.15 and np.abs(Globals.CHAI_DATA.posZ) < 0.15:
      taskVars["choice"] = 1 
    
  taskVars["logFile"].writerow([taskVars["trialNum"], taskVars["field1Visc"],\
                    taskVars["field2Visc"], taskVars["choice"]])
  
  trialEnd = md.M_TRIAL_END()
  trialEnd.header.msg_type = md.TRIAL_END
  packet = MR.makeMessage(trialEnd)
  MR.sendMessage(packet)
  
  taskVars["msgFile"].flush()
  taskVars["msgFile"].write(packet)

  taskControlPtr = taskVars["taskControl"]
  for k in taskVars.keys():
    if k != "taskControl":
      taskControlPtr.sessionInfo[k] = taskVars[k]
  taskControlPtr.addNode()
  return "next"
