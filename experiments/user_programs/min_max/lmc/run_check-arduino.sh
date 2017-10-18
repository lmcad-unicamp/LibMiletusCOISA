#!/bin/bash

PYTHON_SCRIPT_PATH=../../../linux
INPUT_PATH=../in
OUTPUT_PATH=out

case "$1" in

1)  #./compile.sh 10 30 0
    rm -f $OUTPUT_PATH/t01.out
    python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/min_max_01.x $2 $INPUT_PATH/t01.in $OUTPUT_PATH/t01.out
    ./check.sh 1
    ;;
2)  #./compile.sh 1000 3000 0
    rm -f $OUTPUT_PATH/t02.out
    python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/min_max_02.x $2 $INPUT_PATH/t02.in $OUTPUT_PATH/t02.out
    ./check.sh 2
    ;;
3)  #./compile.sh 1.5 5.5 0
    rm -f $OUTPUT_PATH/t03.out
    python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/min_max_03.x $2 $INPUT_PATH/t03.in $OUTPUT_PATH/t03.out
    ./check.sh 3
    ;;
4)  #./compile.sh 10.2 14.2 0
    rm -f $OUTPUT_PATH/t04.out
    python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/min_max_04.x $2 $INPUT_PATH/t04.in $OUTPUT_PATH/t04.out
    ./check.sh 4
    ;;
5)  #./compile.sh 300 400 0
    rm -f $OUTPUT_PATH/t05.out
    python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/min_max_05.x $2 $INPUT_PATH/t05.in $OUTPUT_PATH/t05.out
    ./check.sh 5
    ;;
6)  #./compile.sh 1.0 2.0 0
    rm -f $OUTPUT_PATH/t06.out
    python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/min_max_06.x $2 $INPUT_PATH/t06.in $OUTPUT_PATH/t06.out
    ./check.sh 6
    ;;
7)  #./compile.sh 0.65 0.7 0
    rm -f $OUTPUT_PATH/t07.out
    python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/min_max_07.x $2 $INPUT_PATH/t07.in $OUTPUT_PATH/t07.out
    ./check.sh 7
    ;;
8)  #./compile.sh 20 30.7 0
    rm -f $OUTPUT_PATH/t08.out
    python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/min_max_08.x $2 $INPUT_PATH/t08.in $OUTPUT_PATH/t08.out
    ./check.sh 8
    ;;
9)  #./compile.sh 0.0 10.0 1
    rm -f $OUTPUT_PATH/t09.out
    python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/min_max_09.x $2 $INPUT_PATH/t09.in $OUTPUT_PATH/t09.out
    ./check.sh 9
    ;;
10) #./compile.sh 5 15 1
    rm -f $OUTPUT_PATH/t10.out
    python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/min_max_10.x $2 $INPUT_PATH/t10.in $OUTPUT_PATH/t10.out
    ./check.sh 10
    ;;
all) rm -f $OUTPUT_PATH/*.out
     #./compile.sh 10 30 0
     python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/min_max_01.x $2 $INPUT_PATH/t01.in $OUTPUT_PATH/t01.out

     #./compile.sh 1000 3000 0
     python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/min_max_02.x $2 $INPUT_PATH/t02.in $OUTPUT_PATH/t02.out

     #./compile.sh 1.5 5.5 0
     python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/min_max_03.x $2 $INPUT_PATH/t03.in $OUTPUT_PATH/t03.out

     #./compile.sh 10.2 14.2 0
     python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/min_max_04.x $2 $INPUT_PATH/t04.in $OUTPUT_PATH/t04.out

     #./compile.sh 300 400 0
     python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/min_max_05.x $2 $INPUT_PATH/t05.in $OUTPUT_PATH/t05.out

     #./compile.sh 1.0 2.0 0
     python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/min_max_06.x $2 $INPUT_PATH/t06.in $OUTPUT_PATH/t06.out

     #./compile.sh 0.65 0.7 0
     python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/min_max_07.x $2 $INPUT_PATH/t07.in $OUTPUT_PATH/t07.out

     #./compile.sh 20 30.7 0
     python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/min_max_08.x $2 $INPUT_PATH/t08.in $OUTPUT_PATH/t08.out

     #./compile.sh 0.0 10.0 1
     python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/min_max_09.x $2 $INPUT_PATH/t09.in $OUTPUT_PATH/t09.out

     #./compile.sh 5 15 1
     python $PYTHON_SCRIPT_PATH/send_coisa-arduino.py x/min_max_10.x $2 $INPUT_PATH/t10.in $OUTPUT_PATH/t10.out

     ./check.sh 1

     ./check.sh 2

     ./check.sh 3

     ./check.sh 4

     ./check.sh 5

     ./check.sh 6

     ./check.sh 7

     ./check.sh 8

     ./check.sh 9

     ./check.sh 10
     ;;
*) echo -e "There is no test '$1'.\n"
   ;;
esac