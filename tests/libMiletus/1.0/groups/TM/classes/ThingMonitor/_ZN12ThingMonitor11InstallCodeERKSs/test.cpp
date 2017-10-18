#include <libMiletusCoisa/libMiletus.h>
#include <ArduinoJson/JsonVariant.hpp>
int main(int argc, char *argv[])
{
    ThingMonitor* thingmonitor = new ThingMonitor();
    std::string fileB64Encoded = "testdata/sample.txt";
    thingmonitor->InstallCode(fileB64Encoded); //target call
    return 0;
}
