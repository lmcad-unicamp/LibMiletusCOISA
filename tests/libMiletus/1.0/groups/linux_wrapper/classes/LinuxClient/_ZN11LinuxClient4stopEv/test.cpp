#include <linux/linux_wrapper.h>
int main(int argc, char *argv[])
{
    LinuxClient* client = new LinuxClient(1);
    client->stop(); //target call
    return 0;
}
