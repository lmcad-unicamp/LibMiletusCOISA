#!/bin/bash

PYTHON_SCRIPT_PATH=../../../linux
MEASURING_INPUT_PATH=../../user_input
MEASURING_OUTPUT_PATH=../../user_output/min_max

case "$1" in

small05) #./compile.sh -5 5 1
         rm -f $MEASURING_OUTPUT_PATH/small05.out
         python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/min_max_small05.x $2 $MEASURING_INPUT_PATH/small.in $MEASURING_OUTPUT_PATH/small05.out
         ;;
small10) #./compile.sh -10 10 1
         rm -f $MEASURING_OUTPUT_PATH/small10.out
         python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/min_max_small10.x $2 $MEASURING_INPUT_PATH/small.in $MEASURING_OUTPUT_PATH/small10.out
         ;;
small15) #./compile.sh -15 15 1
         rm -f $MEASURING_OUTPUT_PATH/small15.out
         python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/min_max_small15.x $2 $MEASURING_INPUT_PATH/small.in $MEASURING_OUTPUT_PATH/small15.out
         ;;
small20) #./compile.sh -20 20 1
         rm -f $MEASURING_OUTPUT_PATH/small20.out
         python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/min_max_small20.x $2 $MEASURING_INPUT_PATH/small.in $MEASURING_OUTPUT_PATH/small20.out
         ;;
small25) #./compile.sh -25 25 1
         rm -f $MEASURING_OUTPUT_PATH/small25.out
         python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/min_max_small25.x $2 $MEASURING_INPUT_PATH/small.in $MEASURING_OUTPUT_PATH/small25.out
         ;;
small30) #./compile.sh -30 30 1
         rm -f $MEASURING_OUTPUT_PATH/small30.out
         python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/min_max_small30.x $2 $MEASURING_INPUT_PATH/small.in $MEASURING_OUTPUT_PATH/small30.out
         ;;
medium05) #./compile.sh -5 5 1
          rm -f $MEASURING_OUTPUT_PATH/medium05.out
          python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/min_max_medium05.x $2 $MEASURING_INPUT_PATH/medium.in $MEASURING_OUTPUT_PATH/medium05.out
          ;;
medium10) #./compile.sh -10 10 1
          rm -f $MEASURING_OUTPUT_PATH/medium10.out
          python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/min_max_medium10.x $2 $MEASURING_INPUT_PATH/medium.in $MEASURING_OUTPUT_PATH/medium10.out
          ;;
medium15) #./compile.sh -15 15 1
          rm -f $MEASURING_OUTPUT_PATH/medium15.out
          python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/min_max_medium15.x $2 $MEASURING_INPUT_PATH/medium.in $MEASURING_OUTPUT_PATH/medium15.out
          ;;
medium20) #./compile.sh -20 20 1
          rm -f $MEASURING_OUTPUT_PATH/medium20.out
          python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/min_max_medium20.x $2 $MEASURING_INPUT_PATH/medium.in $MEASURING_OUTPUT_PATH/medium20.out
          ;;
medium25) #./compile.sh -25 25 1
          rm -f $MEASURING_OUTPUT_PATH/medium25.out
          python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/min_max_medium25.x $2 $MEASURING_INPUT_PATH/medium.in $MEASURING_OUTPUT_PATH/medium25.out
          ;;
medium30) #./compile.sh -30 30 1
          rm -f $MEASURING_OUTPUT_PATH/medium30.out
          python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/min_max_medium30.x $2 $MEASURING_INPUT_PATH/medium.in $MEASURING_OUTPUT_PATH/medium30.out
          ;;
large05) #./compile.sh -5 5 1
         rm -f $MEASURING_OUTPUT_PATH/large05.out
         python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/min_max_large05.x $2 $MEASURING_INPUT_PATH/large.in $MEASURING_OUTPUT_PATH/large05.out
         ;;
large10) #./compile.sh -10 10 1
         rm -f $MEASURING_OUTPUT_PATH/large10.out
         python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/min_max_large10.x $2 $MEASURING_INPUT_PATH/large.in $MEASURING_OUTPUT_PATH/large10.out
         ;;
large15) #./compile.sh -15 15 1
         rm -f $MEASURING_OUTPUT_PATH/large15.out
         python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/min_max_large15.x $2 $MEASURING_INPUT_PATH/large.in $MEASURING_OUTPUT_PATH/large15.out
         ;;
large20) #./compile.sh -20 20 1
         rm -f $MEASURING_OUTPUT_PATH/large20.out
         python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/min_max_large20.x $2 $MEASURING_INPUT_PATH/large.in $MEASURING_OUTPUT_PATH/large20.out
         ;;
large25) #./compile.sh -25 25 1
         rm -f $MEASURING_OUTPUT_PATH/large25.out
         python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/min_max_large25.x $2 $MEASURING_INPUT_PATH/large.in $MEASURING_OUTPUT_PATH/large25.out
         ;;
large30) #./compile.sh -30 30 1
         rm -f $MEASURING_OUTPUT_PATH/large30.out
         python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/min_max_large30.x $2 $MEASURING_INPUT_PATH/large.in $MEASURING_OUTPUT_PATH/large30.out
         ;;
*) echo -e "There is no test '$1'.\n"
   ;;
esac