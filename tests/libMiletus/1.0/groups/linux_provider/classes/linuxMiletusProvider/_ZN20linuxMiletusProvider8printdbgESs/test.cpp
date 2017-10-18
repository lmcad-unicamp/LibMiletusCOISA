#include <linux/linux_provider.h>
#include <ArduinoJson/JsonVariant.hpp>
int main(int argc, char *argv[])
{
    linuxMiletusProvider* linuxmiletusprovider = new linuxMiletusProvider();
    std::string msg = "msg";
    linuxmiletusprovider->printdbg(msg); //target call
    return 0;
}
