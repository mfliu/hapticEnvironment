import random 
from ctypes import *
import struct 
import Globals 
import time 

def centerEntry(options):
  startTime = time.time()
  collisionData = ""
  while collisionData != "center":
    timeElapsed = time.time() - startTime
    collisions = c_char_p(addressof(Globals.CHAI_DATA.collisions))
    collisionData = struct.unpack(str(len(collisions.value)) + 's', collisions.value)
    collisionData = str(collisionData).split('\'')[1]
    if timeElapsed > 5 and collisionData != "center":
      return "failure"
  holdStart = time.time()
  while time.time() - holdStart < 1:
    collisions = c_char_p(addressof(Globals.CHAI_DATA.collisions))
    collisionData = struct.unpack(str(len(collisions.value)) + 's', collisions.value)
    collisionData = str(collisionData).split('\'')[1]
    if collisionData != "center":
      return "failure"
  return "success"

def trialEntry(options):
  nextState = random.randint(0, len(options)-1)
  stateMachine = options[nextState]
  transition = stateMachine.run()
  return transition

def punishmentEntry(options):
  nextState = random.randint(0, len(options)-1)
  stateMachine = options[nextState]
  transition = stateMachine.run()
  return transition

def rightTrialEntry(options):
  startTime = time.time()
  while time.time() - startTime < 5:
    collisions = c_char_p(addressof(Globals.CHAI_DATA.collisions))
    collisionData = struct.unpack(str(len(collisions.value)) + 's', collisions.value)
    collisionData = str(collisionData).split('\'')[1]
    if collisionData.find("rightTrial") > -1:
      return "success"
  return "failure"

def leftTrialEntry(options):
  startTime = time.time()
  while time.time() - startTime < 5:
    collisions = c_char_p(addressof(Globals.CHAI_DATA.collisions))
    collisionData = struct.unpack(str(len(collisions.value)) + 's', collisions.value)
    collisionData = str(collisionData).split('\'')[1]
    if collisionData.find("leftTrial") > -1:
      return "success"
  return "failure"

def upTrialEntry(options):
  startTime = time.time()
  while time.time() - startTime < 5:
    collisions = c_char_p(addressof(Globals.CHAI_DATA.collisions))
    collisionData = struct.unpack(str(len(collisions.value)) + 's', collisions.value)
    collisionData = str(collisionData).split('\'')[1]
    if collisionData.find("upTrial") > -1:
      return "success"
  return "failure"

def downTrialEntry(options):
  startTime = time.time()
  while time.time() - startTime < 5:
    collisions = c_char_p(addressof(Globals.CHAI_DATA.collisions))
    collisionData = struct.unpack(str(len(collisions.value)) + 's', collisions.value)
    collisionData = str(collisionData).split('\'')[1]
    if collisionData.find("downTrial") > -1:
      return "success"
  return "failure"

def rightPunishmentEntry(options):
  startTime = time.time()
  while time.time() - startTime < 5:
    collisions = c_char_p(addressof(Globals.CHAI_DATA.collisions))
    collisionData = struct.unpack(str(len(collisions.value)) + 's', collisions.value)
    collisionData = str(collisionData).split('\'')[1]
    if collisionData.find("rightPunishment") > -1:
      return "success"
  return "failure"

def leftPunishmentEntry(options):
  startTime = time.time()
  while time.time() - startTime < 5:
    collisions = c_char_p(addressof(Globals.CHAI_DATA.collisions))
    collisionData = struct.unpack(str(len(collisions.value)) + 's', collisions.value)
    collisionData = str(collisionData).split('\'')[1]
    if collisionData.find("leftPunishment") > -1:
      return "success"
  return "failure"

