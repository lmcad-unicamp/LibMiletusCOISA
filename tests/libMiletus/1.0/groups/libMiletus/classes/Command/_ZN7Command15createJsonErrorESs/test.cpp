#include <libMiletusCoisa/libMiletus.h>
#include <ArduinoJson/JsonVariant.hpp>
int main(int argc, char *argv[])
{
    std::string error = "error";
    Command::createJsonError(error); //target call
    return 0;
}
