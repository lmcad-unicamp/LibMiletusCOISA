#include "THING_EH.h"
#include "serial.h"
#include "syscall_comm.h"

#ifndef QUERY_TYPE
#define QUERY_TYPE 0 // 0 = one-time query, 1 = continuous query
#endif

#ifndef MIN
#define MIN 0.0f
#endif

#ifndef MAX
#define MAX 100.0f
#endif

#ifndef DECIMAL_SEPARATOR
#define DECIMAL_SEPARATOR '.'
#endif

void min_max(float sensor_value)
{
  if (sensor_value < (float)MIN || sensor_value > (float)MAX) {
    print_float((int32_t)&sensor_value, 5);
    print_string("\n");
  }
}

void event_handler()
{
  float sensor_value;

  readFloatState("sensor.sensors.temperature", STATE_CURRENT_VALUE, &sensor_value);

  min_max(sensor_value);

  switch (QUERY_TYPE) {
  case 0:
    unregisterStateChange(event_handler, "sensor.sensors.temperature");
    break;
  case 1:
    break;
  default:
    print_string("ERROR: Invalid query type.\n");
    break;
  }
}

int main(int argc, const char *argv[])
{
  eh_init();

  registerStateChange(event_handler, "sensor.sensors.temperature");

  return 0;
}