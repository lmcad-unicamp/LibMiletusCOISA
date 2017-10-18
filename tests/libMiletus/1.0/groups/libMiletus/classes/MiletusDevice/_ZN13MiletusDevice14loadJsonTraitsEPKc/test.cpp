#include <libMiletusCoisa/libMiletus.h>
#include <ArduinoJson/JsonVariant.hpp>
int main(int argc, char *argv[])
{
    std::string name = "name";
    MiletusDevice* device = new MiletusDevice(name);
    device->loadJsonTraits("json"); //target call
    return 0;
}
