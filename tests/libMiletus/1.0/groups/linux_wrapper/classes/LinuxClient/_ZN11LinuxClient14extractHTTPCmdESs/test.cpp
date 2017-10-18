#include <linux/linux_wrapper.h>
#include <ArduinoJson/JsonVariant.hpp>
int main(int argc, char *argv[])
{
    LinuxClient* client = new LinuxClient(1);
    std::string msg = "msg";
    client->extractHTTPCmd(msg); //target call
    return 0;
}
