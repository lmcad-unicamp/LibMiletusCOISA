#include <linux/linux_provider.h>
#include <stdint.h>
int main(int argc, char *argv[])
{
    linuxMiletusProvider* linuxmiletusprovider = new linuxMiletusProvider();
    linuxmiletusprovider->getRand(); //target call
    return 0;
}
