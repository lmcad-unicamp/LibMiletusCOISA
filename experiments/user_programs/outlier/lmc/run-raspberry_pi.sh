#!/bin/bash

PYTHON_SCRIPT_PATH=../../../linux
MEASURING_INPUT_PATH=/home/pi/libMiletusCoisa/user_programs/user_input
MEASURING_OUTPUT_PATH=/home/pi/libMiletusCoisa/user_output/outlier

case "$1" in

small05) ssh pi@$2 "rm -f $MEASURING_OUTPUT_PATH/small05.out"
         python $PYTHON_SCRIPT_PATH/send_coisa.py x/outlier_small05.x $MEASURING_INPUT_PATH/small05.in $MEASURING_OUTPUT_PATH/small05.out $2
         ;;
small10) ssh pi@$2 "rm -f $MEASURING_OUTPUT_PATH/small10.out"
         python $PYTHON_SCRIPT_PATH/send_coisa.py x/outlier_small10.x $MEASURING_INPUT_PATH/small10.in $MEASURING_OUTPUT_PATH/small10.out $2
         ;;
medium05) ssh pi@$2 "rm -f $MEASURING_OUTPUT_PATH/medium05.out"
          python $PYTHON_SCRIPT_PATH/send_coisa.py x/outlier_medium05.x $MEASURING_INPUT_PATH/medium05.in $MEASURING_OUTPUT_PATH/medium05.out $2
          ;;
medium10) ssh pi@$2 "rm -f $MEASURING_OUTPUT_PATH/medium10.out"
          python $PYTHON_SCRIPT_PATH/send_coisa.py x/outlier_medium10.x $MEASURING_INPUT_PATH/medium10.in $MEASURING_OUTPUT_PATH/medium10.out $2
          ;;
medium20) ssh pi@$2 "rm -f $MEASURING_OUTPUT_PATH/medium20.out"
          python $PYTHON_SCRIPT_PATH/send_coisa.py x/outlier_medium20.x $MEASURING_INPUT_PATH/medium20.in $MEASURING_OUTPUT_PATH/medium20.out $2
          ;;
large05) ssh pi@$2 "rm -f $MEASURING_OUTPUT_PATH/large05.out"
         python $PYTHON_SCRIPT_PATH/send_coisa.py x/outlier_large05.x $MEASURING_INPUT_PATH/large05.in $MEASURING_OUTPUT_PATH/large05.out $2
         ;;
large10) ssh pi@$2 "rm -f $MEASURING_OUTPUT_PATH/large10.out"
         python $PYTHON_SCRIPT_PATH/send_coisa.py x/outlier_large10.x $MEASURING_INPUT_PATH/large10.in $MEASURING_OUTPUT_PATH/large10.out $2
         ;;
large20) ssh pi@$2 "rm -f $MEASURING_OUTPUT_PATH/large20.out"
         python $PYTHON_SCRIPT_PATH/send_coisa.py x/outlier_large20.x $MEASURING_INPUT_PATH/large20.in $MEASURING_OUTPUT_PATH/large20.out $2
         ;;
*) echo -e "There is no test '$1'.\n"
   ;;
esac