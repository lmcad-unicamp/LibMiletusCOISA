#include <linux/linux_wrapper.h>
#include <ArduinoJson/JsonVariant.hpp>
int main(int argc, char *argv[])
{
    LinuxClient* client = new LinuxClient(1);
    client->readString(); //target call
    return 0;
}
