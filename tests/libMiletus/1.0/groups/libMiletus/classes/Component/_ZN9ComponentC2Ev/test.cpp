#include <libMiletusCoisa/libMiletus.h>

class Component_SubClass: public Component
{
public:
    Component_SubClass():Component(){}
};//Component_SubClass

int main(int argc, char *argv[])
{
    Component_SubClass* component = new Component_SubClass(); //target call
    return 0;
}
