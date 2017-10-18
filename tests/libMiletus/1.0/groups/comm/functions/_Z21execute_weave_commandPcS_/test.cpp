#include <coisa/comm.h>
int main(int argc, char *argv[])
{
    char traitCommand [] = {
        'A',
        'B',
        'C',
        'D',
        '\0'};
    char parameters [] = {
        'E',
        'F',
        'G',
        'H',
        '\0'};
    execute_weave_command(traitCommand, parameters); //target call
    return 0;
}
