#!/bin/bash
EDGENT_JAVA8=~/edgent-1.1.0/java8
CLASS_PATH=$EDGENT_JAVA8/lib/edgent.api.function.jar:$EDGENT_JAVA8/ext/gson-2.2.4.jar:$EDGENT_JAVA8/ext/slf4j-api-1.7.12.jar:$EDGENT_JAVA8/ext/metrics-core-3.1.2.jar:$EDGENT_JAVA8/lib/edgent.api.execution.jar:$EDGENT_JAVA8/lib/edgent.api.window.jar:$EDGENT_JAVA8/lib/edgent.api.oplet.jar:$EDGENT_JAVA8/lib/edgent.api.graph.jar:$EDGENT_JAVA8/lib/edgent.api.topology.jar:$EDGENT_JAVA8/lib/edgent.spi.topology.jar:$EDGENT_JAVA8/lib/edgent.spi.graph.jar:$EDGENT_JAVA8/lib/edgent.runtime.appservice.jar:$EDGENT_JAVA8/lib/edgent.runtime.etiao.jar:$EDGENT_JAVA8/lib/edgent.runtime.jsoncontrol.jar:$EDGENT_JAVA8/lib/edgent.providers.direct.jar:$EDGENT_JAVA8/ext/slf4j-jdk14-1.7.12.jar:$EDGENT_JAVA8/analytics/sensors/lib/edgent.analytics.sensors.jar
INPUT_PATH=../in
GOLD_PATH=../gold
OUTPUT_PATH=out

arrayWindowSize=(4 8 16 32 64 128 256)

IN=../../user_input/256.in
./compile.sh $CLASS_PATH
# for j in "${arrayWindowSize[@]}"; do
#      echo "====================="
#      echo "WindowSize"
#      echo $j
#      echo "====================="
#         for i in seq 1 3; do
#    	       # rm -f $OUTPUT_PATH/t01.out
# 	       # valgrind --tool=massif
#            # time java -Xint -Dfile.encoding=UTF-8 -DFIX_IO_CONTENT="true" -DRANGE="[-15..15]" -DQUERY_TYPE="0" -classpath .:$CLASS_PATH MinMax < $IN > /dev/null # > $OUTPUT_PATH/t01.out
#            # time java -Xint -Dfile.encoding=UTF-8 -DFIX_IO_CONTENT="true" -DWINDOW_SIZE="${j}" -DQUERY_TYPE="0" -classpath .:$CLASS_PATH Outlier < $IN  > /dev/null # > $OUTPUT_PATH/t01.out
#            # time java -Xint -Dfile.encoding=UTF-8 -DFIX_IO_CONTENT="true" -DWINDOW_SIZE="${j}" -DQUERY_TYPE="0" -classpath .:$CLASS_PATH FFT < $IN > /dev/null # > $OUTPUT_PATH/t01.out
# 	       # echo -e "\n=========================== Test 01 diff ===========================\n"
# 	       # diff $GOLD_PATH/t01.gold $OUTPUT_PATH/t01.out
#         done
#  done


for i in seq 1 3; do
   time java -Xint -Dfile.encoding=UTF-8 -DFIX_IO_CONTENT="true" -DRANGE="[-15..15]" -DQUERY_TYPE="0" -classpath .:$CLASS_PATH MinMax < $IN # > /dev/null # > $OUTPUT_PATH/t01.out\
done
