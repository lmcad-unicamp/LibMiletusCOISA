#!/bin/bash

PYTHON_SCRIPT_PATH=../../../linux
MEASURING_INPUT_PATH=../../user_input
MEASURING_OUTPUT_PATH=../../user_output/outlier

case "$1" in

small05) #./compile.sh 5 1
         rm -f $MEASURING_OUTPUT_PATH/small05.out
         python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/outlier_small05.x $2 $MEASURING_INPUT_PATH/small.in $MEASURING_OUTPUT_PATH/small05.out
         ;;
small10) #./compile.sh 10 1
         rm -f $MEASURING_OUTPUT_PATH/small10.out
         python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/outlier_small10.x $2 $MEASURING_INPUT_PATH/small.in $MEASURING_OUTPUT_PATH/small10.out
         ;;
medium05) #./compile.sh 5 1
          rm -f $MEASURING_OUTPUT_PATH/medium05.out
          python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/outlier_medium05.x $2 $MEASURING_INPUT_PATH/medium.in $MEASURING_OUTPUT_PATH/medium05.out
          ;;
medium10) #./compile.sh 10 1
          rm -f $MEASURING_OUTPUT_PATH/medium10.out
          python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/outlier_medium10.x $2 $MEASURING_INPUT_PATH/medium.in $MEASURING_OUTPUT_PATH/medium10.out
          ;;
medium20) #./compile.sh 20 1
          rm -f $MEASURING_OUTPUT_PATH/medium20.out
          python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/outlier_medium20.x $2 $MEASURING_INPUT_PATH/medium.in $MEASURING_OUTPUT_PATH/medium20.out
          ;;
large05) #./compile.sh 5 1
         rm -f $MEASURING_OUTPUT_PATH/large05.out
         python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/outlier_large05.x $2 $MEASURING_INPUT_PATH/large.in $MEASURING_OUTPUT_PATH/large05.out
         ;;
large10) #./compile.sh 10 1
         rm -f $MEASURING_OUTPUT_PATH/large10.out
         python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/outlier_large10.x $2 $MEASURING_INPUT_PATH/large.in $MEASURING_OUTPUT_PATH/large10.out
         ;;
large20) #./compile.sh 20 1
         rm -f $MEASURING_OUTPUT_PATH/large20.out
         python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/outlier_large20.x $2 $MEASURING_INPUT_PATH/large.in $MEASURING_OUTPUT_PATH/large20.out
         ;;
*) echo -e "There is no test '$1'.\n"
   ;;
esac