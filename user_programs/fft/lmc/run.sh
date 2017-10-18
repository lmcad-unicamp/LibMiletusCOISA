#!/bin/bash

PYTHON_SCRIPT_PATH=../../../linux
MEASURING_INPUT_PATH=../../user_input
MEASURING_OUTPUT_PATH=../../user_output/fft

case "$1" in

small04) #./compile.sh 4 1
         rm -f $MEASURING_OUTPUT_PATH/small04.out
         python $PYTHON_SCRIPT_PATH/send_coisa.py x/fft_small04.x $MEASURING_INPUT_PATH/small.in $MEASURING_OUTPUT_PATH/small04.out $2
         ;;
small08) #./compile.sh 8 1
         rm -f $MEASURING_OUTPUT_PATH/small08.out
         python $PYTHON_SCRIPT_PATH/send_coisa.py x/fft_small08.x $MEASURING_INPUT_PATH/small.in $MEASURING_OUTPUT_PATH/small08.out $2
         ;;
medium04) #./compile.sh 4 1
          rm -f $MEASURING_OUTPUT_PATH/medium04.out
          python $PYTHON_SCRIPT_PATH/send_coisa.py x/fft_medium04.x $MEASURING_INPUT_PATH/medium.in $MEASURING_OUTPUT_PATH/medium04.out $2
          ;;
medium08) #./compile.sh 8 1
          rm -f $MEASURING_OUTPUT_PATH/medium08.out
          python $PYTHON_SCRIPT_PATH/send_coisa.py x/fft_medium08.x $MEASURING_INPUT_PATH/medium.in $MEASURING_OUTPUT_PATH/medium08.out $2
          ;;
medium16) #./compile.sh 16 1
          rm -f $MEASURING_OUTPUT_PATH/medium16.out
          python $PYTHON_SCRIPT_PATH/send_coisa.py x/fft_medium16.x $MEASURING_INPUT_PATH/medium.in $MEASURING_OUTPUT_PATH/medium16.out $2
          ;;
large04) #./compile.sh 4 1
         rm -f $MEASURING_OUTPUT_PATH/large04.out
         python $PYTHON_SCRIPT_PATH/send_coisa.py x/fft_large04.x $MEASURING_INPUT_PATH/large.in $MEASURING_OUTPUT_PATH/large04.out $2
         ;;
large08) #./compile.sh 8 1
         rm -f $MEASURING_OUTPUT_PATH/large08.out
         python $PYTHON_SCRIPT_PATH/send_coisa.py x/fft_large08.x $MEASURING_INPUT_PATH/large.in $MEASURING_OUTPUT_PATH/large08.out $2
         ;;
large16) #./compile.sh 16 1
         rm -f $MEASURING_OUTPUT_PATH/large16.out
         python $PYTHON_SCRIPT_PATH/send_coisa.py x/fft_large16.x $MEASURING_INPUT_PATH/large.in $MEASURING_OUTPUT_PATH/large16.out $2
         ;;
*) echo -e "There is no test '$1'.\n"
   ;;
esac