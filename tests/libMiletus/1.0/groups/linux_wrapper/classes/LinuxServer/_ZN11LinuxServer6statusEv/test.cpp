#include <linux/linux_wrapper.h>
#include <stdint.h>
int main(int argc, char *argv[])
{
    LinuxServer* server = new LinuxServer((uint16_t) 1);
    server->status(); //target call
    return 0;
}
