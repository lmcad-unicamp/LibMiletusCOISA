#include <coisa/syscall.h>
#include <stdint.h>
int main(int argc, char *argv[])
{
    syscall('A'); //target call
    return 0;
}
