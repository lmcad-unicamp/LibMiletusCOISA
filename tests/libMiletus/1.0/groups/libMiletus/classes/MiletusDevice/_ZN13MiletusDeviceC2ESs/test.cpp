#include <libMiletusCoisa/libMiletus.h>
#include <ArduinoJson/JsonVariant.hpp>

class MiletusDevice_SubClass: public MiletusDevice
{
public:
    MiletusDevice_SubClass(std::string name):MiletusDevice(name){}
};//MiletusDevice_SubClass

int main(int argc, char *argv[])
{
    std::string name = "name";
    MiletusDevice_SubClass* device = new MiletusDevice_SubClass(name); //target call
    return 0;
}
