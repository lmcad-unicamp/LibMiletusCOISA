#!/bin/bash
EDGENT_JAVA8=~/edgent-1.1.0/java8
CLASS_PATH=$EDGENT_JAVA8/lib/edgent.api.function.jar:$EDGENT_JAVA8/ext/gson-2.2.4.jar:$EDGENT_JAVA8/ext/slf4j-api-1.7.12.jar:$EDGENT_JAVA8/ext/metrics-core-3.1.2.jar:$EDGENT_JAVA8/lib/edgent.api.execution.jar:$EDGENT_JAVA8/lib/edgent.api.window.jar:$EDGENT_JAVA8/lib/edgent.api.oplet.jar:$EDGENT_JAVA8/lib/edgent.api.graph.jar:$EDGENT_JAVA8/lib/edgent.api.topology.jar:$EDGENT_JAVA8/lib/edgent.spi.topology.jar:$EDGENT_JAVA8/lib/edgent.spi.graph.jar:$EDGENT_JAVA8/lib/edgent.runtime.appservice.jar:$EDGENT_JAVA8/lib/edgent.runtime.etiao.jar:$EDGENT_JAVA8/lib/edgent.runtime.jsoncontrol.jar:$EDGENT_JAVA8/lib/edgent.providers.direct.jar:$EDGENT_JAVA8/ext/slf4j-jdk14-1.7.12.jar:$EDGENT_JAVA8/analytics/sensors/lib/edgent.analytics.sensors.jar

INPUT_PATH=../in
GOLD_PATH=../gold
OUTPUT_PATH=out

case "$1" in

1)  ./compile.sh $CLASS_PATH
    rm -f $OUTPUT_PATH/t01.out
    java -Dfile.encoding=UTF-8 -DFIX_IO_CONTENT="true" -DWINDOW_SIZE="6" -DQUERY_TYPE="0" -classpath .:$CLASS_PATH Outlier < $INPUT_PATH/t01.in > $OUTPUT_PATH/t01.out
    echo -e "\n=========================== Test 01 diff ===========================\n"
    diff $GOLD_PATH/t01.gold $OUTPUT_PATH/t01.out
    ;;
2)  ./compile.sh $CLASS_PATH
    rm -f $OUTPUT_PATH/t02.out
    java -Dfile.encoding=UTF-8 -DFIX_IO_CONTENT="true" -DWINDOW_SIZE="8" -DQUERY_TYPE="0" -classpath .:$CLASS_PATH Outlier < $INPUT_PATH/t02.in > $OUTPUT_PATH/t02.out
    echo -e "\n=========================== Test 02 diff ===========================\n"
    diff $GOLD_PATH/t02.gold $OUTPUT_PATH/t02.out
    ;;
3)  ./compile.sh $CLASS_PATH
    rm -f $OUTPUT_PATH/t03.out
    java -Dfile.encoding=UTF-8 -DFIX_IO_CONTENT="true" -DWINDOW_SIZE="90" -DQUERY_TYPE="0" -classpath .:$CLASS_PATH Outlier < $INPUT_PATH/t03.in > $OUTPUT_PATH/t03.out
    echo -e "\n=========================== Test 03 diff ===========================\n"
    diff $GOLD_PATH/t03.gold $OUTPUT_PATH/t03.out
    ;;
4) ./compile.sh $CLASS_PATH
    rm -f $OUTPUT_PATH/t04.out
    java -Dfile.encoding=UTF-8 -DFIX_IO_CONTENT="true" -DWINDOW_SIZE="12" -DQUERY_TYPE="0" -classpath .:$CLASS_PATH Outlier < $INPUT_PATH/t04.in > $OUTPUT_PATH/t04.out
    echo -e "\n=========================== Test 04 diff ===========================\n"
    diff $GOLD_PATH/t04.gold $OUTPUT_PATH/t04.out
    ;;
5) ./compile.sh $CLASS_PATH
    rm -f $OUTPUT_PATH/t05.out
    java -Dfile.encoding=UTF-8 -DFIX_IO_CONTENT="true" -DWINDOW_SIZE="6" -DQUERY_TYPE="1" -classpath .:$CLASS_PATH Outlier < $INPUT_PATH/t05.in > $OUTPUT_PATH/t05.out
    echo -e "\n=========================== Test 05 diff ===========================\n"
    diff $GOLD_PATH/t05.gold $OUTPUT_PATH/t05.out
    ;;
6) ./compile.sh $CLASS_PATH
    rm -f $OUTPUT_PATH/t06.out
    java -Dfile.encoding=UTF-8 -DFIX_IO_CONTENT="true" -DWINDOW_SIZE="20" -DQUERY_TYPE="0" -classpath .:$CLASS_PATH Outlier < $INPUT_PATH/t06.in > $OUTPUT_PATH/t06.out
    echo -e "\n=========================== Test 06 diff ===========================\n"
    diff $GOLD_PATH/t06.gold $OUTPUT_PATH/t06.out
    ;;
7) ./compile.sh $CLASS_PATH
    rm -f $OUTPUT_PATH/t07.out
    java -Dfile.encoding=UTF-8 -DFIX_IO_CONTENT="true" -DWINDOW_SIZE="21" -DQUERY_TYPE="0" -classpath .:$CLASS_PATH Outlier < $INPUT_PATH/t07.in > $OUTPUT_PATH/t07.out
    echo -e "\n=========================== Test 07 diff ===========================\n"
    diff $GOLD_PATH/t07.gold $OUTPUT_PATH/t07.out
    ;;
