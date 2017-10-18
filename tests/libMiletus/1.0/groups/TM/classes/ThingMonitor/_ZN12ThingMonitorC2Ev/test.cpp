#include <libMiletusCoisa/libMiletus.h>

class ThingMonitor_SubClass: public ThingMonitor
{
public:
    ThingMonitor_SubClass():ThingMonitor(){}
};//ThingMonitor_SubClass

int main(int argc, char *argv[])
{
    ThingMonitor_SubClass* thingmonitor = new ThingMonitor_SubClass(); //target call
    return 0;
}
