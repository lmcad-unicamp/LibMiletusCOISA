#include <libMiletusCoisa/libMiletus.h>
#include <ArduinoJson/JsonVariant.hpp>
int main(int argc, char *argv[])
{
    std::string name = "name";
    Component* component = new Component(name);
    component->getJson(); //target call
    return 0;
}
