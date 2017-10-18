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
	
#include <stdio.h>
#include "serial.h"
#include "vm.h"

#ifdef ARDUINO
#include <Arduino.h>
#include <SoftwareSerial.h>
extern SoftwareSerial mySerial; // RX, TX
#endif

#ifndef PRINTING_USER
#define PRINTING_USER 0
#endif

void send_byte(unsigned char byte)
{
	printf("%c", byte);
}
char read_byte(void)
{
#if PRINTING_USER
#ifdef ARDUINO
	if (mySerial.available() > 0) {
		// read the incoming byte:
		return mySerial.read();
	}
#else
	char c;
	if (scanf("%c", &c)){
		return c;
	}
#endif
#endif // PRINTING_USER
	return 0;
}
void serial_configure(unsigned int baudrate)
{

}
void enable_commands(void) {
	
}

void printnum(int32_t number)
{
	#if PRINTING_USER
#ifdef ARDUINO
	mySerial.print(number);
#else
	printf("%d", number);
#endif
	#endif //PRINTING_USER
}

void print(char *str)
{
	#if PRINTING_USER
#ifdef ARDUINO
	mySerial.print(str);
#else
	printf("%s", str);
#endif
	#endif //PRINTING_USER
}

// ----------------------------

void print_int32(int32_t number)
{
	#if PRINTING_USER
#ifdef ARDUINO
	mySerial.print(number);
#else
	printf("%d", number);
#endif
	#endif //PRINTING_USER
}

void print_float(uint32_t float_address, uint32_t precision)
{
	#if PRINTING_USER
	number_type num;

	num.as_int32 = ((uint32_t)(VM_memory[float_address + 0]) << 24) |
				   ((uint32_t)(VM_memory[float_address + 1]) << 16) |
				   ((uint32_t)(VM_memory[float_address + 2]) <<  8) |
				   ((uint32_t)(VM_memory[float_address + 3]) <<  0);

#ifdef ARDUINO
	mySerial.print(num.as_float, precision);
#else
	printf(("%." + std::to_string(precision) + "f").c_str(), num.as_float);
#endif
	#endif //PRINTING_USER
}

void print_double(uint32_t double_address, uint32_t precision)
{
	#if PRINTING_USER
	double_number_type num;

	num.as_int64 = ((uint64_t)(VM_memory[double_address + 0]) << 56) |
				   ((uint64_t)(VM_memory[double_address + 1]) << 48) |
				   ((uint64_t)(VM_memory[double_address + 2]) << 40) |
				   ((uint64_t)(VM_memory[double_address + 3]) << 32) |
				   ((uint64_t)(VM_memory[double_address + 4]) << 24) |
				   ((uint64_t)(VM_memory[double_address + 5]) << 16) |
				   ((uint64_t)(VM_memory[double_address + 6]) <<  8) |
				   ((uint64_t)(VM_memory[double_address + 7]) <<  0);

#ifdef ARDUINO
	mySerial.print(num.as_double, precision);
#else
	printf(("%." + std::to_string(precision) + "f").c_str(), num.as_double);
#endif
	#endif //PRINTING_USER
}

void print_string(char *str)
{
	#if PRINTING_USER
#ifdef ARDUINO
	mySerial.print(str);
#else
	printf("%s", str);
#endif
	#endif //PRINTING_USER
}

