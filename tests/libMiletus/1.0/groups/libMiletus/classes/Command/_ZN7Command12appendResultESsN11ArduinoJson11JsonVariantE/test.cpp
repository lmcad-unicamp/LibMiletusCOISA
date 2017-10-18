#include <libMiletusCoisa/libMiletus.h>
#include <ArduinoJson/JsonVariant.hpp>

using namespace ArduinoJson;

int main(int argc, char *argv[])
{
    std::string name = "name";
    Command* obj = new Command(name);
    std::string key = "key";
    JsonVariant value(true);
    obj->appendResult(key, value); //target call
    return 0;
}
