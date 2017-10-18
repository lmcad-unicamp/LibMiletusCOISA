#include <linux/linux_provider.h>

class linuxMiletusProvider_SubClass: public linuxMiletusProvider
{
public:
    linuxMiletusProvider_SubClass():linuxMiletusProvider(){}
};//linuxMiletusProvider_SubClass

int main(int argc, char *argv[])
{
    linuxMiletusProvider_SubClass* linuxmiletusprovider = new linuxMiletusProvider_SubClass(); //target call
    return 0;
}
