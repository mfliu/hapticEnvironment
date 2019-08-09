import sys 
import Globals
sys.path.append(Globals.UTILS_PATH)
from graphics import *
from haptics import *
import struct 
import time
import messageDefinitions as md
import Messenger as MR
from ctypes import *
import numpy as np
import csv


def setup(saveFilePrefix):
  
  ## set up file saving
  metricsFile = saveFilePrefix + "_cst_metrics.csv"
  cursorFile = saveFilePrefix + "_cst_cursor.log"
  msgLogFile = saveFilePrefix + "_cst_msg.log"

  fmetrics = open(metricsFile, 'a')
  fcursor = open(cursorFile, 'ab')
  fmsg = open(msgLogFile, 'ab')
  fmsg.flush()
  fcursor.flush()
  fmetricsWriter = csv.writer(fmetrics)
  fmetricsWriter.writerow(["Trial", "Vision", "Haptic", "Lambda", "Succeeded"])
  
  
  trialNum = 0

  ## sensory modalities
  vision = 1
  haptic = 1
  
  ## starting lambdas based on sensory modality for day 1
  if haptic == 1 and vision == 1:
    startLambda = 1.1
  else:
    startLambda = 1.50
  
  taskVars = {"lambda": startLambda, "success": -1, "lambdaTrials": 0,\
              "vision": vision, "haptic": haptic,\
              "correctTrials": 0, "running":False, "trialNum": trialNum,\
              "logFile": fmetricsWriter, "logFilePtr": fmetrics, "fcursor": fcursor, "msgFile": fmsg,\
              "GUITree": ["lambda", "success", "correctTrials", "lambdaTrials"]}
  
  center = md.M_GRAPHICS_SHAPE_BOX()
  center.header.msg_type = c_int(md.GRAPHICS_SHAPE_BOX)
  name = create_string_buffer(bytes("center", 'utf-8'), md.MAX_STRING_LENGTH)
  namePtr = (c_char_p) (addressof(name))
  center.objectName = namePtr.value 
  center.sizeX = c_double(0)
  center.sizeY = c_double(50)
  center.sizeZ = c_double(50)
  center.localPosition = (c_double*3) (-200.0, 0.0, 0.0)
  center.color = (c_float*4) (0.83, 0.83, 0.83, 1.0)
  packet = MR.makeMessage(center)
  MR.sendMessage(packet)
  enableGraphics("center", 0)

  cst = md.M_CST_CREATE()
  cst.header.msg_type = md.CST_CREATE
  name = create_string_buffer(b"cst", md.MAX_STRING_LENGTH)
  namePtr = (c_char_p) (addressof(name))
  cst.cstName = namePtr.value
  cst.lambdaVal = c_double(startLambda)
  cst.forceMagnitude = c_double(1)
  cst.visionEnabled = c_int(vision)
  cst.hapticEnabled = c_int(haptic)
  packet = MR.makeMessage(cst)
  MR.sendMessage(packet)
  
  return taskVars

def startCST():
  cstStart = md.M_CST_START()
  cstStart.header.msg_type = md.CST_START 
  name = create_string_buffer(b"cst", md.MAX_STRING_LENGTH)
  namePtr = (c_char_p) (addressof(name))
  cstStart.cstName = namePtr.value
  packet = MR.makeMessage(cstStart)
  MR.sendMessage(packet)

def stopCST():
  cstStop = md.M_CST_STOP()
  cstStop.header.msg_type = md.CST_STOP 
  name = create_string_buffer(b"cst", md.MAX_STRING_LENGTH)
  namePtr = (c_char_p) (addressof(name))
  cstStop.cstName = namePtr.value
  packet = MR.makeMessage(cstStop)
  MR.sendMessage(packet)

def setLambda(lambdaVal):
  cst = md.M_CST_SET_LAMBDA()
  cst.header.msg_type = md.CST_SET_LAMBDA 
  name = create_string_buffer(b"cst", md.MAX_STRING_LENGTH)
  namePtr = (c_char_p) (addressof(name))
  cst.cstName = namePtr.value
  cst.lambdaVal = lambdaVal
  packet = MR.makeMessage(cst)
  MR.sendMessage(packet)

