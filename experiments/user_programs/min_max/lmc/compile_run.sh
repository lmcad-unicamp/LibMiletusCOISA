#!/bin/bash

PYTHON_SCRIPT_PATH=../../../linux
MEASURING_INPUT_PATH=../../user_input
MEASURING_OUTPUT_PATH=../../user_output/min_max

case "$1" in

small05) ./compile.sh -5 5 1
         clear
         rm -f $MEASURING_OUTPUT_PATH/small05.out
         python $PYTHON_SCRIPT_PATH/send_coisa.py min_max.x $MEASURING_INPUT_PATH/small.in $MEASURING_OUTPUT_PATH/small05.out $2
         ;;
small10) ./compile.sh -10 10 1
         clear
         rm -f $MEASURING_OUTPUT_PATH/small10.out
         python $PYTHON_SCRIPT_PATH/send_coisa.py min_max.x $MEASURING_INPUT_PATH/small.in $MEASURING_OUTPUT_PATH/small10.out $2
         ;;
small15) ./compile.sh -15 15 1
         clear
         rm -f $MEASURING_OUTPUT_PATH/small15.out
         python $PYTHON_SCRIPT_PATH/send_coisa.py min_max.x $MEASURING_INPUT_PATH/small.in $MEASURING_OUTPUT_PATH/small15.out $2
         ;;
small20) ./compile.sh -20 20 1
         clear
         rm -f $MEASURING_OUTPUT_PATH/small20.out
         python $PYTHON_SCRIPT_PATH/send_coisa.py min_max.x $MEASURING_INPUT_PATH/small.in $MEASURING_OUTPUT_PATH/small20.out $2
         ;;
small25) ./compile.sh -25 25 1
         clear
         rm -f $MEASURING_OUTPUT_PATH/small25.out
         python $PYTHON_SCRIPT_PATH/send_coisa.py min_max.x $MEASURING_INPUT_PATH/small.in $MEASURING_OUTPUT_PATH/small25.out $2
         ;;
small30) ./compile.sh -30 30 1
         clear
         rm -f $MEASURING_OUTPUT_PATH/small30.out
         python $PYTHON_SCRIPT_PATH/send_coisa.py min_max.x $MEASURING_INPUT_PATH/small.in $MEASURING_OUTPUT_PATH/small30.out $2
         ;;
medium05) ./compile.sh -5 5 1
          clear
          rm -f $MEASURING_OUTPUT_PATH/medium05.out
          python $PYTHON_SCRIPT_PATH/send_coisa.py min_max.x $MEASURING_INPUT_PATH/medium.in $MEASURING_OUTPUT_PATH/medium05.out $2
          ;;
medium10) ./compile.sh -10 10 1
          clear
          rm -f $MEASURING_OUTPUT_PATH/medium10.out
          python $PYTHON_SCRIPT_PATH/send_coisa.py min_max.x $MEASURING_INPUT_PATH/medium.in $MEASURING_OUTPUT_PATH/medium10.out $2
          ;;
medium15) ./compile.sh -15 15 1
          clear
          rm -f $MEASURING_OUTPUT_PATH/medium15.out
          python $PYTHON_SCRIPT_PATH/send_coisa.py min_max.x $MEASURING_INPUT_PATH/medium.in $MEASURING_OUTPUT_PATH/medium15.out $2
          ;;
medium20) ./compile.sh -20 20 1
          clear
          rm -f $MEASURING_OUTPUT_PATH/medium20.out
          python $PYTHON_SCRIPT_PATH/send_coisa.py min_max.x $MEASURING_INPUT_PATH/medium.in $MEASURING_OUTPUT_PATH/medium20.out $2
          ;;
medium25) ./compile.sh -25 25 1
          clear
          rm -f $MEASURING_OUTPUT_PATH/medium25.out
          python $PYTHON_SCRIPT_PATH/send_coisa.py min_max.x $MEASURING_INPUT_PATH/medium.in $MEASURING_OUTPUT_PATH/medium25.out $2
          ;;
medium30) ./compile.sh -30 30 1
          clear
          rm -f $MEASURING_OUTPUT_PATH/medium30.out
          python $PYTHON_SCRIPT_PATH/send_coisa.py min_max.x $MEASURING_INPUT_PATH/medium.in $MEASURING_OUTPUT_PATH/medium30.out $2
          ;;
large05) ./compile.sh -5 5 1
         clear
         rm -f $MEASURING_OUTPUT_PATH/large05.out
         python $PYTHON_SCRIPT_PATH/send_coisa.py min_max.x $MEASURING_INPUT_PATH/large.in $MEASURING_OUTPUT_PATH/large05.out $2
         ;;
large10) ./compile.sh -10 10 1
         clear
         rm -f $MEASURING_OUTPUT_PATH/large10.out
         python $PYTHON_SCRIPT_PATH/send_coisa.py min_max.x $MEASURING_INPUT_PATH/large.in $MEASURING_OUTPUT_PATH/large10.out $2
         ;;
large15) ./compile.sh -15 15 1
         clear
         rm -f $MEASURING_OUTPUT_PATH/large15.out
         python $PYTHON_SCRIPT_PATH/send_coisa.py min_max.x $MEASURING_INPUT_PATH/large.in $MEASURING_OUTPUT_PATH/large15.out $2
         ;;
large20) ./compile.sh -20 20 1
         clear
         rm -f $MEASURING_OUTPUT_PATH/large20.out
         python $PYTHON_SCRIPT_PATH/send_coisa.py min_max.x $MEASURING_INPUT_PATH/large.in $MEASURING_OUTPUT_PATH/large20.out $2
         ;;
large25) ./compile.sh -25 25 1
         clear
         rm -f $MEASURING_OUTPUT_PATH/large25.out
         python $PYTHON_SCRIPT_PATH/send_coisa.py min_max.x $MEASURING_INPUT_PATH/large.in $MEASURING_OUTPUT_PATH/large25.out $2
         ;;
large30) ./compile.sh -30 30 1
         clear
         rm -f $MEASURING_OUTPUT_PATH/large30.out
         python $PYTHON_SCRIPT_PATH/send_coisa.py min_max.x $MEASURING_INPUT_PATH/large.in $MEASURING_OUTPUT_PATH/large30.out $2
         ;;
*) echo -e "There is no test '$1'.\n"
   ;;
esac