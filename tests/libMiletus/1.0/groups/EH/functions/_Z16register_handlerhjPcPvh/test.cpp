#include <libMiletusCoisa/libMiletus.h>
#include <stdint.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
    void* argv_1 = malloc(256);
    register_handler(
        'A',
        1,
        "evname",
        argv_1,
        'B'); //target call
    return 0;
}
