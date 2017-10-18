#ifndef SYSCALL_COMM_H
#define SYSCALL_COMM_H

#include <stdint.h>

/* Operation codes (read a state, write a state or send a command) */
#define READ_STRING_CODE 1
#define WRITE_CODE 2 // not used
#define EXECUTE_COMMAND_CODE 3
#define REGISTER_EVENT_CODE 4
#define UNREGISTER_EVENT_CODE 5
#define READ_FLOAT_CODE 6
#define READ_INT32_CODE 7

#define COMMUNICATION_EVENT_ID 99

/* State arguments (characteristic)*/
#define STATE_UNITY 0
#define STATE_TYPE 1
#define STATE_MAXIMUM 2
#define STATE_MINIMUM 3
#define STATE_CURRENT_VALUE 4

/********************************************************************
** register_handler(uint8_t event_id, void (*handler)(void), ...)  **
**                                                                 **
** PARAMS:                                                         **
** syscallNumber: the syscall number (16 for a hall call)          **
** code: call argument (1-> read, 2-> write, 3-> statePath)        **
** Command: A pointer to the state in the format                   **
**          "componet.trait.state"                                 **
** characteristic: (Optional) the                               **
**                                                                 **
** paramaters: An optional parameter (e.g. "LEDS" or "TXRT")       **
**                                                                 **
** RETURNS:                                                        **
** The state value or a pointer to it                              **
**                                                                 **
** DESCRIPTION: (TODO)                                             **
** Register 'handler' as a callback to be executed when the event  **
** of id 'event_id' happens                                        **
********************************************************************/
void* readStringState(const char* statePath, int characteristic, char *buffer);
void* readFloatState(const char* statePath, int characteristic, float *buffer);
void* readInt32State(const char* statePath, int characteristic, int32_t *buffer);

// returns 0 for success -1 for unsuccess??
int registerStateChange(void * procedure, const char * statePath);

int unregisterStateChange(void * procedure, const char * statePath);

// returns 0 for success -1 for unsuccess?? //will be implemented in the future
int registerTimmedPolling(void(*procedure)(void), unsigned int intervalMilliseconds);

// returns 0 for success -1 for unsuccess?? //will be implemented in the future
int unregisterTimmedPolling(void(*procedure)(void));

#endif // SYSCALL_COMM_H