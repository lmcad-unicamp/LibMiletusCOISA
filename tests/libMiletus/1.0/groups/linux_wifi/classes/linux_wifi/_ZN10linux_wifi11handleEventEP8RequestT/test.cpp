#include <linux/linux_wifi.h>
#include <libMiletusCoisa/libMiletus.h>
#include <ArduinoJson/JsonVariant.hpp>

class linux_wifi_SubClass: public linux_wifi
{
public:
    linux_wifi_SubClass():linux_wifi(){}

    int handleEvent(struct RequestT* p1) {
       return 3;
    }

    bool sendErrorToClient() {
       return true;
    }

    bool sendJsonToClient(std::string json) {
       return true;
    }
};//linux_wifi_SubClass

class MiletusDeviceCommIf_SubClass: public MiletusDeviceCommIf
{
public:
    MiletusDeviceCommIf_SubClass():MiletusDeviceCommIf(){}

    int handleEvent(struct RequestT* p1) {
       return 4;
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
    linux_wifi* obj = (linux_wifi*)new linux_wifi_SubClass();
    struct RequestT requestt = {
        1,
        new MiletusDeviceCommIf_SubClass(),
        2,
        "commandJson"};
    obj->handleEvent(&requestt); //target call
    return 0;
}
