#include <libMiletusCoisa/libMiletus.h>
#include <ArduinoJson/JsonVariant.hpp>
int main(int argc, char *argv[])
{
    State* state = new State();
    state->getJson(); //target call
    return 0;
}
