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
all) echo -e "\n=========================== Test 01 diff ===========================\n"
     diff $GOLD_PATH/t01.gold $OUTPUT_PATH/t01.out

     echo -e "\n=========================== Test 02 diff ===========================\n"
     diff $GOLD_PATH/t02.gold $OUTPUT_PATH/t02.out
     ;;
*) echo -e "There is no test '$1'.\n"
   ;;
esac