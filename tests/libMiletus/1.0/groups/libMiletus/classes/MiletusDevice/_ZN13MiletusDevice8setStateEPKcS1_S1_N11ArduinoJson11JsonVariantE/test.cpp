#include <libMiletusCoisa/libMiletus.h>
#include <ArduinoJson/JsonVariant.hpp>

using namespace ArduinoJson;

int main(int argc, char *argv[])
{
    std::string name = "name";
    MiletusDevice* device = new MiletusDevice(name);
    JsonVariant value(true);
    device->setState(
        "componentName",
        "traitName",
        "stateName",
        value); //target call
    return 0;
}
