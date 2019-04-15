#  Software License Agreement (BSD License)
#  Copyright (c) 2003-2016, CHAI3D.
#  (www.chai3d.org)
#
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions
#  are met:
#
#  * Redistributions of source code must retain the above copyright
#  notice, this list of conditions and the following disclaimer.
#
#  * Redistributions in binary form must reproduce the above
#  copyright notice, this list of conditions and the following
#  disclaimer in the documentation and/or other materials provided
#  with the distribution.
#
#  * Neither the name of CHAI3D nor the names of its contributors may
#  be used to endorse or promote products derived from this software
#  without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
#  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
#  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
#  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
#  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
#  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
#  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
#  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
#  POSSIBILITY OF SUCH DAMAGE.
#
#  $Author: seb $
#  $Date: 2016-01-21 16:13:27 +0100 (Thu, 21 Jan 2016) $
#  $Rev: 1906 $

CXX = clang++
# project layout
TOP_DIR = ./external/chai3d-3.2.0
BASE_DIR = ./bin/$(OS)-$(ARCH)-$(COMPILER)
include $(TOP_DIR)/Makefile.common
RPCLIB_DIR = ./external/rpclib

# GLFW dependency
CXXFLAGS += -I$(GLFW_DIR)/include -I./common -I$(RPCLIB_DIR)/include
LDFLAGS  += -L$(GLFW_DIR)/lib/$(CFG)/$(OS)-$(ARCH)-$(COMPILER) -L$(RPCLIB_DIR)/build
LDLIBS   += $(LDLIBS_GLFW) -lrpc  

# platform-dependent adjustments
ifeq ($(OS), mac)
  DEPLOY = Rez -append $(CHAI_DIR)/bin/resources/icons/chai3d.rsrc -o $(OUTPUT); SetFile -a C $(OUTPUT)
endif

# local configuration
SRC_DIR   = ./src
HDR_DIR   = ./src
OBJ_DIR   = ./obj/$(CFG)/$(OS)-$(ARCH)-$(COMPILER)
PROG      = $(notdir $(shell pwd)) 
SOURCES   = $(wildcard $(SRC_DIR)/*/*.cpp)
INCLUDES  = $(wildcard $(HDR_DIR)/*/*.h)
OBJECTS   = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(notdir $(SOURCES)))
OUTPUT    = $(BASE_DIR)/$(PROG)

# Message handling configuration 
MSG_DIR = ./messaging/MessageHandler
MSG_HDR = ./messaging/MessageHandler
MSG_OBJ = ./obj/$(CFG)/$(OS)-$(ARCH)-$(COMPILER)
MSG_PROG = messageHandler
MSG_SOURCES = $(wildcard $(MSG_DIR)/*.cpp)
MSG_INCLUDES = $(wildcard $(MSG_DIR)/*.h)
MSG_OBJECTS = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(notdir $(MSG_SOURCES)))
MSG_OUTPUT = $(BASE_DIR)/$(MSG_PROG)
MSG_FLAGS = -DLINUX -Wno-deprecated -std=c++17 -I$(RPCLIB_DIR)/include/  
MSG_LDFLAGS = -L$(RPCLIB_DIR)/build -lrpc -lpthread

# Logging configuration 
#LOG_DIR = ./messaging/Logger
#LOG_HDR = ./messaging/Logger
#LOG_OBJ = ./obj/$(CFG)/$(OS)-$(ARCH)-$(COMPILER)
#LOG_PROG = logger
#LOG_SOURCES = $(wildcard $(LOG_DIR)/*.cpp)
#LOG_INCLUDES = $(wildcard $(LOG_DIR)/*.h)
#LOG_OBJECTS = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(notdir $(LOG_SOURCES)))
#LOG_OUTPUT = $(BASE_DIR)/$(LOG_PROG)
#LOG_FLAGS = -DLINUX -Wno-deprecated -std=c++17 -I./common  
#LOG_LDFLAGS = -lpthread

all: $(OUTPUT) $(MSG_OUTPUT) #$(LOG_OUTPUT)

D_FILES = $(OBJECTS:.o=.d)
-include $(D_FILES)

#########################################################
$(OBJECTS): $(INCLUDES) 

$(OUTPUT): $(OBJ_DIR) $(BASE_DIR) $(LIB_TARGET) $(OBJECTS)
	$(CXX) $(CXXFLAGS) -I$(HDR_DIR) $(OBJECTS) $(LDFLAGS) $(LDLIBS) -o $(OUTPUT)
	$(DEPLOY)

$(OBJ_DIR):
	mkdir -p $@

$(BASE_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o : $(SRC_DIR)/*/%.cpp | $(OBJ_DIR) 
	$(CXX) $(CXXFLAGS) -I$(HDR_DIR) -MD -MF $(OBJ_DIR)/$.d -c -o $@ $<
#########################################################
#########################################################
$(MSG_OBJECTS): $(MSG_INCLUDES)

$(MSG_OUTPUT): $(MSG_OBJ) $(BASE_DIR) $(MSG_OBJECTS)
	$(CXX) $(MSG_FLAGS) -I$(MSG_HDR) $(MSG_OBJECTS) $(MSG_LDFLAGS) -o $(MSG_OUTPUT)

$(MSG_OBJ)/%.o: $(MSG_DIR)/%.cpp | $(MSG_OBJ)
	$(CXX) $(MSG_FLAGS) -I$(MSG_HDR) -MD -MF $(MSG_OBJ)/$.d -c -o $@ $<
#########################################################
#########################################################
#$(LOG_OBJECTS): $(LOG_INCLUDES)

#$(LOG_OUTPUT): $(LOG_OBJ) $(BASE_DIR) $(LOG_OBJECTS)
#	$(CXX) $(LOG_FLAGS) -I$(LOG_HDR) $(LOG_OBJECTS) $(LOG_LDFLAGS) -o $(LOG_OUTPUT)

#$(LOG_OBJ)/%.o: $(LOG_DIR)/%.cpp | $(LOG_OBJ)
#	$(CXX) $(LOG_FLAGS) -I$(LOG_HDR) -MD -MF $(LOG_OBJ)/$.d -c -o $@ $<
#########################################################

clean:
	rm -f $(OUTPUT) $(OBJECTS) *~
	rm -f $(MSG_OUTPUT) $(MSG_OBJECTS) *~
	#rm -f $(LOG_OUTPUT) $(LOG_OBJECTS) *~
	rm -rf $(OBJ_DIR)
	rm -rf $(MSG_OBJ)
	#rm -rf $(LOG_OBJ)
	rm -rf $(BASE_DIR)

	