8) ./compile.sh $CLASS_PATH
    rm -f $OUTPUT_PATH/t08.out
    java -Dfile.encoding=UTF-8 -DFIX_IO_CONTENT="true" -DWINDOW_SIZE="15" -DQUERY_TYPE="0" -classpath .:$CLASS_PATH Outlier < $INPUT_PATH/t08.in > $OUTPUT_PATH/t08.out
    echo -e "\n=========================== Test 08 diff ===========================\n"
    diff $GOLD_PATH/t08.gold $OUTPUT_PATH/t08.out
    ;;
9) ./compile.sh $CLASS_PATH
    rm -f $OUTPUT_PATH/t09.out
    java -Dfile.encoding=UTF-8 -DFIX_IO_CONTENT="true" -DWINDOW_SIZE="19" -DQUERY_TYPE="0" -classpath .:$CLASS_PATH Outlier < $INPUT_PATH/t09.in > $OUTPUT_PATH/t09.out
    echo -e "\n=========================== Test 09 diff ===========================\n"
    diff $GOLD_PATH/t09.gold $OUTPUT_PATH/t09.out
    ;;
10) ./compile.sh $CLASS_PATH
    rm -f $OUTPUT_PATH/t10.out
    java -Dfile.encoding=UTF-8 -DFIX_IO_CONTENT="true" -DWINDOW_SIZE="15" -DQUERY_TYPE="1" -classpath .:$CLASS_PATH Outlier < $INPUT_PATH/t10.in > $OUTPUT_PATH/t10.out
    echo -e "\n=========================== Test 10 diff ===========================\n"
    diff $GOLD_PATH/t10.gold $OUTPUT_PATH/t10.out
    ;;
all) rm -f $OUTPUT_PATH/*.out
     ./compile.sh $CLASS_PATH
     java -Dfile.encoding=UTF-8 -DFIX_IO_CONTENT="true" -DWINDOW_SIZE="6" -DQUERY_TYPE="0" -classpath .:$CLASS_PATH Outlier < $INPUT_PATH/t01.in > $OUTPUT_PATH/t01.out

     ./compile.sh $CLASS_PATH
     java -Dfile.encoding=UTF-8 -DFIX_IO_CONTENT="true" -DWINDOW_SIZE="8" -DQUERY_TYPE="0" -classpath .:$CLASS_PATH Outlier < $INPUT_PATH/t02.in > $OUTPUT_PATH/t02.out

     ./compile.sh $CLASS_PATH
     java -Dfile.encoding=UTF-8 -DFIX_IO_CONTENT="true" -DWINDOW_SIZE="90" -DQUERY_TYPE="0" -classpath .:$CLASS_PATH Outlier < $INPUT_PATH/t03.in > $OUTPUT_PATH/t03.out

     ./compile.sh $CLASS_PATH
     java -Dfile.encoding=UTF-8 -DFIX_IO_CONTENT="true" -DWINDOW_SIZE="12" -DQUERY_TYPE="0" -classpath .:$CLASS_PATH Outlier < $INPUT_PATH/t04.in > $OUTPUT_PATH/t04.out

     ./compile.sh $CLASS_PATH
     java -Dfile.encoding=UTF-8 -DFIX_IO_CONTENT="true" -DWINDOW_SIZE="6" -DQUERY_TYPE="1" -classpath .:$CLASS_PATH Outlier < $INPUT_PATH/t05.in > $OUTPUT_PATH/t05.out

     ./compile.sh $CLASS_PATH
     java -Dfile.encoding=UTF-8 -DFIX_IO_CONTENT="true" -DWINDOW_SIZE="20" -DQUERY_TYPE="0" -classpath .:$CLASS_PATH Outlier < $INPUT_PATH/t06.in > $OUTPUT_PATH/t06.out

     ./compile.sh $CLASS_PATH
     java -Dfile.encoding=UTF-8 -DFIX_IO_CONTENT="true" -DWINDOW_SIZE="21" -DQUERY_TYPE="0" -classpath .:$CLASS_PATH Outlier < $INPUT_PATH/t07.in > $OUTPUT_PATH/t07.out

     ./compile.sh $CLASS_PATH
     java -Dfile.encoding=UTF-8 -DFIX_IO_CONTENT="true" -DWINDOW_SIZE="15" -DQUERY_TYPE="0" -classpath .:$CLASS_PATH Outlier < $INPUT_PATH/t08.in > $OUTPUT_PATH/t08.out

     ./compile.sh $CLASS_PATH
     java -Dfile.encoding=UTF-8 -DFIX_IO_CONTENT="true" -DWINDOW_SIZE="19" -DQUERY_TYPE="0" -classpath .:$CLASS_PATH Outlier < $INPUT_PATH/t09.in > $OUTPUT_PATH/t09.out

     ./compile.sh $CLASS_PATH
     java -Dfile.encoding=UTF-8 -DFIX_IO_CONTENT="true" -DWINDOW_SIZE="15" -DQUERY_TYPE="1" -classpath .:$CLASS_PATH Outlier < $INPUT_PATH/t10.in > $OUTPUT_PATH/t10.out

     echo -e "\n=========================== Test 01 diff ===========================\n"
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