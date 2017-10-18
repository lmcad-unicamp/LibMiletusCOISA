#!/bin/bash

PYTHON_SCRIPT_PATH=../../../linux
INPUT_PATH=/home/pi/libMiletusCoisa/user_programs/fft/in
OUTPUT_PATH=/home/pi/libMiletusCoisa/user_programs/fft/lmc/out
CHECK_PATH=/home/pi/libMiletusCoisa/user_programs/fft/lmc

case "$1" in

1)  ssh pi@$2 "rm -f $OUTPUT_PATH/t01.out"
    python $PYTHON_SCRIPT_PATH/send_coisa.py x/fft_01.x $INPUT_PATH/t01.in $OUTPUT_PATH/t01.out $2
    ssh pi@$2 "cd $CHECK_PATH; ./check.sh 1"
    ;;
2)  ssh pi@$2 "rm -f $OUTPUT_PATH/t02.out"
    python $PYTHON_SCRIPT_PATH/send_coisa.py x/fft_02.x $INPUT_PATH/t02.in $OUTPUT_PATH/t02.out $2
    ssh pi@$2 "cd $CHECK_PATH; ./check.sh 2"
    ;;
*) echo -e "There is no test '$1'.\n"
   ;;
esac