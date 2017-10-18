#include <libMiletusCoisa/libMiletus.h>
#include <ArduinoJson/JsonVariant.hpp>
int main(int argc, char *argv[])
{
    std::string name = "name";
    Command* obj = new Command(name);
    std::string paramName = "paramName";
    obj->getParameter(paramName); //target call
    return 0;
}
