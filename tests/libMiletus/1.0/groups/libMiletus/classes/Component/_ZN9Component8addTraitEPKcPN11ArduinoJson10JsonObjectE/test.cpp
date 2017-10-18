#include <libMiletusCoisa/libMiletus.h>
#include <ArduinoJson/JsonObject.hpp>
#include <ArduinoJson/JsonBuffer.hpp>
#include <ArduinoJson/JsonVariant.hpp>
#include <stdlib.h>

using namespace ArduinoJson;

class JsonBuffer_SubClass: public JsonBuffer
{
public:
    JsonBuffer_SubClass():JsonBuffer(){}

    void* alloc(size_t size) {
       return malloc(256);
    }
};//JsonBuffer_SubClass

int main(int argc, char *argv[])
{
    std::string name = "name";
    Component* component = new Component(name);
    JsonBuffer_SubClass* buffer = new JsonBuffer_SubClass();
    JsonObject* jsonTraits = new JsonObject(buffer);
    component->addTrait("trait", jsonTraits); //target call
    return 0;
}
