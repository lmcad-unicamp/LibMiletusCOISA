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

#include "HAL.h"
#include "vm.h"
#include "serial.h"
//#include "comm.h"
// #include <stdutils.h>
#include <string.h> //TODO purge it from here

#if PRINTING
#include <stdio.h>
#endif

/*Defined peripherals ids:
	See file on doc/CallsDefinition.txt
*/

uint8_t hal_call(uint32_t sensid, char identifier[])//Call to hardware I/O
{
#if PRINTING
	printf("** identifier: %s\nsensid: %u \n", identifier, sensid);
#endif
	uint8_t retval = 0;
	if (!strcmp(identifier,"US_S"))
	{
#if HAS_ULTRASONIC
		switch (sensid)
		{
			case 0: { //Read Ultrasonic
	#if PRINTING
				printf("\n(HAL)Read Ultra called\n");
	#endif // PRINTING
				RF[2] = read_ultrassonic();
				break;
			}
			case 1: {
	#if PRINTING
				printf("\n(HAL)Config Ultra called\n");
	#endif // PRINTING
				configure_ultrassonic(RF[4]);
				break;
			}
			case 2: {
				init_ultrassonic();
				break;
			}
		}
#endif
	}
	else if (!strcmp(identifier,"LEDS"))
	{
#if HAS_LEDS
		switch (sensid)
		{
			case 0: //Led On
				ledon(RF[4]);
				break;
			case 1: //Led Off
				ledoff(RF[4]);
				break;
		}
#endif
	}
	else if (!strcmp(identifier,"ENCD"))
	{
#if HAS_ENCODER
		switch (sensid)
		{
			case 5 : {
	#if PRINTING
				printf("\n(HAL)Read Encoder Count called\n");
	#endif // PRINTING	
				// read_encoder_counter(0);
				break;
			}
			case 6 : {
	#if PRINTING
				printf("\n(HAL)Read Encoder Time called\n");
	#endif // PRINTING	
				break;
			}
		}
#endif
	}
	else if (!strcmp(identifier,"RXTX"))
	{
#if HAS_SERIAL 
		switch (sensid)
		{
			case 10: {
	#if PRINTING
				printf("\n(HAL)Send Byte called\n");
	#endif // PRINTING	
				send_byte(RF[4]);
				break;
			}
			case 11: {
	#if PRINTING
				printf("\n(HAL)Read Byte called\n");
	#endif // PRINTING
				RF[2] = read_byte();	
				break;	
			}
			case 12: {
	#if PRINTING
				printf("\n(HAL)Configure serial called\n");
	#endif // PRINTING	
				serial_configure(RF[4]); //TODO: Allow user to enable and disable interruptions later
				break;
			}
			case 13: {
	#if PRINTING
				printf("\n(HAL)Print number called\n");
	#endif // PRINTING	
				printnum(RF[4]); 
				break;
			}
			case 14: {
	#if PRINTING
				printf("\n(HAL)Print string called\n");
	#endif // PRINTING	
				print((char *)&VM_memory[RF[4]]);
				break;
			}
			case 22: {
	#if PRINTING
				printf("\n(HAL)Print int32 called (syscall_print.s)\n");
	#endif // PRINTING	
				print_int32(RF[4]);
				break;
			}
			case 23: {
	#if PRINTING
				printf("\n(HAL)Print float called (syscall_print.s)\n");
	#endif // PRINTING	
				print_float(RF[4], RF[5]);
				break;
			}
			case 24: {
	#if PRINTING
				printf("\n(HAL)Print double called (syscall_print.s)\n");
	#endif // PRINTING	
				print_double(RF[4], RF[5]);
				break;
			}
			case 25: {
	#if PRINTING
				printf("\n(HAL)Print string called (syscall_print.s)\n");
	#endif // PRINTING	
				print_string((char *)&VM_memory[RF[4]]);
				break;
			}
		}
#endif
	}
	else if (!strcmp(identifier,"MOVM"))
	{
#if HAS_MOTORS
		switch (sensid)
		{
			case 15: {
	#if PRINTING
				printf("\n(HAL)Ahead called\n");
	#endif		
				// ahead();
				break;
			}
			case 16: { 
	#if PRINTING
				printf("\n(HAL)Right called\n");
	#endif		
				// turn_right();
				break;
			}
			case 17: {
	#if PRINTING
				printf("\n(HAL)Left called\n");
	#endif		
				// turn_left();
				break;
			}
			case 18: {
	#if PRINTING
				printf("\n(HAL)Mov. Setup called\n");
	#endif		
				setup_movement();
				break;
			}
			case 20: {
	#if PRINTING
				printf("\n(HAL)PWM called\n");
	#endif		
				break;
			}
			case 21: { //Forward
				reset_variables();
				set_targetRPM_L(80);
				set_targetRPM_R(80);
				break;
			}
			case 22: { //Left
				reset_variables();
				set_targetRPM_L(-80);
				set_targetRPM_R(80);
				break;
			}
			case 23: { //Right
				reset_variables();
				set_targetRPM_L(80);
				set_targetRPM_R(-80);
				break;
			}
			case 24: { //Stop
				reset_variables();
				set_targetRPM_L(0);
				set_targetRPM_R(0);
				break;
			}
		}
#endif
	}
#if HAS_BUTTON
	else if(!strcmp(identifier,"BTOG"))
	{
		// print("BUT\n");
		init_button(RF[4], RF[5]);
	}
#endif
	else
	{
#if PRINTING
			printf("\n(HAL) error - Unknown Hal Call number\n");
#endif	
	}
	return retval;
}
