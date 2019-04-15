## State machine for task control
import importlib.util
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
import os 

class State(object):
  def __init__(self, name, transitions, options, entry, graphics):
    self.name = name
    self.transitions = transitions 
    self.options = options
    self.entry = entry
  
class StateMachine(object):
  def __init__(self, configFile, saveFilePrefix):
    config = json.load(open(configFile))
    self.build(config, saveFilePrefix)
    
  # Build should return the start state of the StateMachine
  def build(self, config, saveFilePrefix):
    self.name = config["name"]
    self.states = {}
    self.startState = config["startState"]
    self.currentState = self.startState
    self.transitionSymbols = config["transitionSymbols"]
    self.transitionTable = {}
    self.stateTree = {}
    self.entryFile = os.path.join(Globals.FUNCTIONS_PATH, config["entryFile"] + ".py")
    moduleSpec = importlib.util.spec_from_file_location("entryModule", self.entryFile)
    self.entryModule = importlib.util.module_from_spec(moduleSpec) 
    moduleSpec.loader.exec_module(self.entryModule)

    self.setupFunc = getattr(self.entryModule, "setup")
    self.taskVars = self.setupFunc(saveFilePrefix)
    self.running = False
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

  def run(self):
    self.running = True
    while self.currentState != "end" and self.running == True:
      currentState = self.states[self.currentState]
      transition = currentState.entry(self.stateTree[self.currentState], self.taskVars)
      nextState = self.transitionTable[(self.currentState, transition)]
      if nextState == "end":
        return transition
      self.currentState = nextState
    return "done"
 
if __name__ == "__main__":
  import sys
  taskConfig = sys.argv[1]
  taskSM = StateMachine(taskConfig, "")
  taskSM.run()

