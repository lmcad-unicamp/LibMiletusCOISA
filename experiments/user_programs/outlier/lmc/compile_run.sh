#!/bin/bash

PYTHON_SCRIPT_PATH=../../../linux
MEASURING_INPUT_PATH=../../user_input
MEASURING_OUTPUT_PATH=../../user_output/outlier

case "$1" in

small05) ./compile.sh 5 1
         clear
         rm -f $MEASURING_OUTPUT_PATH/small05.out
         python $PYTHON_SCRIPT_PATH/send_coisa.py outlier.x $MEASURING_INPUT_PATH/small.in $MEASURING_OUTPUT_PATH/small05.out $2
         ;;
small10) ./compile.sh 10 1
         clear
         rm -f $MEASURING_OUTPUT_PATH/small10.out
         python $PYTHON_SCRIPT_PATH/send_coisa.py outlier.x $MEASURING_INPUT_PATH/small.in $MEASURING_OUTPUT_PATH/small10.out $2
         ;;
medium05) ./compile.sh 5 1
          clear
          rm -f $MEASURING_OUTPUT_PATH/medium05.out
          python $PYTHON_SCRIPT_PATH/send_coisa.py outlier.x $MEASURING_INPUT_PATH/medium.in $MEASURING_OUTPUT_PATH/medium05.out $2
          ;;
medium10) ./compile.sh 10 1
          clear
          rm -f $MEASURING_OUTPUT_PATH/medium10.out
          python $PYTHON_SCRIPT_PATH/send_coisa.py outlier.x $MEASURING_INPUT_PATH/medium.in $MEASURING_OUTPUT_PATH/medium10.out $2
          ;;
medium20) ./compile.sh 20 1
          clear
          rm -f $MEASURING_OUTPUT_PATH/medium20.out
          python $PYTHON_SCRIPT_PATH/send_coisa.py outlier.x $MEASURING_INPUT_PATH/medium.in $MEASURING_OUTPUT_PATH/medium20.out $2
          ;;
large05) ./compile.sh 5 1
         clear
         rm -f $MEASURING_OUTPUT_PATH/large05.out
         python $PYTHON_SCRIPT_PATH/send_coisa.py outlier.x $MEASURING_INPUT_PATH/large.in $MEASURING_OUTPUT_PATH/large05.out $2
         ;;
large10) ./compile.sh 10 1
         clear
         rm -f $MEASURING_OUTPUT_PATH/large10.out
         python $PYTHON_SCRIPT_PATH/send_coisa.py outlier.x $MEASURING_INPUT_PATH/large.in $MEASURING_OUTPUT_PATH/large10.out $2
         ;;
large20) ./compile.sh 20 1
         clear
         rm -f $MEASURING_OUTPUT_PATH/large20.out
         python $PYTHON_SCRIPT_PATH/send_coisa.py outlier.x $MEASURING_INPUT_PATH/large.in $MEASURING_OUTPUT_PATH/large20.out $2
         ;;
*) echo -e "There is no test '$1'.\n"
   ;;
esac