#!/bin/bash

MIPS_C_TEST_CASES_PATH=../../../MipsCTestCases
MIPS_COMPILER_PATH=~/ellcc
CROSS_COMPILER_PATH=/opt/cross/bin

rm -f *.o *.s *.x *.bin

$MIPS_COMPILER_PATH/bin/ecc -target mips-linux-eng \
                            -S fft.c \
                            -D WINDOW_SIZE=$1 -D QUERY_TYPE=$2 \
                            -I$MIPS_COMPILER_PATH/libecc/clang/ \
                            -I$MIPS_C_TEST_CASES_PATH/extras/lib/ \
                            -I../../../ \
                            -O3 -nostdlib -fno-exceptions -fno-rtti -static -fno-builtin -nostdinc

$CROSS_COMPILER_PATH/mips-as $MIPS_C_TEST_CASES_PATH/extras/crt0.s \
                             $MIPS_C_TEST_CASES_PATH/extras/lib/THING_ultrasonic.s \
                             $MIPS_C_TEST_CASES_PATH/extras/lib/THING_serial.s \
                             $MIPS_C_TEST_CASES_PATH/extras/lib/THING_encoder.s \
                             $MIPS_C_TEST_CASES_PATH/extras/lib/THING_movement.s \
                             $MIPS_C_TEST_CASES_PATH/extras/lib/THING_EH.s \
                             $MIPS_C_TEST_CASES_PATH/extras/lib/THING_button.s \
                             ../../../syscall_comm.s \
                             ../../../syscall_print.s \
                             fft.s \
                             -o fft.o

$CROSS_COMPILER_PATH/mips-ld -T $MIPS_C_TEST_CASES_PATH/linker.ld fft.o -o fft.bin

$CROSS_COMPILER_PATH/mips-objcopy -O binary fft.bin x/fft_${1}_${2}.x

rm -f *.o *.s *.x *.bin