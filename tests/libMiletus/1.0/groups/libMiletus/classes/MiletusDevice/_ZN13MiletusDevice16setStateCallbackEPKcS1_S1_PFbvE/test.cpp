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
    MiletusDevice* device = new MiletusDevice(name);
    device->setStateCallback(
        "componentName",
        "traitName",
        "stateName",
        &f); //target call
    return 0;
}
