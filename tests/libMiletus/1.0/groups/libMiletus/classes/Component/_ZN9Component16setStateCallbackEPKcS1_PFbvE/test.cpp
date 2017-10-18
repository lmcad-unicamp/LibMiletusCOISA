#include <libMiletusCoisa/libMiletus.h>
#include <ArduinoJson/JsonVariant.hpp>

//auxiliary function
bool
f(){
    return true;
}

int main(int argc, char *argv[])
{
    std::string name = "name";
    Component* component = new Component(name);
    component->setStateCallback("trait", "state", &f); //target call
    return 0;
}
