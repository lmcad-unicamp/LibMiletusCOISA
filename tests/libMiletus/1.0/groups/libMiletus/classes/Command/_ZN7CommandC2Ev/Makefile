INSTALL_PREFIX ?= /home/flavia/Arduino/libraries/libMiletusCoisa

CXX      = g++
CXXFLAGS = -Wall -DBUFFERSIZE=16777216
INCLUDES = -I/home/flavia/Arduino/libraries -I"$(INSTALL_PREFIX)"/linux/third-party/ArduinoJson
LIBS     = "$(INSTALL_PREFIX)"/linux/bin/sharedlibrary.so

all: test

test: test.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) test.cpp -o test $(LIBS)

clean:
	rm -f test test.o
