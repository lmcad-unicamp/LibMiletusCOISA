#include <libMiletusCoisa/libMiletus.h>
#include <ArduinoJson/JsonVariant.hpp>
int main(int argc, char *argv[])
{
    Trait* trait = new Trait();
    trait->getJson(); //target call
    return 0;
}
