#!/bin/bash

PYTHON_SCRIPT_PATH=../../../linux
MEASURING_INPUT_PATH=../../user_input
MEASURING_OUTPUT_PATH=../../user_output/fft

case "$1" in

small04) #./compile.sh 4 1
         rm -f $MEASURING_OUTPUT_PATH/small04.out
         python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/fft_small04.x $2 $MEASURING_INPUT_PATH/small.in $MEASURING_OUTPUT_PATH/small04.out
         ;;
small08) #./compile.sh 8 1
         rm -f $MEASURING_OUTPUT_PATH/small08.out
         python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/fft_small08.x $2 $MEASURING_INPUT_PATH/small.in $MEASURING_OUTPUT_PATH/small08.out
         ;;
medium04) #./compile.sh 4 1
          rm -f $MEASURING_OUTPUT_PATH/medium04.out
          python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/fft_medium04.x $2 $MEASURING_INPUT_PATH/medium.in $MEASURING_OUTPUT_PATH/medium04.out
          ;;
medium08) #./compile.sh 8 1
          rm -f $MEASURING_OUTPUT_PATH/medium08.out
          python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/fft_medium08.x $2 $MEASURING_INPUT_PATH/medium.in $MEASURING_OUTPUT_PATH/medium08.out
          ;;
medium16) #./compile.sh 16 1
          rm -f $MEASURING_OUTPUT_PATH/medium16.out
          python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/fft_medium16.x $2 $MEASURING_INPUT_PATH/medium.in $MEASURING_OUTPUT_PATH/medium16.out
          ;;
large04) #./compile.sh 4 1
         rm -f $MEASURING_OUTPUT_PATH/large04.out
         python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/fft_large04.x $2 $MEASURING_INPUT_PATH/large.in $MEASURING_OUTPUT_PATH/large04.out
         ;;
large08) #./compile.sh 8 1
         rm -f $MEASURING_OUTPUT_PATH/large08.out
         python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/fft_large08.x $2 $MEASURING_INPUT_PATH/large.in $MEASURING_OUTPUT_PATH/large08.out
         ;;
large16) #./compile.sh 16 1
         rm -f $MEASURING_OUTPUT_PATH/large16.out
         python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/fft_large16.x $2 $MEASURING_INPUT_PATH/large.in $MEASURING_OUTPUT_PATH/large16.out
         ;;
*) echo -e "There is no test '$1'.\n"
   ;;
esac