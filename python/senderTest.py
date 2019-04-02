import socket
import time
import struct
import messageDefinitions as md
import Messenger as MR
from ctypes import * 

UDP_IP = "127.0.0.1"
UDP_PORT = 2000

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

#bgColor = md.M_GRAPHICS_CHANGE_BG_COLOR()
#bgColor.header.serialNo = c_int(1)
#bgColor.header.msg_type = c_int(md.GRAPHICS_CHANGE_BG_COLOR)
#bgColor.header.timestamp = c_double(0.01)
#bgColor.color = (c_float * 4) (255, 0, 0, 1)
#packet = MR.makeMessage(bgColor)
#sock.sendto(packet, (UDP_IP, UDP_PORT))

vField = md.M_HAPTICS_VISCOSITY_FIELD()
vField.header.serialNo = c_int(1)
vField.header.msg_type = c_int(md.HAPTICS_VISCOSITY_FIELD)
vField.header.timestamp = c_double(0.01)
effectName = create_string_buffer(b"viscousField", md.MAX_STRING_LENGTH)
effectNamePtr = (c_char_p) (addressof(effectName))
vField.effectName = effectNamePtr.value 
visc = -0.2
vField.viscosityMatrix = (c_double * 9) (visc, 0.0, 0.0, 0.0, visc, 0.0, 0.0, 0.0, visc)
packet = MR.makeMessage(vField)
sock.sendto(packet, (UDP_IP, UDP_PORT))

time.sleep(10)

rmField = md.M_HAPTICS_REMOVE_FIELD_EFFECT()
rmField.header.serialNo = c_int(1)
rmField.header.msg_type = c_int(md.HAPTICS_REMOVE_FIELD_EFFECT)
rmField.header.timestamp = c_double(0.01)
rmField.effectName = effectNamePtr.value
packet = MR.makeMessage(rmField)
sock.sendto(packet, (UDP_IP, UDP_PORT)) 

#message = md.M_HAPTICS_BOUNDING_PLANE()
#message.header.serialNo = c_int(1)
#message.header.msg_type = c_int(md.HAPTICS_BOUNDING_PLANE)
#message.header.timestamp = c_double(0.01) 
#message.bWidth = c_double(3.0)
#message.bHeight = c_double(3.0)
#packet = MR.makeMessage(message)
#sock.sendto(packet, (UDP_IP, UDP_PORT))

#dots = md.M_GRAPHICS_MOVING_DOTS()
#dots.header.serialNo = c_int(1)
#dots.header.msg_type = c_int(md.GRAPHICS_MOVING_DOTS)
#dots.header.timestamp = c_double(0.01)
#dots.numDots = c_int(500)
#dots.coherence = c_double(0.75)
#dots.direction = c_double(250)
#dots.magnitude = c_double(50)
#packet = MR.makeMessage(dots)
#sock.sendto(packet, (UDP_IP, UDP_PORT))

#forcefield = md.M_HAPTICS_CONSTANT_FORCE_FIELD()
#forcefield.header.serialNo = c_int(1)
#forcefield.header.msg_type = c_int(md.HAPTICS_CONSTANT_FORCE_FIELD)
#forcefield.header.timestamp = c_double(0.01)
#forcefield.direction = c_double(250.0)
#forcefield.magnitude = c_double(5.0)
#packet = MR.makeMessage(forcefield)
#sock.sendto(packet, (UDP_IP, UDP_PORT))
"""
sphereObj = md.M_GRAPHICS_SHAPE_SPHERE();
sphereObj.header.serialNo = c_int(1);
sphereObj.header.msg_type = c_int(md.GRAPHICS_SHAPE_SPHERE);
sphereObj.header.timestamp = c_double(0.01);

objectName = create_string_buffer(b"SphereObject", 128)
objectNamePtr = (c_char_p) (addressof(objectName))

sphereObj.objectName = objectNamePtr.value;
sphereObj.radius = c_double(0.1);
sphereObj.localPosition = (c_double * 3) (0.0, 0.0, 0.0) 
sphereObj.color = (c_float * 4) (255, 0, 0, 1)

packet = MR.makeMessage(sphereObj)
sock.sendto(packet, (UDP_IP, UDP_PORT))


sphereObj = md.M_GRAPHICS_SHAPE_SPHERE();
sphereObj.header.serialNo = c_int(1);
sphereObj.header.msg_type = c_int(md.GRAPHICS_SHAPE_SPHERE);
sphereObj.header.timestamp = c_double(0.01);

objectName = create_string_buffer(b"SphereObjectGreen", 128)
objectNamePtr = (c_char_p) (addressof(objectName))

sphereObj.objectName = objectNamePtr.value;
sphereObj.radius = c_double(0.2);
sphereObj.localPosition = (c_double * 3) (0.0, 0.0, 0.0) 
sphereObj.color = (c_float * 4) (0, 255, 0, 1)

packet = MR.makeMessage(sphereObj)
sock.sendto(packet, (UDP_IP, UDP_PORT))
"""

"""
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
"""
