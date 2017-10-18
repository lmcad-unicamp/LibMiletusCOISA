#include <libMiletusCoisa/libMiletus.h>
#include <ArduinoJson/JsonVariant.hpp>

//auxiliary function
bool
f(Command& p1){
    return true;
}

int main(int argc, char *argv[])
{
    std::string name = "name";
    Component* component = new Component(name);
    component->setCommand("trait", "cmd", &f); //target call
    return 0;
}
