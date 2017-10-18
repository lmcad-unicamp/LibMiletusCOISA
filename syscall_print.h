#ifndef SYSCALL_PRINT_H
#define SYSCALL_PRINT_H

#include <stdint.h>

void print_int32(int32_t number);
void print_float(uint32_t float_address, uint32_t precision);
void print_double(uint32_t double_address, uint32_t precision);
void print_string(char *str);

#endif // SYSCALL_PRINT_H