INSTALL_PREFIX ?= /home/flavia/Arduino/libraries/libMiletusCoisa

CXX      = g++
CXXFLAGS = -Wall -DBUFFERSIZE=16777216
INCLUDES = -I"$(INSTALL_PREFIX)"/linux/third-party/ArduinoJson/src -I/home/flavia/Arduino/libraries -I"$(INSTALL_PREFIX)"/linux/third-party/ArduinoJson
LIBS     = "$(INSTALL_PREFIX)"/linux/bin/sharedlibrary.so "$(INSTALL_PREFIX)"/linux/bin/sharedlibrary.so /lib/x86_64-linux-gnu/libgcc_s.so.1 /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.19 -L/lib/x86_64-linux-gnu -lm-2.19 -lc-2.19

all: test

test: test.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) test.cpp -o test $(LIBS)

clean:
	rm -f test test.o
