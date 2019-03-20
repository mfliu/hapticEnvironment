import random 
from ctypes import *
import struct 
import Globals 
import time 

def collidingWith(objectName):
  for i in range(0, len(Globals.CHAI_DATA.collisions)):
    collision = c_char_p(addressof(Globals.CHAI_DATA.collisions[i]))
    collisionName = struct.unpack(str(len(collision.value)) + 's', collision.value)
    collisionName = str(collisionName).split("\'")[1]
    if collisionName == objectName:
      return True 
  return False

def centerEntry(options):
  startTime = time.time()
  while collidingWith("center") == False:
    timeElapsed = time.time() - startTime
    if timeElapsed > 5 and collidingWith("center") == False:
      return "failure"
  holdStart = time.time()
  while time.time() - holdStart < 1:
    if not collidingWith("center"):
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
    if collidingWith("rightTrial"):
      return "success"
  return "failure"

def leftTrialEntry(options):
  startTime = time.time()
  while time.time() - startTime < 5:
    if collidingWith("leftTrial"):
      return "success"
  return "failure"

def upTrialEntry(options):
  startTime = time.time()
  while time.time() - startTime < 5:
    if collidingWith("upTrial"):
      return "success"
  return "failure"

def downTrialEntry(options):
  startTime = time.time()
  while time.time() - startTime < 5:
    if collidingWith("downTrial"):
      return "success"
  return "failure"

def rightPunishmentEntry(options):
  startTime = time.time()
  while time.time() - startTime < 5:
    if collidingWith("rightPunishment"):
      return "success"
  return "failure"

def leftPunishmentEntry(options):
  startTime = time.time()
  while time.time() - startTime < 5:
    if collidingWith("leftPunishment"):
      return "success"
  return "failure"

