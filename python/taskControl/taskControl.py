import sys
sys.path.append("/home/mfl24/Documents/chaiProjects/hapticControl/python/")
from statemachine import *
import json
import messageDefinitions as md
import Messenger as MR
import Globals
import ctypes
from threading import Thread 
import os 
import kivy
kivy.require("1.9.1")
from kivy import Config 
Config.set('graphics', 'multisamples', '0')
Config.set('graphics', 'width', 900)
Config.set('graphics', 'height', 700)
Config.set('graphics', 'resizable', False)
from kivy.app import App
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.button import Button 
from kivy.uix.treeview import TreeView, TreeViewLabel
from kivy.uix.popup import Popup
from kivy.uix.filechooser import FileChooserIconView
from kivy.uix.label import Label
from functools import partial
from kivy.properties import BooleanProperty

class TaskControl(BoxLayout):
  def __init__(self, **kwargs):
    super(TaskControl, self).__init__(**kwargs)
    self.sessionInfo = {}
    self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)

  def setSubjectName(self, text):
    self.sessionInfo["subjectName"] = text 
  
  def initializeTreeView(self):
    self.tv = self.get_root_window().children[-1].ids["experimentRecords"]
    self.tv.bind(minimum_height=self.tv.setter('height'))
    self.tvRoot = self.tv.add_node(TreeViewLabel(text="Session Log"))

  def addNode(self):
    trialNum = self.sessionInfo["trialNum"]
    trialNode = self.tv.add_node(TreeViewLabel(text="Trial " + str(trialNum)), self.tvRoot)
    for k in self.sessionInfo.keys():
      if k!= "trialNum":
        infoNode = self.tv.add_node(TreeViewLabel(text=k + ": " + str(self.sessionInfo[k])), trialNode)

  def startSM(self):
    self.initializeTreeView()
    needList = ["configFile", "saveDir", "subjectName", "taskName"]
    for n in needList:
      if n not in self.sessionInfo.keys():
        print("Need " + n)
        break
    
    sessionStart = md.M_SESSION_START()
    sessionStart.header.msg_type = c_int(md.SESSION_START)
    packet = MR.makeMessage(sessionStart)
    self.sock.sendto(packet, (Globals.SENDER_IP, Globals.SENDER_PORT))
    saveFilePrefix = os.path.join(self.sessionInfo["saveDir"], self.sessionInfo["subjectName"] + "-" +\
                                  time.ctime(time.time()).replace(" ", "_").replace(":", "-"))
    
    startRecording= md.M_START_RECORDING()
    startRecording.header.msg_type = c_int(md.START_RECORDING)
    fileName = create_string_buffer(str.encode(saveFilePrefix+"_trial.data"), md.MAX_STRING_LENGTH)
    fileNamePtr = (c_char_p) (addressof(fileName))
    startRecording.filename = fileNamePtr.value
    packet = MR.makeMessage(startRecording)
    self.sock.sendto(packet, (Globals.SENDER_IP, Globals.SENDER_PORT))

    taskSM = StateMachine(self.sessionInfo["configFile"], saveFilePrefix, True)
    self.sm = taskSM
    self.sm.taskVars["taskControl"] = self
    self.smThread = Thread(target=self.sm.run)
    self.smThread.daemon = True
    self.smThread.start()
    
    #self.trialThread = Thread(target=self.trialInfoUpdate)
    #self.trialThread.daemon = True
    #self.trialThread.start()

  #def trialInfoUpdate(self):
  #  trialNum = 0 #self.sm.taskVars["trialNum"]
  #  while True:
  #    if self.sm.taskVars["trialNum"] == trialNum:
  #      continue
  #    else:
  #      trialNum = self.sm.taskVars["trialNum"]
  #      for k in self.sm.taskVars.keys():
  #        self.sessionInfo[k] = self.sm.taskVars[k]
  #      self.addNode()

  def stopSM(self):
    self.sm.currentState = "end"
    
    stopRecording = md.M_STOP_RECORDING()
    stopRecording.header.msg_type = c_int(md.STOP_RECORDING)
    packet = MR.makeMessage(stopRecording)
    self.sock.sendto(packet, (Globals.SENDER_IP, Globals.SENDER_PORT))
    
    sessionStop = md.M_SESSION_END()
    sessionStop.header.msg_type = c_int(md.SESSION_END)
    packet = MR.makeMessage(sessionStop)
    self.sock.sendto(packet, (Globals.SENDER_IP, Globals.SENDER_PORT))

  def chooseSaveDir(self):
    filePopup = FilePopup(titleText="Choose Save Directory", canBeDir=True, buttonText="Select Folder")
    filePopup.open()
  
  def chooseConfigFile(self):
    filePopup = FilePopup(titleText="Choose Configuration File", canBeDir=False, buttonText="Select Configuration File")
    filePopup.open()

class FilePopup(Popup):
  def __init__(self, **kwargs):
    super(FilePopup, self).__init__()
    titleText = kwargs['titleText']
    dirTrue = kwargs['canBeDir']
    buttonText = kwargs['buttonText']
    if buttonText == "Select Folder":
      self.choice = "saveDir"
    elif buttonText == "Select Configuration File":
      self.choice = "configFile"
    content = BoxLayout(orientation='vertical')
    chooseButton = Button(text=buttonText, size_hint=(1, 0.2))
    fileChooser = FileChooserIconView()
    fileChooser.dirselect = BooleanProperty(dirTrue)
    fileChooser.bind(selection=self.selectingFolder)
    chooseButton.bind(on_release=self.chooseFile)
    content.add_widget(fileChooser)
    content.add_widget(chooseButton)
    self.title = titleText
    self.canvas.opacity = 0.8
    self.background_color = 1,1,1,1
    self.content = content
    self.size_hint = (0.8, 0.8)
     
  def selectingFolder(self, path, selection):
    self.selectedFolder = selection[0]
  
  def chooseFile(self, obj):
    self.get_root_window().children[-1].sessionInfo[self.choice] = self.selectedFolder
    if self.choice == "configFile":
      self.get_root_window().children[-1].ids["configFileText"].text = self.selectedFolder
      configData = json.load(open(self.selectedFolder))
      self.get_root_window().children[-1].ids["taskName"].text = configData["name"] 
      self.get_root_window().children[-1].sessionInfo["taskName"] = configData["name"]
    elif self.choice == "saveDir":
      self.get_root_window().children[-1].ids["saveDirText"].text = self.selectedFolder
    self.dismiss()

class TaskControlApp(App):
  def __init__(self, **kwargs):
    super(TaskControlApp, self).__init__(**kwargs)

  def build(self,):
    return TaskControl()


if __name__ == "__main__":
  TaskControlApp().run()
