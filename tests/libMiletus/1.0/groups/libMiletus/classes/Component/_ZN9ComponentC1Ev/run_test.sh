#!/bin/sh
INSTALL_PREFIX=${INSTALL_PREFIX:-/home/flavia/Arduino/libraries/libMiletusCoisa}

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:"$INSTALL_PREFIX/linux/bin" && ./test arg1 arg2 arg3 >output 2>&1
