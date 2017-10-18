#!/bin/bash

arrary_one_time_min=(-15)
arrary_one_time_max=(15)
one_time_length=${#arrary_one_time_min[@]}

arrary_continuous_min=(-15)
arrary_continuous_max=(15)
continuous_length=${#arrary_continuous_min[@]}

for (( i=0; i<$one_time_length; i++ )); do
   ./generate_min_max.sh ${arrary_one_time_min[$i]} ${arrary_one_time_max[$i]} 0
done

for (( i=0; i<$continuous_length; i++ )); do
   ./generate_min_max.sh ${arrary_continuous_min[$i]} ${arrary_continuous_max[$i]} 1
done