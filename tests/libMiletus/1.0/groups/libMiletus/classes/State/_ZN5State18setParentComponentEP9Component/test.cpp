#include <libMiletusCoisa/libMiletus.h>
#include <ArduinoJson/JsonVariant.hpp>
int main(int argc, char *argv[])
{
    State* state = new State();
    std::string name = "name";
    Component* component = new Component(name);
    state->setParentComponent(component); //target call
    return 0;
}
