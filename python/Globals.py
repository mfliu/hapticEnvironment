#from gevent import monkey 
#monkey.patch_all()

import messageDefinitions as md
#from mprpc import RPCClient 

LISTENER_IP = "127.0.0.1"
LISTENER_PORT = 9000

SENDER_IP = "127.0.0.1"
SENDER_PORT = 7000

RPC_IP = "127.0.0.1"
RPC_PORT = 8080

#client = None 
#def getClient():
#  global client 
#  if client == None:
#    client = RPCClient(RPC_IP, RPC_PORT)
#  return client

CHAI_DATA = md.M_HAPTIC_DATA_STREAM() 
