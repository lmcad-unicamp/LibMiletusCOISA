#include <coisa/comm.h>
#include <stdint.h>
int main(int argc, char *argv[])
{
    char parameters [] = {
        'A',
        'B',
        'C',
        'D',
        '\0'};
    comm_call(1, parameters); //target call
    return 0;
}
