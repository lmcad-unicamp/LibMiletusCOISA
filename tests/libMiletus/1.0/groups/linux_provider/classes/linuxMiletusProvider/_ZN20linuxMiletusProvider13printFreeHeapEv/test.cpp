#include <linux/linux_provider.h>
int main(int argc, char *argv[])
{
    linuxMiletusProvider* linuxmiletusprovider = new linuxMiletusProvider();
    linuxmiletusprovider->printFreeHeap(); //target call
    return 0;
}
