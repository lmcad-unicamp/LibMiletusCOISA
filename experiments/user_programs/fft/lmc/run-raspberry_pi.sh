#!/bin/bash

PYTHON_SCRIPT_PATH=../../../linux
MEASURING_INPUT_PATH=/home/pi/libMiletusCoisa/user_programs/user_input
MEASURING_OUTPUT_PATH=/home/pi/libMiletusCoisa/user_output/fft

case "$1" in

small04) ssh pi@$2 "rm -f $MEASURING_OUTPUT_PATH/small04.out"
         python $PYTHON_SCRIPT_PATH/send_coisa.py x/fft_small04.x $MEASURING_INPUT_PATH/small04.in $MEASURING_OUTPUT_PATH/small04.out $2
         ;;
small08) ssh pi@$2 "rm -f $MEASURING_OUTPUT_PATH/small08.out"
         python $PYTHON_SCRIPT_PATH/send_coisa.py x/fft_small08.x $MEASURING_INPUT_PATH/small08.in $MEASURING_OUTPUT_PATH/small08.out $2
         ;;
medium04) ssh pi@$2 "rm -f $MEASURING_OUTPUT_PATH/medium04.out"
          python $PYTHON_SCRIPT_PATH/send_coisa.py x/fft_medium04.x $MEASURING_INPUT_PATH/medium04.in $MEASURING_OUTPUT_PATH/medium04.out $2
          ;;
medium08) ssh pi@$2 "rm -f $MEASURING_OUTPUT_PATH/medium08.out"
          python $PYTHON_SCRIPT_PATH/send_coisa.py x/fft_medium08.x $MEASURING_INPUT_PATH/medium08.in $MEASURING_OUTPUT_PATH/medium08.out $2
          ;;
medium16) ssh pi@$2 "rm -f $MEASURING_OUTPUT_PATH/medium16.out"
          python $PYTHON_SCRIPT_PATH/send_coisa.py x/fft_medium16.x $MEASURING_INPUT_PATH/medium16.in $MEASURING_OUTPUT_PATH/medium16.out $2
          ;;
large04) ssh pi@$2 "rm -f $MEASURING_OUTPUT_PATH/large04.out"
         python $PYTHON_SCRIPT_PATH/send_coisa.py x/fft_large04.x $MEASURING_INPUT_PATH/large04.in $MEASURING_OUTPUT_PATH/large04.out $2
         ;;
large08) ssh pi@$2 "rm -f $MEASURING_OUTPUT_PATH/large08.out"
         python $PYTHON_SCRIPT_PATH/send_coisa.py x/fft_large08.x $MEASURING_INPUT_PATH/large08.in $MEASURING_OUTPUT_PATH/large08.out $2
         ;;
large16) ssh pi@$2 "rm -f $MEASURING_OUTPUT_PATH/large16.out"
         python $PYTHON_SCRIPT_PATH/send_coisa.py x/fft_large16.x $MEASURING_INPUT_PATH/large16.in $MEASURING_OUTPUT_PATH/large16.out $2
         ;;
*) echo -e "There is no test '$1'.\n"
   ;;
esac