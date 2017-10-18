#!/bin/bash

arrary_one_time=(4 8 16 32 64 128 256)
arrary_continuous=(4 16 64 256)

for i in "${arrary_one_time[@]}"; do
   ./generate_outlier.sh $i 0
done

for i in "${arrary_continuous[@]}"; do
   ./generate_outlier.sh $i 1
done