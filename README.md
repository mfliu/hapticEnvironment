
Just some notes for now.
Documentation here: https://mfliu.github.io/hapticEnvironment/index.html

If you are cloning this repository and need the `rpclib` library, use 
```
git submodule init
git submodule update`
```
to populate the `external/rpclib` folder
Build rpclib from the `external/rpclib` folder with:
```
mkdir build && cd build && cmake .. && make 
```

RPC Server is built into MessageHandler, both the C++ module and the Python module runs a RPC
client. Same `rpclib` for the server and client in C++, [`msgpack-rpc`](https://github.com/msgpack-rpc/msgpack-rpc-python) for Python client 

Heavily based on and inspired by: [this project](https://github.com/djoshea/haptic-control)
