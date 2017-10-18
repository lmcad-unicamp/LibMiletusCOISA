#include <libMiletusCoisa/libMiletus.h>
#include <ArduinoJson/JsonVariant.hpp>

//auxiliary function
bool
f(Command& p1){
    return true;
}

int main(int argc, char *argv[])
{
    std::string name = "name";
    Command* obj = new Command(name);
    obj->setUserFunction(&f); //target call
    return 0;
}
