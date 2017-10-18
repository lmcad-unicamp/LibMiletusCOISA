#include <libMiletusCoisa/libMiletus.h>
#include <ArduinoJson/JsonVariant.hpp>
int main(int argc, char *argv[])
{
    std::string name = "name";
    Command* obj = new Command(name);
    std::string name_1 = "name";
    Component* component = new Component(name_1);
    obj->setParentComponent(component); //target call
    return 0;
}
