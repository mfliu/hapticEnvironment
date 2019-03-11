import socket
import time
import struct
import messageDefinitions as md
import Messenger as MR
from ctypes import * 

UDP_IP = "127.0.0.1"
UDP_PORT = 2000

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
#sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
#sock.bind((UDP_IP, UDP_PORT))

sphereObj = md.M_GRAPHICS_SHAPE_SPHERE();
sphereObj.header.serialNo = c_int(1);
sphereObj.header.msg_type = c_int(md.GRAPHICS_SHAPE_SPHERE);
sphereObj.header.timestamp = c_double(0.01);

objectName = create_string_buffer(b"SphereObject", 128)
objectNamePtr = (c_char_p) (addressof(objectName))

sphereObj.objectName = objectNamePtr.value;
sphereObj.radius = c_double(1.0);
sphereObj.localPosition = (c_double * 3) (0.0, 0.0, 0.0) 
sphereObj.color = (c_float * 4) (255, 0, 0, 1)

packet = MR.makeMessage(sphereObj)
sock.sendto(packet, (UDP_IP, UDP_PORT))

time.sleep(1)

sphereStiffness = md.M_HAPTICS_SET_STIFFNESS();
sphereStiffness.header.serialNo = c_int(1);
sphereStiffness.header.msg_type = c_int(md.HAPTICS_SET_STIFFNESS);
sphereStiffness.header.timestamp = c_double(0.01);

objectName = create_string_buffer(b"SphereObject", 128)
objectNamePtr = (c_char_p) (addressof(objectName))

sphereStiffness.objectName = objectNamePtr.value
sphereStiffness.stiffness = (c_double) (5.0)
packet = MR.makeMessage(sphereStiffness)
sock.sendto(packet, (UDP_IP, UDP_PORT))
