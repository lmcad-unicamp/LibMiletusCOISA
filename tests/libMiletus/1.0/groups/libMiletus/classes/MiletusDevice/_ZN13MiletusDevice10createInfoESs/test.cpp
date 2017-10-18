#include <libMiletusCoisa/libMiletus.h>
#include <ArduinoJson/JsonVariant.hpp>
int main(int argc, char *argv[])
{
    std::string name = "name";
    MiletusDevice* device = new MiletusDevice(name);
    std::string name_1 = "name";
    device->createInfo(name_1); //target call
    return 0;
}