def changeCenterColor(r, g, b, a):
  centerColor = md.M_GRAPHICS_CHANGE_OBJECT_COLOR()
  centerColor.header.msg_type = md.GRAPHICS_CHANGE_OBJECT_COLOR
  name = create_string_buffer(b"center", md.MAX_STRING_LENGTH)
  namePtr = (c_char_p)(addressof(name))
  centerColor.objectName = namePtr.value 
  centerColor.color = (c_float * 4) (r, g, b, a)
  packet = MR.makeMessage(centerColor)
  MR.sendMessage(packet)

def readMessage(message, options, taskVars):
  header = md.MSG_HEADER()
  MR.readMessage(message, header)
  if header.msg_type == md.CST_DATA:
    cstData = md.M_CST_DATA()
    MR.readMessage(message, cstData)
    if np.abs(cstData.cursorY) > 120 and taskVars["running"] == True:
      taskVars["success"] = 0

def startEntry(options, taskVars):
  #trialStart = md.M_TRIAL_START()
  #trialStart.header.msg_type = c_int(md.TRIAL_START)
  #packet = MR.makeMessage(trialStart)
  #MR.sendMessage(packet)
  enableGraphics("center", 1)
  
  if taskVars["trialNum"] >= 10 and taskVars["vision"] == 1:
    visionOff = md.M_CST_SET_VISUAL()
    visionOff.header.msg_type = md.CST_SET_VISUAL
    name = create_string_buffer(b"cst", md.MAX_STRING_LENGTH)
    namePtr = (c_char_p) (addressof(name))
    visionOff.cstName = namePtr.value 
    visionOff.visionEnabled = c_int(0)
    packet = MR.makeMessage(visionOff)
    MR.sendMessage(packet)
    taskVars["vision"] = 0
    taskVars["lambda"] = 1.01
    taskVars["lambdaTrials"] = 0 
    taskVars["correctTrials"] = 0
    setLambda(taskVars["lambda"])
  
  time.sleep(1.5)
  changeCenterColor(0.83, 0.83, 0.83, 1.0)

  #taskVars["fmsg"].flush()
  #taskVars["fmsg"].write(packet)
  taskVars["trialNum"] = taskVars["trialNum"] + 1
  taskVars["success"] = -1
 
  if taskVars["lambdaTrials"] == 0:
    percCorrect = 0.0
  else:
    percCorrect = float(taskVars["correctTrials"]/taskVars["lambdaTrials"])
  
  if percCorrect >= 0.85 and taskVars["lambdaTrials"] > 5:
    taskVars["lambda"] = taskVars["lambda"] + 0.01
    taskVars["lambdaTrials"] = 0 
    taskVars["correctTrials"] = 0
    setLambda(taskVars["lambda"])
  

  reachTime = time.time()
  while (time.time() - reachTime) <= 1.0:
    if np.abs(Globals.CHAI_DATA.posY) < 50 and np.abs(Globals.CHAI_DATA.posZ) < 50: 
      startTime = time.time()
      while np.abs(Globals.CHAI_DATA.posY) < 50 and np.abs(Globals.CHAI_DATA.posZ) < 50 and time.time()-startTime < 1.0:
        time.sleep(0.001)
      return "succeed"
    time.sleep(0.001)
  changeCenterColor(1.0, 0.0, 0.0, 1.0)
  return "fail"

def runningEntry(options, taskVars):
  startCST()  
  taskVars["lambdaTrials"] = taskVars["lambdaTrials"] + 1
  taskVars["running"] = True
  startTime = time.time()
  while (time.time() - startTime) <= 6.0:
    if taskVars["success"] == 0:
      break
    time.sleep(0.001)
  
  stopCST()
  taskVars["running"] = False 

  if taskVars["success"] == 0:
    changeCenterColor(1.0, 0.0, 0.0, 1.0)
    taskVars["logFile"].writerow([taskVars["trialNum"], taskVars["vision"], taskVars["haptic"],\
                                  taskVars["lambda"], 0])
    print("Failure")
  else:
    taskVars["success"] = 1
    taskVars["running"] = False
    taskVars["correctTrials"] = taskVars["correctTrials"] + 1
    taskVars["logFile"].writerow([taskVars["trialNum"], taskVars["vision"], taskVars["haptic"],\
                                  taskVars["lambda"], 1])
    print("Success")
    
  taskControlPtr = taskVars["taskControl"]
  for k in taskVars.keys():
    if k != "taskControl":
      taskControlPtr.sessionInfo[k] = taskVars[k]
  taskControlPtr.addNode()
  
  return "next"
