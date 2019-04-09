## State machine for task control
import warnings  
import socket
import time
import messageDefinitions as md
import Messenger as MR
from ctypes import *
from threading import Thread
import struct 
import json
import Globals

senderSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

class State(object):
  def __init__(self, name, transitions, options, entry, graphics):
    self.name = name
    self.transitions = transitions 
    self.options = options
    self.entry = entry
    self.graphics = graphics 
    if len(graphics.keys()) > 0:
      self.makeStateGraphics(self.graphics)
  
  #def makeStateGraphics(self, graphics):
  #  shape = graphics["shape"] 
  #  if shape == "sphere":
  #    radius = graphics["radius"]
  #    color = graphics["color"]
  #    position = graphics["position"]
  #    stateShape = md.M_GRAPHICS_SHAPE_SPHERE()
  #    stateShape.header.serial_no = c_int(1) # bullshit number for testing 
  #    stateShape.header.msg_type = c_int(md.GRAPHICS_SHAPE_SPHERE)
  #    stateShape.header.timestamp = c_double(0.01) # bullshit number for testing 
  #    self.objectName = create_string_buffer(bytes(graphics["name"], 'utf-8'), 128)
  #    objectNamePtr = (c_char_p) (addressof(self.objectName))
  #    stateShape.objectName = objectNamePtr.value
  #    stateShape.radius = c_double(radius) 
  #    stateShape.localPosition = (c_double * 3) (position[0], position[1], position[2])
  #    stateShape.color = (c_float * 4) (color[0], color[1], color[2], color[3])
  #    stateShapePacket = MR.makeMessage(stateShape)
  #    senderSocket.sendto(stateShapePacket, (Globals.SENDER_IP, Globals.SENDER_PORT))
  #    self.setTargetGraphics(self.objectName, 0)
    
  #def setTargetGraphics(self, objectName, setVal):
  #  namePtr = (c_char_p) (addressof(objectName))
  #  message = md.M_GRAPHICS_SET_ENABLED()
  #  message.header.serialNo = c_int(1)
  #  message.header.msg_type = c_int(md.GRAPHICS_SET_ENABLED)
  #  message.header.timestamp = c_double(0.01) 
  #  message.objectName = namePtr.value 
  #  message.enabled = c_int(setVal)
  #  packet = MR.makeMessage(message)
  #  senderSocket.sendto(packet, (Globals.SENDER_IP, Globals.SENDER_PORT))

  #def setTargetHaptics(self, objectName, setVal):
  #  namePtr = (c_char_p) (addressof(objectName))
  #  message = md.M_HAPTICS_SET_ENABLED()
  #  message.header.serialNo = c_int(1)
  #  message.header.msg_type = c_int(md.HAPTICS_SET_ENABLED)
  #  message.header.timestamp = c_double(0.01) 
  #  message.objectName = namePtr.value 
  #  message.enabled = c_int(setVal)
  #  packet = MR.makeMessage(message)
  #  senderSocket.sendto(packet, (Globals.SENDER_IP, Globals.SENDER_PORT))

class StateMachine(object):
  def __init__(self, configFile, saveFilePrefix, top):
    config = json.load(open(configFile))
    self.build(config, saveFilePrefix)
    
    if top == True:
      #self.setBoundingPlane()
      self.listenerSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
      self.listenerSocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
      self.listenerSocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
      self.listenerSocket.bind((Globals.DATA_LISTENER_IP, Globals.DATA_LISTENER_PORT))
      self.listenerThread = Thread(target=self.listener)
      self.listenerThread.daemon = True
      self.listenerThread.start()

  #def setBoundingPlane(self):
  #  message = md.M_HAPTICS_BOUNDING_PLANE()
  #  message.header.serialNo = c_int(1)
  #  message.header.msg_type = c_int(md.HAPTICS_BOUNDING_PLANE)
  #  message.header.timestamp = c_double(0.01) 
  #  packet = MR.makeMessage(message)
  #  senderSocket.sendto(packet, (Globals.SENDER_IP, Globals.SENDER_PORT))


  # Build should return the start state of the StateMachine
  def build(self, config, saveFilePrefix):
    self.name = config["name"]
    self.states = {}
    self.startState = config["startState"]
    self.currentState = self.startState
    self.transitionSymbols = config["transitionSymbols"]
    self.transitionTable = {}
    self.stateTree = {}
    self.entryFile = config["entryFile"]
    self.entryModule = __import__(self.entryFile, globals(), locals(), [], 0)
    
    self.setupFunc = getattr(self.entryModule, "setup")
    self.taskVars = self.setupFunc(saveFilePrefix)

    for stateName in config["states"]:
      stateConfig = config[stateName]
      stateTransitions = stateConfig["transitions"]
      for t in stateTransitions.keys():
        self.transitionTable[(stateName, t)] = stateTransitions[t]
      stateOptions = stateConfig["options"]
      stateEntryFunc = getattr(self.entryModule, stateConfig["entry"])
      stateGraphics = stateConfig["graphics"]
      state = State(stateName, stateTransitions, stateOptions, stateEntryFunc, stateGraphics)
      self.states[stateName] = state
      self.stateTree[stateName] = []
      if len(stateOptions) > 0:
        for option in stateOptions:
          nestedSM = StateMachine(option + "Config.json", False)
          self.stateTree[stateName].append(nestedSM)

  def listener(self):
    while True:
      data, addr = self.listenerSocket.recvfrom(md.MAX_PACKET_LENGTH)
      msg_data = md.M_HAPTIC_DATA_STREAM()
      MR.readMessage(data, msg_data)
      Globals.CHAI_DATA = msg_data
      time.sleep(0.001)
  
  def run(self):
    #print(self.name)
    while self.currentState != "end":
      currentState = self.states[self.currentState]
      if len(currentState.graphics.keys()) > 0:
        currentState.setTargetGraphics(currentState.objectName, 1)
        currentState.setTargetHaptics(currentState.objectName, 1)

      transition = currentState.entry(self.stateTree[self.currentState], self.taskVars)
      if len(currentState.graphics.keys()) > 0:
        currentState.setTargetGraphics(currentState.objectName, 0)
        currentState.setTargetHaptics(currentState.objectName, 0)

      nextState = self.transitionTable[(self.currentState, transition)]
      if nextState == "end":
        return transition

      self.currentState = nextState
    return "something is wrong"
  
if __name__ == "__main__":
  import sys
  taskConfig = sys.argv[1]
  taskSM = StateMachine(taskConfig, True)
  taskSM.run()

