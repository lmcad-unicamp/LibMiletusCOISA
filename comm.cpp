#include "comm.h"
#include "vm.h"
#include "syscall_comm.h"
#include "EH.h"

//#include <string.h>
//#include <stdint.h>
//#include <stdio.h>

//#include <unistd.h>
//#include <sys/wait.h>

#ifdef ARDUINO
#include <Arduino.h>
#endif

/*****************************************************************************
 * By executing the function comm_call, the user can:
 *
 *   1) Retrive Weave components
 *      code = 1
 *      parameters = NULL
 *      returns in RF[2] a char* to the response in JSON format
 *
 *   2) Retrive Weave traits
 *      code = 2
 *      parameters = NULL
 *      returns in RF[2] a char* to the response in JSON format
 *
 *   3) Invoke a Weave command
 *      code = 3
 *      parameters = command_name,name_param1,value_param1,name_param2,value_param2...
 *      or, if there are no command parameters,
 *      parameters = command_name
 *      returns in RF[2] a char* to the response in JSON format
 *****************************************************************************/

uint8_t comm_call(uint32_t code, char parameters[]) {
  switch(code) {
    case 1: { // Retrive Weave components
        std::string raw_command = (char *)&VM_memory[RF[4]];

        size_t first_dot = raw_command.find(".");
        size_t second_dot = raw_command.find(".", first_dot+1);

        comm_call_return_id.component_name = raw_command.substr(0, first_dot);
        comm_call_return_id.trait_name = raw_command.substr(first_dot+1, second_dot-first_dot-1);
        comm_call_return_id.state_name = raw_command.substr(second_dot+1);

        user_string_buffer = (char *)&VM_memory[RF[6]];

        RF[2] = 0;
        comm_call_return_type = RETURN_STRING;

        break;
    }  
    case 2: { // Retrive Weave traits
        // std::cout << "\n\nreturned: " << execute_weave_command((char*)"onOff.setConfig", (char*)"state=on") << std::endl;
        break;
    }
    case 3: { // Invoke a Weave command
              // RF[4] contains the string component.trait, command.arg0
              // RF[5] contains the value in string format
        std::string raw_command = (char *)&VM_memory[RF[4]];

        size_t first_dot = raw_command.find(".");
        size_t second_dot = raw_command.find(".", first_dot+1);
        size_t third_dot = raw_command.find(".", second_dot+1);

        std::string trait_command  = raw_command.substr(first_dot +1, third_dot-first_dot-1);

        std::string argument_value = raw_command.substr(third_dot+1);
        argument_value += "=";
#ifdef ARDUINO
        argument_value += String((uint8_t)RF[7]).c_str(); // value is in $a3
#else
        argument_value += std::to_string((uint8_t)RF[7]); // value is in $a3
#endif
        break;
    }
    case 6: {
        std::string raw_command = (char *)&VM_memory[RF[4]];
        size_t first_dot = raw_command.find(".");
        size_t second_dot = raw_command.find(".", first_dot+1);

        comm_call_return_id.component_name = raw_command.substr(0, first_dot);
        comm_call_return_id.trait_name = raw_command.substr(first_dot+1, second_dot-first_dot-1);
        comm_call_return_id.state_name = raw_command.substr(second_dot+1);

        //user_float_buffer = (float *)(&VM_memory[RF[6]]);

        user_buffer_addr = RF[6];

        RF[2] = 0;
        comm_call_return_type = RETURN_FLOAT;
      break;
    }
    case 7: {
        std::string raw_command = (char *)&VM_memory[RF[4]];
        size_t first_dot = raw_command.find(".");
        size_t second_dot = raw_command.find(".", first_dot+1);

        comm_call_return_id.component_name = raw_command.substr(0, first_dot);
        comm_call_return_id.trait_name = raw_command.substr(first_dot+1, second_dot-first_dot-1);
        comm_call_return_id.state_name = raw_command.substr(second_dot+1);

        //user_int32_buffer = (int32_t *)(&VM_memory[RF[6]]);

        user_buffer_addr = RF[6];

        RF[2] = 0;
        comm_call_return_type = RETURN_INT32;
      break;
    }
    default:
      RF[2] = (uint8_t) NULL;
      break;
  }

  //if (!strcmp(parameters,"LEDS")) {}

  return 0;
}


void on_some_state_changed(const char * component_trait_state_value){
  std::string raw_state = component_trait_state_value;
  size_t equal_pos = raw_state.find("=");
  std::string component_trait_state = raw_state.substr(0, equal_pos);
  insert_event(COMMUNICATION_EVENT_ID, (char*)component_trait_state.c_str()); // inser an state change event
}