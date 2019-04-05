from mprpc import RPCClient 

client = RPCClient("127.0.0.1", 8080)
for i in range(0, 100):
  print(client.call("getMsgNum"))
  print(client.call("getTimestamp"))
