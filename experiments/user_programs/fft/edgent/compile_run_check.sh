#!/bin/bash
EDGENT_JAVA8=~/edgent-1.1.0/java8
CLASS_PATH=$EDGENT_JAVA8/lib/edgent.api.function.jar:$EDGENT_JAVA8/ext/gson-2.2.4.jar:$EDGENT_JAVA8/ext/slf4j-api-1.7.12.jar:$EDGENT_JAVA8/ext/metrics-core-3.1.2.jar:$EDGENT_JAVA8/lib/edgent.api.execution.jar:$EDGENT_JAVA8/lib/edgent.api.window.jar:$EDGENT_JAVA8/lib/edgent.api.oplet.jar:$EDGENT_JAVA8/lib/edgent.api.graph.jar:$EDGENT_JAVA8/lib/edgent.api.topology.jar:$EDGENT_JAVA8/lib/edgent.spi.topology.jar:$EDGENT_JAVA8/lib/edgent.spi.graph.jar:$EDGENT_JAVA8/lib/edgent.runtime.appservice.jar:$EDGENT_JAVA8/lib/edgent.runtime.etiao.jar:$EDGENT_JAVA8/lib/edgent.runtime.jsoncontrol.jar:$EDGENT_JAVA8/lib/edgent.providers.direct.jar:$EDGENT_JAVA8/ext/slf4j-jdk14-1.7.12.jar:$EDGENT_JAVA8/analytics/sensors/lib/edgent.analytics.sensors.jar
INPUT_PATH=../in
GOLD_PATH=../gold
OUTPUT_PATH=out

case "$1" in

1)  ./compile.sh $CLASS_PATH
    rm -f $OUTPUT_PATH/t01.out
    java -Dfile.encoding=UTF-8 -DFIX_IO_CONTENT="true" -DWINDOW_SIZE="8" -DQUERY_TYPE="0" -classpath .:$CLASS_PATH FFT < $INPUT_PATH/t01.in > $OUTPUT_PATH/t01.out
    echo -e "\n=========================== Test 01 diff ===========================\n"
    diff $GOLD_PATH/t01.gold $OUTPUT_PATH/t01.out
    ;;
2)  ./compile.sh $CLASS_PATH
    rm -f $OUTPUT_PATH/t02.out
    java -Dfile.encoding=UTF-8 -DFIX_IO_CONTENT="true" -DWINDOW_SIZE="32" -DQUERY_TYPE="0" -classpath .:$CLASS_PATH FFT < $INPUT_PATH/t02.in > $OUTPUT_PATH/t02.out
    echo -e "\n=========================== Test 02 diff ===========================\n"
    diff $GOLD_PATH/t02.gold $OUTPUT_PATH/t02.out
    ;;
all) rm -f $OUTPUT_PATH/*.out
     ./compile.sh $CLASS_PATH
     java -Dfile.encoding=UTF-8 -DFIX_IO_CONTENT="true" -DWINDOW_SIZE="8" -DQUERY_TYPE="0" -classpath .:$CLASS_PATH FFT < $INPUT_PATH/t01.in > $OUTPUT_PATH/t01.out

     ./compile.sh $CLASS_PATH
     java -Dfile.encoding=UTF-8 -DFIX_IO_CONTENT="true" -DWINDOW_SIZE="32" -DQUERY_TYPE="0" -classpath .:$CLASS_PATH FFT < $INPUT_PATH/t02.in > $OUTPUT_PATH/t02.out

     echo -e "\n=========================== Test 01 diff ===========================\n"
     diff $GOLD_PATH/t01.gold $OUTPUT_PATH/t01.out

     echo -e "\n=========================== Test 02 diff ===========================\n"
     diff $GOLD_PATH/t02.gold $OUTPUT_PATH/t02.out
    ;;
*) echo -e "There is no test '$1'.\n"
   ;;
esac