#include <coisa/HAL.h>
#include <stdint.h>
int main(int argc, char *argv[])
{
    char identifier [] = {
        'A',
        'B',
        'C',
        'D',
        '\0'};
    hal_call(1, identifier); //target call
    return 0;
}
