/**************************************************
 *
 * This communication layer is a Weave wrapper.
 * When the user code executes a comm_call, it will
 * contact the Weave components directly.
 *
 **************************************************/
#ifndef COMM_H
#define COMM_H

#include <stdint.h>

// std::unique_ptr<weave::provider::TaskRunner> my_task_runner_;

uint8_t comm_call(uint32_t code, char parameters[]);

int execute_weave_command(char traitCommand[], char parameters[]);


void on_some_state_changed(const char * component_trait_state_value);

/*void AddCoisaRequestHandler(
      const std::string& path,
      const RequestHandlerCallback& callback) = 0;*/

// weave::CommandQueue* coisa_queue_pointer_;
#endif // COMM_H