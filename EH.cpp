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

#include "EH.h"
#include "vm.h"
//TODO Jeferson: purge string include from here (or not?)
#include <string.h> 
//#include <stdio.h>


uint32_t ehvec[EHVECSZ] = {0};
uint8_t vec_size = 0;
new_event ehqueue[EHQUEUESZ] = {0};
volatile uint8_t queue_init = 0;
volatile uint8_t queue_size = 0;
volatile uint8_t timer_flag = 0;
ev_point ehvecpointers[EVENTQTTY];

/*// CREATION
volatile unsigned int robot_proximit_threshold = 0;
volatile unsigned int robot_timer_threshold = 0;
volatile unsigned int room_temperature_above_threshold = 0;
volatile unsigned int room_temperature_below_threshold = 0;
volatile unsigned int room_presence_threshold = 0;
volatile int car_hour_threshold1 = -1;
volatile int car_hour_threshold2 = -1;
*/
void eh_init(void)
{
	//initialize timer - HAL WORK
	
	/*Initializes the event descriptors*/
	register uint8_t i = 0;
	for (i = 0; i < EVENTQTTY; i++)
	{
		ehvecpointers[i].id = -1;
		ehvecpointers[i].pos = 0;
		ehvecpointers[i].sz = 0;
	}
	/*---------------------------------*/
}


