#!/bin/bash
SRC=./src
COISA=$SRC/COISA
LINUX=$SRC/LINUX
INCLUDE=$SRC/../include
OPKG_CMD=$(which opkg)

if [ ! -d third-party ]; then
    THIRD_PARTY_ALREADY_EXISTS=false
    echo ">> Creating 'third-party' directory"
    mkdir third-party
else
    THIRD_PARTY_ALREADY_EXISTS=true
fi

echo ">> Entering 'third-party' directory"
cd third-party

if [ ! -d ArduinoJson ]; then
    git clone https://github.com/bblanchon/ArduinoJson.git

    echo ">> Entering 'ArduinoJson' directory"
    cd ArduinoJson

    echo ">> Running CMake"
    cmake .
    if [ $? -ne 0 ]; then
        echo ">> CMake error. Reversing changes!"
        if $THIRD_PARTY_ALREADY_EXISTS; then
            rm -rf ../../third-party/ArduinoJson
        else
            rm -rf ../../third-party
        fi
        exit 1
    fi
    make -j 4

    git checkout v5.8.3 # FIXME: it is only compiling after executing this checkout, but why?

    echo ">> Leaving 'ArduinoJson' directory"
    cd ../
else
    echo ">> The 'ArduinoJson' directory already exists. Moving on..."
fi

echo ">> Leaving 'third-party' directory"
cd ../

echo ">> Recreating bin directory"
rm -rf bin
mkdir bin
cd bin

echo ">> Compiling"
g++ -c -I ../include/ -g -Wall -fpic ../$SRC/base64.cpp -std=c++11

# COISA:
g++ -c -I ../include/  -O3 -Wall -fpic ../$COISA/comm.cpp -std=c++11
g++ -c -I ../include/  -O3 -Wall -fpic ../$COISA/EH.cpp -std=c++11
g++ -c -I ../include/  -O3 -Wall -fpic ../$COISA/HAL.cpp -std=c++11
g++ -c -I ../include/  -O3 -Wall -fpic ../$COISA/serial.cpp -std=c++11
g++ -c -I ../include/  -O3 -Wall -fpic ../$COISA/syscall.cpp -std=c++11
g++ -c -I ../include/  -O3 -Wall -fpic ../$COISA/TM.cpp -std=c++11
g++ -c -I ../include/  -O3 -Wall -fpic ../$COISA/vm.cpp -std=c++11

# Optional: Set FIX_IO_CONTENT to 1 to run test cases with the send_test.py script
#           (input and output will be read from files). Default is 0 (libMiletus.h).
if [ $# -eq 0 ]; then
    g++ -c  -O3 -Wall -fpic ../$SRC/libMiletus.cpp       -I ../include/ -std=c++11
    g++ -c  -O3 -Wall -fpic ../$LINUX/linux_wifi.cpp     -I ../include/ -std=c++11
    g++ -c  -O3 -Wall -fpic ../$LINUX/linux_wrapper.cpp  -I ../include/ -std=c++11
    g++ -c  -O3 -Wall -fpic ../$LINUX/linux_provider.cpp -I ../include/ -std=c++11

    g++ -O3 -I../$INCLUDE ../$LINUX/example_libMiletusLinux_wifi.cpp *.o -o linux_example_wifi -std=c++11 -lpapi
else
    g++ -c -O3 -Wall -fpic ../$SRC/libMiletus.cpp       -DFIX_IO_CONTENT=$1 -I ../include/ -std=c++11
    g++ -c -O3 -Wall -fpic ../$LINUX/linux_wifi.cpp     -DFIX_IO_CONTENT=$1 -I ../include/ -std=c++11
    g++ -c -O3 -Wall -fpic ../$LINUX/linux_wrapper.cpp  -DFIX_IO_CONTENT=$1 -I ../include/ -std=c++11
    g++ -c -O3 -Wall -fpic ../$LINUX/linux_provider.cpp -DFIX_IO_CONTENT=$1 -I ../include/ -std=c++11

    g++ -O3 -I../$INCLUDE ../$LINUX/example_libMiletusLinux_wifi.cpp *.o -DFIX_IO_CONTENT=$1 -o linux_example_wifi -std=c++11 -lpapi
fi

if [[ ! -z $OPKG_CMD ]]; then
	g++ -O3 -I../$INCLUDE ../$LINUX/example_libMiletus_edison_wifi.cpp *.o -o edison_example_wifi -std=c++11 -lmraa
fi

echo ">> Done!"
