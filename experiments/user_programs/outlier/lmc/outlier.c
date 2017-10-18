#include "THING_EH.h"
#include "serial.h"
#include "syscall_comm.h"

#ifndef QUERY_TYPE
#define QUERY_TYPE 0 // 0 = one-time query, 1 = continuous query
#endif

#ifndef WINDOW_SIZE
#define WINDOW_SIZE 10
#endif

int window_counter;
float values[WINDOW_SIZE];

float find_median(unsigned int begin, unsigned int end)
{
  if (end < begin) {
    print_string("ERROR: End must be greater than or equal to begin.\n");
    return 0;
  }

  unsigned int size = end - begin + 1;
  unsigned int middle = begin + size / 2;

  float median;

  if (size % 2 == 0) {
    median = (values[middle - 1] + values[middle]) / 2.0f;
  } else {
    median = values[middle];
  }

  return median;
}

void insertion_sort()
{
  int i, j;
  float current;

  for (i = 1; i < WINDOW_SIZE; ++i) {
    current = values[i];
    j = i - 1;

    while ((j >= 0) && (current < values[j])) {
      values[j + 1] = values[j];
      j = j - 1;
    }

    values[j + 1] = current;
  }
}

// Tukey's test
void outlier()
{
  if (WINDOW_SIZE < 1) {
    print_string("ERROR: The window must contain at least one number.\n");
    return;
  }

  // Sort values
  insertion_sort();

  // Calculate median
  // float q2 = find_median(0, WINDOW_SIZE - 1);

  // The following calculations use the definition
  // of quartile that does not include the median

  // Calculate lower quartile
  float q1 = find_median(0, (WINDOW_SIZE / 2) - 1);

  // Calculate upper quartile
  float q3;

  if (WINDOW_SIZE % 2 == 0) {
    q3 = find_median(WINDOW_SIZE / 2, WINDOW_SIZE - 1);
  } else {
    q3 = find_median((WINDOW_SIZE / 2) + 1, WINDOW_SIZE - 1);
  }

  // Find interquartile range
  float range = q3 - q1;

  // Find inner fences
  float inner_lower_boundary = q1 - range * 1.5f;
  float inner_upper_boundary = q3 + range * 1.5f;

  // Find outer fences
  float outer_lower_boundary = q1 - range * 3.0f;
  float outer_upper_boundary = q3 + range * 3.0f;

  /* An outlier is an observation outside of the range
   * [q1 - k * (q3 - q1), q3 + k * (q3 - q1)].
   *
   * It is a minor outlier when k = 1.5 and
   * a major outlier when k = 3.0.
   */
  unsigned int i;
  for (i = 0; i < WINDOW_SIZE; ++i) {
    if (values[i] < outer_lower_boundary) {
      print_string("Major: ");
      print_float((int32_t) & (values[i]), 5);
      print_string("\n");
    } else {
      if (values[i] < inner_lower_boundary) {
        print_string("Minor: ");
        print_float((int32_t) & (values[i]), 5);
        print_string("\n");
      } else {
        if (values[i] > outer_upper_boundary) {
          print_string("Major: ");
          print_float((int32_t) & (values[i]), 5);
          print_string("\n");
        } else {
          if (values[i] > inner_upper_boundary) {
            print_string("Minor: ");
            print_float((int32_t) & (values[i]), 5);
            print_string("\n");
          }
        }
      }
    }
  }
}

int event_handler()
{
  readFloatState("sensor.sensors.temperature", STATE_CURRENT_VALUE, &(values[window_counter]));

  window_counter += 1;

  if (window_counter == WINDOW_SIZE) {
    outlier();

    switch (QUERY_TYPE) {
    case 0:
      unregisterStateChange(event_handler, "sensor.sensors.temperature");
      break;
    case 1:
      window_counter = 0;
      break;
    default:
      print_string("ERROR: Invalid query type.\n");
      break;
    }
  }

  return 1;
}

int main(int argc, const char *argv[])
{
  window_counter = 0;

  eh_init();

  registerStateChange(event_handler, "sensor.sensors.temperature");

  return 0;
}