int8_t register_handler(uint8_t event_id, uint32_t handler, char * evname, void * argv, uint8_t argc)
{

	// printf("\n*** Trying to register the event %s ***\n", evname);
	// printf("\t\t\t\t\t***\t\t\t***register_handler***\n");
	if (!(vec_size < EHVECSZ)){
		// printf("\t\t\t\t\t***\t\t\t!(vec_size < EHVECSZ)\n");
		return -1; //No space for one more handler
	}
	register uint8_t selected;
	for (selected = 0; selected < EVENTQTTY; selected++)
	{
		if (ehvecpointers[selected].id == event_id && !strcmp(ehvecpointers[selected].name,evname)) break; //Selected is now the position of the event on the vector
	}
	if (selected == EVENTQTTY) //Case where there was no break, thus no match was found
	{
		// printf("\t\t\t\t\t***\t\t\tSearches for the first empty space\n");
		for (selected = 0; selected < EVENTQTTY; selected++)//Searches for the first empty space
		{
			// printf("\t\t\t\t\t***\t\t\tSelected is now the position of the first empty slot on the array\n");
			if (ehvecpointers[selected].id == -1) break; //Selected is now the position of the first empty slot on the array
		}
		if (selected == EVENTQTTY){
			// printf("\t\t\t\t\t***\t\t\tNo empty space\n");
			return -1;//No empty space
		}
		// printf("\t\t\t\t\t***\t\t\tmarks the empty space as the new event\n");
		ehvecpointers[selected].id = event_id; //marks the empty space as the new event
		strcpy(ehvecpointers[selected].name, evname);
		for (ehvecpointers[selected].pos = 0; ehvecpointers[selected].pos < EHVECSZ; ehvecpointers[selected].pos++)
		{
			// printf("\t\t\t\t\t***\t\t\tfoooorrr....\n");
			if (ehvec[ehvecpointers[selected].pos] == 0) break;
		}//Gets the first free position for the new event
		
	}
	// printf("****%d\n****%d\n****%d\n****%d\n", selected, ehvecpointers[selected].pos, ehvecpointers[selected].sz, ehvec[ehvecpointers[selected].pos + ehvecpointers[selected].sz]);
	if (ehvec[ehvecpointers[selected].pos + ehvecpointers[selected].sz] == 0) //No handler in the position after the last handler of this event
	{
		// printf("\t\t\t\t\t***\t\t\tNo handler in the position after the last handler of this event\n");
		ehvec[ehvecpointers[selected].pos + ehvecpointers[selected].sz] = handler;
	}
	else //Shift to get space
	{
		// printf("\t\t\t\t\t***\t\t\tShift to get space\n");
		register uint8_t first_zero;
		register uint8_t idnearfz;
		for (first_zero = 0; first_zero < EHVECSZ; first_zero++)
		{
			if (ehvec[first_zero] == 0) break;
		}//find first empty, check which event it is from, then shift... firstzero>pos and <= pos+sz
		if (first_zero < ehvecpointers[selected].pos) // the empty space is before the event, should shift left
		{
			// printf("\t\t\t\t\t***\t\t\tfind first empty\n");
			for (idnearfz = 0; idnearfz < selected; idnearfz++) //To set idnearfz
			{
				if(first_zero < ehvecpointers[idnearfz].pos + ehvecpointers[idnearfz].sz -1) break; //Ensures that the first zero is on that id
			}
			for (;idnearfz <= selected; idnearfz++) //Starts shifting id by id
			{
				ehvec[first_zero] = ehvec[ehvecpointers[idnearfz].pos + ehvecpointers[idnearfz].sz -1];
				ehvec[ehvecpointers[idnearfz].pos + ehvecpointers[idnearfz].sz -1] = 0;
				first_zero = ehvecpointers[idnearfz].pos + ehvecpointers[idnearfz].sz -1;
				ehvecpointers[idnearfz].pos--;
			}
			ehvec[ehvecpointers[selected].pos + ehvecpointers[selected].sz] = handler;
		}
		else if (first_zero > ehvecpointers[selected].pos + ehvecpointers[selected].sz)
		{
			// printf("\t\t\t\t\t***\t\t\tAqui\n");
			for (idnearfz = EVENTQTTY-1; idnearfz > selected; idnearfz--) //To set idnearfz
			{
				if (ehvecpointers[idnearfz].id == -1) continue; //Position not used
				if(first_zero > ehvecpointers[idnearfz].pos + ehvecpointers[idnearfz].sz -1) break; //Ensures that the first zero is on that id
			}
			for (;idnearfz > selected; idnearfz--) //Starts shifting id by id
			{
				ehvec[first_zero] = ehvec[ehvecpointers[idnearfz].pos];
				ehvec[ehvecpointers[idnearfz].pos] = 0;
				first_zero = ehvecpointers[idnearfz].pos;
				ehvecpointers[idnearfz].pos++;
			}
			ehvec[ehvecpointers[selected].pos + ehvecpointers[selected].sz] = handler;
		}
		else //Should never get here
		{
			// printf("\t\t\t\t\t***\t\t\tShould never get here\n");
			return -3;
		}
	}
	
#if HAS_ULTRASONIC
	// print("Uhm\n");
	if (!strcmp(evname, "US_S")) //US dist sensor has the threshold argument
	{
		// print("IuhullUM\n");
		if (argc == 1)
		{
			// printnum(((int*)argv)[0]);
			// print("<<Iuhull\n");
			us_threshold = ((int*)argv)[0];
		}
	}
#endif

/*	// CREATION
	if (!strcmp(evname, "ROPR")) //US dist sensor has the threshold argument
	{
		printf("Limiar (robot_proximit_threshold) set to %d\n", (uint8_t)RF[7]); // threshold is in $a3
		robot_proximit_threshold = (uint8_t)RF[7];
	}
	else if (!strcmp(evname, "RESE")) //US dist sensor has the threshold argument
	{
		printf("Limiar (robot_timer_threshold) set to %d\n", (uint8_t)RF[7]); // threshold is in $a3
		robot_timer_threshold = (uint8_t)RF[7];
	}
	else if (!strcmp(evname, "ROTA")) //room temperature Above
	{
		printf("Limiar (room_temperature_above_threshold) set to %d\n", (uint8_t)RF[7]); // threshold is in $a3
		room_temperature_above_threshold = (uint8_t)RF[7];
	}
	else if (!strcmp(evname, "ROTB")) //room below
	{
		printf("Limiar (room_temperature_below_threshold) set to %d\n", (uint8_t)RF[7]); // threshold is in $a3
		 room_temperature_below_threshold= (uint8_t)RF[7];
	}
	else if (!strcmp(evname, "ROPY")) //room presence
	{
		room_presence_threshold |= 1;
		printf("Limiar (room_presence_threshold) set to %d\n", room_presence_threshold); // threshold is defined here
	}
	else if (!strcmp(evname, "ROPN")) //room presence
	{
		room_presence_threshold |= 2;
		printf("Limiar (room_presence_threshold) set to %d\n", room_presence_threshold); // threshold is defined here
	}
	else if (!strcmp(evname, "CAH1")) //Car on Hour X1
	{
		car_hour_threshold1 = (uint8_t)RF[7];
		printf("Limiar (car_hour_threshold1) set to %d\n", car_hour_threshold1); // threshold is in $a3
	}

	else if (!strcmp(evname, "CAH2")) //Car on Hour X2
	{
		car_hour_threshold2 = (uint8_t)RF[7];
		printf("Limiar (car_hour_threshold2) set to %d\n", car_hour_threshold2); // threshold is in $a3
	}*/
	
	//TODO:Remember to tell HAL to generate events! -- Currently it always generate events
	
	ehvecpointers[selected].sz++;
	vec_size++;
	//printf("*** register_handler %s registrado com sucesso ***\n", evname);
	return 1;
}

