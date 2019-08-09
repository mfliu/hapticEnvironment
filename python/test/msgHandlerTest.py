import msgpackrpc
RPC_IP = "127.0.0.1"
RPC_PORT = 8080

client = msgpackrpc.Client(msgpackrpc.Address(RPC_IP, RPC_PORT))

what = client.call_async("addModule", 1, "127.0.0.1", 8080)
print(what.get())
