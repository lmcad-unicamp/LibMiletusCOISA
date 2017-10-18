#include <libMiletusCoisa/libMiletus.h>
#include <ArduinoJson/JsonVariant.hpp>

class MiletusDeviceCommIf_SubClass: public MiletusDeviceCommIf
{
public:
    MiletusDeviceCommIf_SubClass():MiletusDeviceCommIf(){}

    int handleEvent(struct RequestT* p1) {
       return 1;
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
    MiletusDeviceCommIf_SubClass* comm_if = new MiletusDeviceCommIf_SubClass();
    device->addCommInterface(comm_if, true); //target call
    return 0;
}