int8_t remove_handler(uint8_t event_id, uint32_t handler, char * evname)
{
	register int8_t selected;
	for (selected = 0; selected < EVENTQTTY; selected++)
	{
		if (ehvecpointers[selected].id == event_id && !strcmp(ehvecpointers[selected].name,evname)) break; //Selected is now the position of the event on the vector
	}
	if (selected == EVENTQTTY) return -1; //No such event

	register int8_t event;
	for (event = ehvecpointers[selected].pos; event < ehvecpointers[selected].sz; event++)
	{
		if (ehvec[event] == handler) break;
	}
	if (ehvec[event] != handler) return -1; //No such handler

	ehvec[event] = ehvec[ehvecpointers[selected].pos + ehvecpointers[selected].sz -1]; //Make sure there won`t be a gap in the vector
	ehvec[ehvecpointers[selected].pos + ehvecpointers[selected].sz - 1] = 0;

	if (--ehvecpointers[selected].sz == 0) //Became empty
	{
		ehvecpointers[selected].id = -1;
		ehvecpointers[selected].pos = 0;
	}

	//TODO:VERIFY HAL after removing, should stop generating events?

	vec_size--;
	return 1;
}

void event_timer(void)
{
	timer_flag = 1; //Set flag to true
}

int8_t insert_event(uint8_t event_id, char * evname)
{
	if (queue_size < EHQUEUESZ) //Still has space
	{
		// printf("************\t\t\tStill has space (insert_event)\n");
		ehqueue[(queue_init + queue_size) % EHQUEUESZ].id = event_id; //Its a vector list
		strcpy(ehqueue[(queue_init + queue_size) % EHQUEUESZ].name, evname);
		queue_size++;
		return 1; //Success
	}
	else {
		//printf("NO SPACE on insert_event!!\n");
		return -1; // No space, returns error
	}
}

int8_t consume_event(void) //TODO:For some reason i cannot print from inside this func
{
	// while
	if (queue_size > 0) //Has something
	{
		// printf("\nqueue_size: %d\n", queue_size);
		// printf("\nqueue_size > 0\n");
		register int8_t selected;
		{ //Block used to scope the event variable
			new_event event = ehqueue[queue_init];
			queue_size--;
			queue_init = (queue_init+1)%(EHQUEUESZ); //Pops the first value and updates

			//Gets the event pointer to
			for (selected = 0; selected < EVENTQTTY; selected++)
			{
				// printf("ehvecpointers[selected].name: %s\n", ehvecpointers[selected].name );
				if (!strcmp(ehvecpointers[selected].name,event.name)) break; //Selected is now the position of the event on the vector
			}
			// printf("*****One: %s\n", ehvecpointers[selected].name);
			// printf("*****Tow: %s\n", event.name);
			if (strcmp(ehvecpointers[selected].name,event.name) ){
				// printf("\nstrcmp(ehvecpointers[selected].name,event.name)\n");
				return -1; //No event with that id found -- ERROR
			}
		}
		// printf("Consuming the event: %s\n", ehvecpointers[selected].name);
		register  uint8_t loop;
		for (loop = ehvecpointers[selected].pos; loop < ehvecpointers[selected].pos + ehvecpointers[selected].sz; loop++)
		{
			RF[4] = ehvec[loop];
			vm_init(hand_addr);
			vm_cpu();
		}
		// printf("Event %s consumed.\n", ehvecpointers[selected].name);
		return 1; // Success
	}
	// printf(".\n");
	return -1; //Nothing to Consume
}

void timed_polling(void)
{
#if HAS_ULTRASONIC
	// printnum(read_ultrassonic());
	// print("<<\n");
	uint8_t dist = read_ultrassonic();
	if (dist < us_threshold - 1) //Hysteresis
	{
		if (!is_close)
		{
			is_close = 1;
			insert_event(1,"US_S");
		}
	}
	else if (dist > us_threshold + 1)
	{
		if (is_close)
		{
			is_close = 0;
			insert_event(1,"US_F");
		}
	}
#endif //HAS_ULTRASONIC
#if HAS_ENCODER
	uint8_t count = read_encoder_counter(RIGHT);
#endif // HAS_ENCODER
}
