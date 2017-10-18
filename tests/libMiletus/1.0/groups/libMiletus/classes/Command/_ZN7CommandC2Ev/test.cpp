#include <libMiletusCoisa/libMiletus.h>

class Command_SubClass: public Command
{
public:
    Command_SubClass():Command(){}
};//Command_SubClass

int main(int argc, char *argv[])
{
    Command_SubClass* obj = new Command_SubClass(); //target call
    return 0;
}
