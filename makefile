COMPILER=g++
SRC=$(wildcard *.cpp)
INCLUDE=$(wildcard *.h)
LINK=$(shell pkg-config --cflags glfw3) -lGLEW $(shell pkg-config --static --libs glfw3)
COMP_SETTINGS=-std=c++11

all:
	$(COMPILER) -g -o MATHVRT $(SRC) $(INCLUDE) $(LINK) $(COMP_SETTINGS) -I../ovr_sdk_linux_0.4.4/LibOVR/Src -I../ovr_sdk_linux_0.4.4/LibOVR/Src/Kernel -L../ovr_sdk_linux_0.4.4/LibOVR/Lib/Linux/Release/x86_64 -lovr