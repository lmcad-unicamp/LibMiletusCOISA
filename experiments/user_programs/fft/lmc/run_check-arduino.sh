#!/bin/bash

PYTHON_SCRIPT_PATH=../../../linux
INPUT_PATH=../in
OUTPUT_PATH=out

case "$1" in

1)  #./compile.sh 8 0
    rm -f $OUTPUT_PATH/t01.out
    python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/fft_01.x $2 $INPUT_PATH/t01.in $OUTPUT_PATH/t01.out
    ./check.sh 1
    ;;
2)  #./compile.sh 32 0
    rm -f $OUTPUT_PATH/t02.out
    python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/fft_02.x $2 $INPUT_PATH/t02.in $OUTPUT_PATH/t02.out
    ./check.sh 2
    ;;
all) rm -f $OUTPUT_PATH/*.out
     #./compile.sh 8 0
     python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/fft_01.x $2 $INPUT_PATH/t01.in $OUTPUT_PATH/t01.out

     #./compile.sh 32 0
     python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/fft_02.x $2 $INPUT_PATH/t02.in $OUTPUT_PATH/t02.out

     ./check.sh 1

     ./check.sh 2
     ;;
*) echo -e "There is no test '$1'.\n"
   ;;
esac