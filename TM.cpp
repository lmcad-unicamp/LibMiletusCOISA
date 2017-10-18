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

#include "TM.h"
#include "EH.h"
#include "vm.h"

//#include <stdio.h>
//#include <stdint.h>
//#include <time.h>

//#include <iostream>
//#include <fstream>
//#include <sstream>
//#include <string>
#include <cstring>

#include "base64.h"

#include "libMiletus.h"

uint32_t tm_counter = 0;
uint16_t tot_size = 0;

uint32_t vm_memory_used_size = 0;


// char receivingString(const std::string &fileB64Encoded);
void Executing(void);
void Reseting(void);

void (*state)(void);

ThingMonitor::ThingMonitor()
{
  eh_init();
}

bool ThingMonitor::InstallCode(const std::string &fileB64Encoded) {
  // TODO check if the size of binary fit in memory VM_MEMORY_SZ
  // TODO split memory for each program

  // Use https://docs.python.org/2/library/base64.html in python side
  if (vm_memory_used_size == 0){
#ifdef MEASURING
      global_vm_cycle_count_with_syscall = 0;
      global_vm_cycle_count_without_syscall = 0;
      iterations = 0;
#endif
      uint32_t i;
      for (i = 0; i < VM_MEMORY_SZ; i++) {
        VM_memory[i] = 0;
      }
      instruction_count = 0;
  }

#ifdef MEASURING
#ifdef ARDUINO
  Serial.printf("We are MEASURING!!");
#endif
#endif

  if (fileB64Encoded.empty()) {
    return false;
  }

  std::string decodedFile = base64_decode(fileB64Encoded);

  size_t programSize = decodedFile.size();

  if (programSize >= VM_MEMORY_SZ) {
    return false;
  }

  std::memcpy(VM_memory+vm_memory_used_size, decodedFile.c_str(), programSize);

  vm_memory_used_size += programSize;

  vm_init(0);

  return true;
}

void ThingMonitor::RunCode(void){ // TODO check if it is not used
  //std::cout << "Running code" << std::endl;
  while (!vm_cpu());
  /*if (!vm_cpu()){
    task_runner_->PostDelayedTask(
          FROM_HERE,
          base::Bind(&ThingMonitor::RunCode, weak_ptr_factory_.GetWeakPtr()),
          base::TimeDelta::FromSeconds(30));
  }*/
  //std::cout << "Running code, done!!" << std::endl;
}

bool ThingMonitor::ExecuteStep(void) {
  return vm_cpu();
}

void ThingMonitor::Reseting(void) {
  vm_memory_used_size = 0;
  uint32_t i;
  for (i = 0; i < VM_MEMORY_SZ; i++) {
    VM_memory[i] = 0;
  }
  // state = idle;
  // TODO
}

void ThingMonitor::Restart(void) {
  vm_memory_used_size = 0;
}

//void ThingMonitor::Tm_init(void) {
//  /*COISA's Initialization*/
//  eh_init();
//  /*Everything initialized*/
//}


/*void ThingMonitor::Loop(void){
  // TODO: Purge this loop
  // TODO: implement timer interruptions somewere
  if(timer_flag)
  { 
    tm_counter++;
    timed_polling();
    timer_flag = 0;
    // if (state == idle) //Doesn't interrupts other functions - All funcs must be non blocking
    // {
    //  state = executing; //TODO: do i really need to set this here, can iterate for nothing :(
      consume_event();
    // }
  }
  state();
}*/




