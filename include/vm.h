/***************************************************************************
 *   Copyright (C) 2015 by                                                 *
 *   - Carlos Eduardo Millani (carloseduardomillani@gmail.com)             *
 *   - Edson Borin (edson@ic.unicamp.br)                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef VM_H
#define VM_H

#define RETURN_NONE 0
#define RETURN_STRING 1
#define RETURN_FLOAT 2
#define RETURN_INT32 3

#include <stdint.h>
//#include <iostream>
#include <string>

// TODO define PROCESSOR_MEMORY_SZ in a right size
#define PROCESSOR_MEMORY_SZ 1024 * 15 /* 1024k * 15 = 15 Kilobytes */
// #define PROCESSOR_MEMORY_SZ 1024 * 512 /* */
#define VM_MEMORY_SZ PROCESSOR_MEMORY_SZ /* Available memory from the microprocessor  */
/* Register file. */
extern uint32_t RF[32];
extern float RF_float[32];
extern uint32_t hand_addr;
/* Floating-Point Unit condition code flags. */
extern uint8_t fpu_cc;
/* VM memory vector. */
extern uint8_t VM_memory[VM_MEMORY_SZ];

extern uint32_t instruction_count;

typedef union {
  uint32_t as_int32;
  float as_float;
} number_type;

typedef union {
  uint64_t as_int64;
  double as_double;
} double_number_type;

typedef struct comm_call_return {
  std::string component_name;
  std::string trait_name;
  std::string state_name;
} comm_call_return_t;

extern uint32_t comm_call_return_type;
extern comm_call_return_t comm_call_return_id;

#ifndef HEAP_COUNT
#define HEAP_COUNT 1
#endif

#if HEAP_COUNT
extern uint32_t max_heap;
#endif

// extern int32_t* user_int32_buffer;
// extern float* user_float_buffer;
extern char *user_string_buffer;
extern uint32_t user_buffer_addr;

void vm_init(uint32_t newPC);
uint32_t fetch(uint32_t PC);
int8_t vm_cpu(void);
void advance_pc(int32_t offset);
void vm_continue(void);

#endif // VM_H