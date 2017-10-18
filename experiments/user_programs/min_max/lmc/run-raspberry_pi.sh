#!/bin/bash

PYTHON_SCRIPT_PATH=../../../linux
MEASURING_INPUT_PATH=/home/pi/libMiletusCoisa/user_programs/user_input
MEASURING_OUTPUT_PATH=/home/pi/libMiletusCoisa/user_output/min_max

case "$1" in

small05) ssh pi@$2 "rm -f $MEASURING_OUTPUT_PATH/small05.out"
         python $PYTHON_SCRIPT_PATH/send_coisa.py x/min_max_small05.x $MEASURING_INPUT_PATH/small.in $MEASURING_OUTPUT_PATH/small05.out $2
         ;;
small10) ssh pi@$2 "rm -f $MEASURING_OUTPUT_PATH/small10.out"
         python $PYTHON_SCRIPT_PATH/send_coisa.py x/min_max_small10.x $MEASURING_INPUT_PATH/small.in $MEASURING_OUTPUT_PATH/small10.out $2
         ;;
small15) ssh pi@$2 "rm -f $MEASURING_OUTPUT_PATH/small15.out"
         python $PYTHON_SCRIPT_PATH/send_coisa.py x/min_max_small15.x $MEASURING_INPUT_PATH/small.in $MEASURING_OUTPUT_PATH/small15.out $2
         ;;
small20) ssh pi@$2 "rm -f $MEASURING_OUTPUT_PATH/small20.out"
         python $PYTHON_SCRIPT_PATH/send_coisa.py x/min_max_small20.x $MEASURING_INPUT_PATH/small.in $MEASURING_OUTPUT_PATH/small20.out $2
         ;;
small25) ssh pi@$2 "rm -f $MEASURING_OUTPUT_PATH/small25.out"
         python $PYTHON_SCRIPT_PATH/send_coisa.py x/min_max_small25.x $MEASURING_INPUT_PATH/small.in $MEASURING_OUTPUT_PATH/small25.out $2
         ;;
small30) ssh pi@$2 "rm -f $MEASURING_OUTPUT_PATH/small30.out"
         python $PYTHON_SCRIPT_PATH/send_coisa.py x/min_max_small30.x $MEASURING_INPUT_PATH/small.in $MEASURING_OUTPUT_PATH/small30.out $2
         ;;
medium05) ssh pi@$2 "rm -f $MEASURING_OUTPUT_PATH/medium05.out"
          python $PYTHON_SCRIPT_PATH/send_coisa.py x/min_max_medium05.x $MEASURING_INPUT_PATH/medium.in $MEASURING_OUTPUT_PATH/medium05.out $2
          ;;
medium10) ssh pi@$2 "rm -f $MEASURING_OUTPUT_PATH/medium10.out"
          python $PYTHON_SCRIPT_PATH/send_coisa.py x/min_max_medium10.x $MEASURING_INPUT_PATH/medium.in $MEASURING_OUTPUT_PATH/medium10.out $2
          ;;
medium15) ssh pi@$2 "rm -f $MEASURING_OUTPUT_PATH/medium15.out"
          python $PYTHON_SCRIPT_PATH/send_coisa.py x/min_max_medium15.x $MEASURING_INPUT_PATH/medium.in $MEASURING_OUTPUT_PATH/medium15.out $2
          ;;
medium20) ssh pi@$2 "rm -f $MEASURING_OUTPUT_PATH/medium20.out"
          python $PYTHON_SCRIPT_PATH/send_coisa.py x/min_max_medium20.x $MEASURING_INPUT_PATH/medium.in $MEASURING_OUTPUT_PATH/medium20.out $2
          ;;
medium25) ssh pi@$2 "rm -f $MEASURING_OUTPUT_PATH/medium25.out"
          python $PYTHON_SCRIPT_PATH/send_coisa.py x/min_max_medium25.x $MEASURING_INPUT_PATH/medium.in $MEASURING_OUTPUT_PATH/medium25.out $2
          ;;
medium30) ssh pi@$2 "rm -f $MEASURING_OUTPUT_PATH/medium30.out"
          python $PYTHON_SCRIPT_PATH/send_coisa.py x/min_max_medium30.x $MEASURING_INPUT_PATH/medium.in $MEASURING_OUTPUT_PATH/medium30.out $2
          ;;
large05) ssh pi@$2 "rm -f $MEASURING_OUTPUT_PATH/large05.out"
         python $PYTHON_SCRIPT_PATH/send_coisa.py x/min_max_large05.x $MEASURING_INPUT_PATH/large.in $MEASURING_OUTPUT_PATH/large05.out $2
         ;;
large10) ssh pi@$2 "rm -f $MEASURING_OUTPUT_PATH/large10.out"
         python $PYTHON_SCRIPT_PATH/send_coisa.py x/min_max_large10.x $MEASURING_INPUT_PATH/large.in $MEASURING_OUTPUT_PATH/large10.out $2
         ;;
large15) ssh pi@$2 "rm -f $MEASURING_OUTPUT_PATH/large15.out"
         python $PYTHON_SCRIPT_PATH/send_coisa.py x/min_max_large15.x $MEASURING_INPUT_PATH/large.in $MEASURING_OUTPUT_PATH/large15.out $2
         ;;
large20) ssh pi@$2 "rm -f $MEASURING_OUTPUT_PATH/large20.out"
         python $PYTHON_SCRIPT_PATH/send_coisa.py x/min_max_large20.x $MEASURING_INPUT_PATH/large.in $MEASURING_OUTPUT_PATH/large20.out $2
         ;;
large25) ssh pi@$2 "rm -f $MEASURING_OUTPUT_PATH/large25.out"
         python $PYTHON_SCRIPT_PATH/send_coisa.py x/min_max_large25.x $MEASURING_INPUT_PATH/large.in $MEASURING_OUTPUT_PATH/large25.out $2
         ;;
large30) ssh pi@$2 "rm -f $MEASURING_OUTPUT_PATH/large30.out"
         python $PYTHON_SCRIPT_PATH/send_coisa.py x/min_max_large30.x $MEASURING_INPUT_PATH/large.in $MEASURING_OUTPUT_PATH/large30.out $2
         ;;
*) echo -e "There is no test '$1'.\n"
   ;;
esac