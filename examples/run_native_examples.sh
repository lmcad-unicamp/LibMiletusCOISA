#!/bin/bash

programs=(fft outlier)

arrayWindowSize=(4 16 64 256)
arrayInputSize=(16 256 4096 65536)


echo "========== FFT ==========="
for wind in "${arrayWindowSize[@]}"; do
     echo "====================="
     echo "windows Size"
     echo $wind
     echo "====================="
     for j in "${arrayInputSize[@]}"; do
         echo "******************"
         echo "Input Size"
         echo $j
         echo "******************"
        for i in seq 1 3; do
     	   for i in `seq 1 10`; do time ./fft ${wind} ${j}; done 2>&1 | grep real | cut -dm -f2 | sort -n | head -n 15 | tail -n 1
        done
    done
 done

echo "========== outlier ==========="
for wind in "${arrayWindowSize[@]}"; do
     echo "====================="
     echo "windows Size"
     echo $wind
     echo "====================="
     for j in "${arrayInputSize[@]}"; do
         echo "******************"
         echo "Input Size"
         echo $j
         echo "******************"
        for i in seq 1 3; do
     	   for i in `seq 1 10`; do time ./outlier ${wind} ${j}; done 2>&1 | grep real | cut -dm -f2 | sort -n | head -n 15 | tail -n 1
        done
    done
 done


echo "========== min_max ==========="
for j in "${arrayInputSize[@]}"; do
	echo "******************"
	echo "Input Size"
	echo $j
	echo "******************"
	for i in seq 1 3; do
		for i in `seq 1 10`; do time ./min_max ${j}; done 2>&1 | grep real | cut -dm -f2 | sort -n | head -n 15 | tail -n 1
	done
done

