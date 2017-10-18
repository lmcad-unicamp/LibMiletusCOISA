#include <libMiletusCoisa/libMiletus.h>
#include <ArduinoJson/JsonVariant.hpp>
int main(int argc, char *argv[])
{
    std::string name = "name";
    MiletusDevice* device = new MiletusDevice(name);
    MiletusProvider* p = new MiletusProvider();
    device->setProvider(p); //target call
    return 0;
}
