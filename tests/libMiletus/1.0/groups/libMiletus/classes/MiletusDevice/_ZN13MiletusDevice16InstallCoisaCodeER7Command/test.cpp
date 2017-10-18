#include <libMiletusCoisa/libMiletus.h>
#include <ArduinoJson/JsonVariant.hpp>
int main(int argc, char *argv[])
{
    std::string name = "name";
    Command command_1(name);
    MiletusDevice::InstallCoisaCode(command_1); //target call
    return 0;
}
