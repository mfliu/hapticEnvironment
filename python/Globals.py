import messageDefinitions as md
import msgpackrpc
import socket 

LISTENER_IP = "127.0.0.1"
LISTENER_PORT = 9000

MESSAGE_IP = "127.0.0.1"
MESSAGE_PORT = 10000

SENDER_IP = "127.0.0.1"
SENDER_PORT = 7000

RPC_IP = "127.0.0.1"
RPC_PORT = 8080

client = None 
def getClient():
  global client 
  if client == None:
    client = msgpackrpc.Client(msgpackrpc.Address(RPC_IP, RPC_PORT))
  return client

senderSocket = None
def getSenderSocket():
  global senderSocket
  if senderSocket == None:
    senderSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    senderSocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    senderSocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
  return senderSocket

listenerSocket = None 
def getListenerSocket():
  global listenerSocket
  if listenerSocket == None:
    listenerSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    listenerSocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    listenerSocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
    listenerSocket.bind((LISTENER_IP, LISTENER_PORT))
  return listenerSocket 

messageSocket = None
def getMessageSocket():
  global messageSocket
  if messageSocket == None:
    messageSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    messageSocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    messageSocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
    messageSocket.bind((MESSAGE_IP, MESSAGE_PORT))
  return messageSocket 

HOME_PATH = "/home/mfl24/Documents/chaiProjects/hapticControl/python/"
UTILS_PATH = "/home/mfl24/Documents/chaiProjects/hapticControl/python/utils/"
FUNCTIONS_PATH = "/home/mfl24/Documents/chaiProjects/hapticControl/python/trialControl/"

CHAI_DATA = md.M_HAPTIC_DATA_STREAM() 
