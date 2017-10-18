#include <libMiletusCoisa/libMiletus.h>
#include <ArduinoJson/JsonVariant.hpp>

using namespace ArduinoJson;

int main(int argc, char *argv[])
{
    std::string name = "name";
    Component* component = new Component(name);
    JsonVariant value(true);
    component->setState("trait", "state", value); //target call
    return 0;
}
