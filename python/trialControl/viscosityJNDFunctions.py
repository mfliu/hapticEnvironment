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
  stepViscUp = np.array([0.01, 0.15, 0.2, 0.35, 0.45, 0.75, 1.5]) 
  stepViscDown = -1*np.array([0.03, 0.05, 0.1, 0.25, 0.3, 0.6, 1.0])
  steps = np.sort(np.concatenate([stepViscUp, stepViscDown]))
  logFilePath = saveFilePrefix + "_data.csv"
  msgFilePath = saveFilePrefix + "_msg.log"
  trialNum = 0
  field1Visc = 0.0
  field2Visc = 0.0 
  choice = -1
  arrowDir = "verticalArrow"

  fmsg = open(msgFilePath, 'ab')
  csvFile = open(logFilePath, 'a')
  fdata = csv.writer(csvFile)
  fmsg.flush()
  taskVars = { "refVisc": refVisc, "steps": steps,\
              "filepath": logFilePath, "msgFilePath": msgFilePath, "trialNum": trialNum,\
              "field1Visc": field1Visc, "field2Visc": field2Visc, "choice": choice,\
              "msgFile": fmsg, "logFile": fdata, "logFilePtr": csvFile, \
              "arrowDir": arrowDir, "GUITree": ["field1Visc", "field2Visc", "choice"]}
  
  fdata.writerow(["Trial", "Reference Viscosity", "Compare Viscosity", "Choice"])

  makeSphere("field1Target", 0.1, [0.0, -0.5, 1.0], [4/250, 133/250, 209/250, 1], 0)
  makeSphere("field2Target", 0.1, [0.0, 0.5, 1.0], [250/250, 194/250, 5/250, 1], 0)
  makeArrow("verticalArrow", 1.0, 0.01, 0.02, 0.02, 1, [0.0, 0.0, 1.0], [0.0, 0.0, -0.5],\
            [66/256, 66/256, 66/256, 1.0], 0)
  makeArrow("horizontalArrow", 1.0, 0.01, 0.02, 0.02, 1, [0.0, 1.0, 0.0], [0.0, -0.5, 0.0],\
            [66/256, 66/256, 66/256, 1.0], 0)
  
  return taskVars

def startEntry(options, taskVars):
  trialStart = md.M_TRIAL_START()
  trialStart.header.msg_type = c_int(md.TRIAL_START)
  packet = MR.makeMessage(trialStart)
  MR.sendMessage(packet)

  taskVars["msgFile"].flush()
  taskVars["msgFile"].write(packet)
  taskVars["trialNum"] = taskVars["trialNum"] + 1
  taskVars["choice"] = -1
  
  setBackground(0.0, 0.0, 0.0)
  enableGraphics("field1Target", 0)
  enableGraphics("field2Target", 0)
  time.sleep(1)
  
  return "next"

def field1Entry(options, taskVars):
  
  setBackground(4.0, 133.0, 209.0)
  field1Visc = random.choice(taskVars["refVisc"])
  taskVars["field1Visc"] = field1Visc
  
  viscosityMatrix = [-1*field1Visc, 0.0, 0.0,\
                     0.0, -1*field1Visc, 0.0,\
                     0.0, 0.0, -1*field1Visc]
  packet = viscousField("field1", viscosityMatrix)
  enableGraphics(taskVars["arrowDir"], 1)

  taskVars["msgFile"].flush()
  taskVars["msgFile"].write(packet)

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
  removeEffect("field1")
  enableGraphics(taskVars["arrowDir"], 0)
  setBackground(132.0, 11.0, 11.0)
  time.sleep(2.0)
  return "next"

def field2Entry(options, taskVars):
  setBackground(250.0, 194.0, 5.0)
  field2Visc = round(taskVars["field1Visc"] + random.choice(taskVars["steps"]), 2)
  taskVars["field2Visc"] = field2Visc 
  
  viscosityMatrix = [-1*field2Visc, 0.0, 0.0,\
                    0.0, -1*field2Visc, 0.0,\
                    0.0, 0.0, -1*field2Visc]
  packet = viscousField("field2", viscosityMatrix)
  enableGraphics(taskVars["arrowDir"], 1)

  taskVars["msgFile"].flush()
  taskVars["msgFile"].write(packet)
  
  enableGraphics("field1Target", 1)
  enableGraphics("field2Target", 1)

  red = 250.0 
  green = 194.0 
  blue = 5.0 
  startTime = time.time()
  prevTime = time.time()
  while time.time() - startTime < 3:
    if time.time() - prevTime > 0.1:
      increment = round((3.0-(time.time()-startTime))/5.0, 3)
      redF = round(red*increment, 2)
      greenF = round(green*increment, 2)
      blueF = round(blue*increment, 2)
      setBackground(redF, greenF, blueF)
      prevTime = time.time()
    if np.abs(Globals.CHAI_DATA.posY - 0.5) < 0.15 and np.abs(Globals.CHAI_DATA.posZ - 1.0) < 0.15:
      taskVars["choice"] = 2
      return "next"
    elif np.abs(Globals.CHAI_DATA.posY + 0.5) < 0.15 and np.abs(Globals.CHAI_DATA.posZ - 1.0) < 0.15:
      taskVars["choice"] = 1 
      return "next"
    time.sleep(0.005)
  return "next"

def decisionEntry(options, taskVars):
  removeEffect("field2")
  setBackground(0.0, 0.0, 0.0)
  enableGraphics(taskVars["arrowDir"], 0)

  decisionMade = False
  if taskVars["choice"] != -1:
    decisionMade = True
  
  while decisionMade == False:
    if np.abs(Globals.CHAI_DATA.posY - 0.5) < 0.15 and np.abs(Globals.CHAI_DATA.posZ - 1.0) < 0.15:
      decisionMade = True
      taskVars["choice"] = 2 
    elif np.abs(Globals.CHAI_DATA.posY + 0.5) < 0.15 and np.abs(Globals.CHAI_DATA.posZ - 1.0) < 0.15:
      decisionMade = True
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
