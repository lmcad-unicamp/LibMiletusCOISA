#include <linux/linux_wifi.h>

class linux_wifi_SubClass: public linux_wifi
{
public:
    linux_wifi_SubClass():linux_wifi(){}

    int handleEvent(struct RequestT* p1) {
       return 1;
    }

    bool sendErrorToClient() {
       return true;
    }

    bool sendJsonToClient(std::string json) {
       return true;
    }
};//linux_wifi_SubClass

int main(int argc, char *argv[])
{
    linux_wifi_SubClass* obj = new linux_wifi_SubClass(); //target call
    return 0;
}
