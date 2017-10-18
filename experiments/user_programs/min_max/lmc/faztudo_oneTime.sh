#!/bin/bash

killall linux_example_wifi
for X in $(ls x/*_0.x); do
     echo $X
     for i in seq 1 3; do
         ../../../../bin/linux_example_wifi &> .tmp &
         ../../../../scripts/send_coisa.py $X \
            /home/linaro/libMiletusCoisa/user_programs/fft/in/t01.in \
            /home/linaro/libMiletusCoisa/user_programs/fft/lmc/out/t01.out 127.0.0.1 > /dev/null
         wait
         cat .tmp | tail -n 7 | head -n 1 | cut -d':' -f2  
     done
 done

