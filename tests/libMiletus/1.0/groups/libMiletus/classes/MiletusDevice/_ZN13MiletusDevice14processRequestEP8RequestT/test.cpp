#include <libMiletusCoisa/libMiletus.h>
#include <ArduinoJson/JsonVariant.hpp>

class MiletusDeviceCommIf_SubClass: public MiletusDeviceCommIf
{
public:
    MiletusDeviceCommIf_SubClass():MiletusDeviceCommIf(){}

    int handleEvent(struct RequestT* p1) {
       return 3;
    }

    bool sendErrorToClient() {
       return true;
    }

    bool sendJsonToClient(std::string json) {
       return true;
    }
};//MiletusDeviceCommIf_SubClass

int main(int argc, char *argv[])
{
    std::string name = "name";
    MiletusDevice* device = new MiletusDevice(name);
    struct RequestT request = {
        1,
        new MiletusDeviceCommIf_SubClass(),
        2,
        "commandJson"};
    device->processRequest(&request); //target call
    return 0;
}
