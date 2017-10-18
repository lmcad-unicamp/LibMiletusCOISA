#!/bin/bash

GOLD_PATH=../gold
OUTPUT_PATH=out

case "$1" in

1)  echo -e "\n=========================== Test 01 diff ===========================\n"
    diff $GOLD_PATH/t01.gold $OUTPUT_PATH/t01.out
    ;;
2)  echo -e "\n=========================== Test 02 diff ===========================\n"
    diff $GOLD_PATH/t02.gold $OUTPUT_PATH/t02.out
    ;;
3)  echo -e "\n=========================== Test 03 diff ===========================\n"
    diff $GOLD_PATH/t03.gold $OUTPUT_PATH/t03.out
    ;;
4)  echo -e "\n=========================== Test 04 diff ===========================\n"
    diff $GOLD_PATH/t04.gold $OUTPUT_PATH/t04.out
    ;;
5)  echo -e "\n=========================== Test 05 diff ===========================\n"
    diff $GOLD_PATH/t05.gold $OUTPUT_PATH/t05.out
    ;;
6)  echo -e "\n=========================== Test 06 diff ===========================\n"
    diff $GOLD_PATH/t06.gold $OUTPUT_PATH/t06.out
    ;;
7)  echo -e "\n=========================== Test 07 diff ===========================\n"
    diff $GOLD_PATH/t07.gold $OUTPUT_PATH/t07.out
    ;;
8)  echo -e "\n=========================== Test 08 diff ===========================\n"
    diff $GOLD_PATH/t08.gold $OUTPUT_PATH/t08.out
    ;;
9)  echo -e "\n=========================== Test 09 diff ===========================\n"
    diff $GOLD_PATH/t09.gold $OUTPUT_PATH/t09.out
    ;;
10) echo -e "\n=========================== Test 10 diff ===========================\n"
    diff $GOLD_PATH/t10.gold $OUTPUT_PATH/t10.out
    ;;
all) echo -e "\n=========================== Test 01 diff ===========================\n"
     diff $GOLD_PATH/t01.gold $OUTPUT_PATH/t01.out

     echo -e "\n=========================== Test 02 diff ===========================\n"
     diff $GOLD_PATH/t02.gold $OUTPUT_PATH/t02.out

     echo -e "\n=========================== Test 03 diff ===========================\n"
     diff $GOLD_PATH/t03.gold $OUTPUT_PATH/t03.out

     echo -e "\n=========================== Test 04 diff ===========================\n"
     diff $GOLD_PATH/t04.gold $OUTPUT_PATH/t04.out

     echo -e "\n=========================== Test 05 diff ===========================\n"
     diff $GOLD_PATH/t05.gold $OUTPUT_PATH/t05.out

     echo -e "\n=========================== Test 06 diff ===========================\n"
     diff $GOLD_PATH/t06.gold $OUTPUT_PATH/t06.out

     echo -e "\n=========================== Test 07 diff ===========================\n"
     diff $GOLD_PATH/t07.gold $OUTPUT_PATH/t07.out

     echo -e "\n=========================== Test 08 diff ===========================\n"
     diff $GOLD_PATH/t08.gold $OUTPUT_PATH/t08.out

     echo -e "\n=========================== Test 09 diff ===========================\n"
     diff $GOLD_PATH/t09.gold $OUTPUT_PATH/t09.out

     echo -e "\n=========================== Test 10 diff ===========================\n"
     diff $GOLD_PATH/t10.gold $OUTPUT_PATH/t10.out
     ;;
*) echo -e "There is no test '$1'.\n"
   ;;
esac