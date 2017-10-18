#include <libMiletusCoisa/libMiletus.h>
#include <ArduinoJson/JsonVariant.hpp>
int main(int argc, char *argv[])
{
    std::string name = "name";
    Command* obj = new Command(name);
    obj->getJson(); //target call
    return 0;
}
