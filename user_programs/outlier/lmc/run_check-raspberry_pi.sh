#!/bin/bash

PYTHON_SCRIPT_PATH=../../../linux
INPUT_PATH=/home/pi/libMiletusCoisa/user_programs/outlier/in
OUTPUT_PATH=/home/pi/libMiletusCoisa/user_programs/outlier/lmc/out
CHECK_PATH=/home/pi/libMiletusCoisa/user_programs/outlier/lmc

case "$1" in

1)  ssh pi@$2 "rm -f $OUTPUT_PATH/t01.out"
    python $PYTHON_SCRIPT_PATH/send_coisa.py x/outlier_01.x $INPUT_PATH/t01.in $OUTPUT_PATH/t01.out $2
    ssh pi@$2 "cd $CHECK_PATH; ./check.sh 1"
    ;;
2)  ssh pi@$2 "rm -f $OUTPUT_PATH/t02.out"
    python $PYTHON_SCRIPT_PATH/send_coisa.py x/outlier_02.x $INPUT_PATH/t02.in $OUTPUT_PATH/t02.out $2
    ssh pi@$2 "cd $CHECK_PATH; ./check.sh 2"
    ;;
3)  ssh pi@$2 "rm -f $OUTPUT_PATH/t03.out"
    python $PYTHON_SCRIPT_PATH/send_coisa.py x/outlier_03.x $INPUT_PATH/t03.in $OUTPUT_PATH/t03.out $2
    ssh pi@$2 "cd $CHECK_PATH; ./check.sh 3"
    ;;
4)  ssh pi@$2 "rm -f $OUTPUT_PATH/t04.out"
    python $PYTHON_SCRIPT_PATH/send_coisa.py x/outlier_04.x $INPUT_PATH/t04.in $OUTPUT_PATH/t04.out $2
    ssh pi@$2 "cd $CHECK_PATH; ./check.sh 4"
    ;;
5)  ssh pi@$2 "rm -f $OUTPUT_PATH/t05.out"
    python $PYTHON_SCRIPT_PATH/send_coisa.py x/outlier_05.x $INPUT_PATH/t05.in $OUTPUT_PATH/t05.out $2
    ssh pi@$2 "cd $CHECK_PATH; ./check.sh 5"
    ;;
6)  ssh pi@$2 "rm -f $OUTPUT_PATH/t06.out"
    python $PYTHON_SCRIPT_PATH/send_coisa.py x/outlier_06.x $INPUT_PATH/t06.in $OUTPUT_PATH/t06.out $2
    ssh pi@$2 "cd $CHECK_PATH; ./check.sh 6"
    ;;
7)  ssh pi@$2 "rm -f $OUTPUT_PATH/t07.out"
    python $PYTHON_SCRIPT_PATH/send_coisa.py x/outlier_07.x $INPUT_PATH/t07.in $OUTPUT_PATH/t07.out $2
    ssh pi@$2 "cd $CHECK_PATH; ./check.sh 7"
    ;;
8)  ssh pi@$2 "rm -f $OUTPUT_PATH/t08.out"
    python $PYTHON_SCRIPT_PATH/send_coisa.py x/outlier_08.x $INPUT_PATH/t08.in $OUTPUT_PATH/t08.out $2
    ssh pi@$2 "cd $CHECK_PATH; ./check.sh 8"
    ;;
9)  ssh pi@$2 "rm -f $OUTPUT_PATH/t09.out"
    python $PYTHON_SCRIPT_PATH/send_coisa.py x/outlier_09.x $INPUT_PATH/t09.in $OUTPUT_PATH/t09.out $2
    ssh pi@$2 "cd $CHECK_PATH; ./check.sh 9"
    ;;
10) ssh pi@$2 "rm -f $OUTPUT_PATH/t10.out"
    python $PYTHON_SCRIPT_PATH/send_coisa.py x/outlier_10.x $INPUT_PATH/t10.in $OUTPUT_PATH/t10.out $2
    ssh pi@$2 "cd $CHECK_PATH; ./check.sh 10"
    ;;
*) echo -e "There is no test '$1'.\n"
   ;;
esac