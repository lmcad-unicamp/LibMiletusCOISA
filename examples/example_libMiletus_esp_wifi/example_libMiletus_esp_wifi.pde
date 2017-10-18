/*************************************************************************
The MIT License (MIT)
Copyright (c) 2016 Jeferson Rech Brunetta -- ra161253@students.ic.unicamp.br
Copyright (c) 2016 Edson Borin -- edson@ic.unicamp.br
Copyright (c) 2016 João Batista C. G. Moreira -- joao.moreira@lsc.ic.unicamp.br

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Description:

In this example, we demonstrate how to implement a IoT device that
have no commands but expose the state of an attached DHT sensor receives.
You must adjust the code to reflect the setting of your wifi network (Hint:
look for MODIFY strings).

*************************************************************************/

#include <libMiletus.h>
#include <ArduinoJson.h>
#include <esp8266_wifi.h>
#include <esp8266_provider.h>
#include <SoftwareSerial.h>
#include "example_libMiletus_esp_wifi.h"
#include "DHT.h"

#define FIX_IO_CONTENT 1


#define NOT_SERIAL 1

#ifdef NOT_SERIAL
  uint64_t input_counter = 0;
  uint64_t max_input_dispatch = 65536;
#endif

/* MODIFY: Modify this macro with your WiFi SSID */
#define WIFI_SSID "LMCAD"
/* MODIFY: Modify this macro with your WiFi password */
#define WIFI_PASSWORD "Senhadowifidolmcad752"
#define DEVICE_NAME "My_Favorite_IOT_Device"

/* MODIFY: Modify this macro with your DHT plugged pin */
#define DHTPIN D2
/* MODIFY: Modify this macro with your DHT model */
#define DHTTYPE DHT11

/* Global variable indicating the current LED state. False == off, true == on.*/
bool LEDstate = false;

/* Global variable indicating if stdin and stdout have been redirected to files. */
bool redirected = false;

/* Keep the least values to avoid unnecessary updates */
float temperature = 0;
float humidity = 0;

/* This is the LibMiletus IoT device. */
MiletusDevice * _myDevice;

/* This is the DHT object. */
DHT dht(DHTPIN, DHTTYPE);

/* Component. In this example, the IoT device has a single component. */
const char * kComponentName = "sensor";

char kTraits[] = R"({
  "sensors": {
    "commands": {
      "toggleLED": {
        "minimalRole": "user",
        "parameters": {
        }
      }
    },
    "state": {
      "temperature": {
        "isRequired": true,
        "type": "number",
        "unity": "celsius",
        "minimum": 0,
        "maximum": 50
      },
      "humidity": {
        "isRequired": true,
        "type": "number"
      }
    }
  }
})";

bool toggleLED_handler(Command& command)
{
  printf("I'm called\n");
  command.appendResult("CurrentTemp", temperature);
  return true;
}

void updateSensors(){
#if NOT_SERIAL
  if(_myDevice->getSuccessfulInstallation()){
    temperature = input_car[input_counter];
    input_counter++;
    if (input_counter > 65536){
      input_counter = 0;
    }
    _myDevice->setState(kComponentName, "sensors", "temperature" , temperature);
  }
#elif FIX_IO_CONTENT
  if (MiletusDevice::getSuccessfulInstallation() && Serial.available() > 0) {
    temperature = Serial.parseFloat();

    char junk;
    while (Serial.available() > 0) { // .parseFloat() can leave non-numeric characters
      junk = Serial.read() ;         // clear the keyboard buffer
      if (junk == ' ') break;
      if (junk == 0) Serial.print("EOF!\n");
    }

    _myDevice->setState(kComponentName, "sensors", "temperature" , temperature);
  }
#else
  temperature += 0.1;
  humidity += 0.15;

  _myDevice->setState(kComponentName, "sensors", "temperature" , temperature);    
  _myDevice->setState(kComponentName, "sensors", "humidity" , humidity);
#endif
}

void setup()
{
  _myDevice = new MiletusDevice(DEVICE_NAME);

  /* Add basic hardware features provider. */
  _myDevice->setProvider(new ESP8266MiletusProvider());

  /* Load device traits. */
  _myDevice->loadJsonTraits(kTraits);

  /* Add a component to the device. */
  Component* component = new Component(kComponentName);
  list<const char*> traits_list;
  traits_list.push_back("sensors");
  _myDevice->addComponent(component, traits_list);

  /* Map commant to handler. */
  component->setCommand("sensors", "toggleLED", toggleLED_handler);

  /* Create a communication interface and add it to the device.
   * Using ESP8266 Wifi Interface. */
  esp8266_wifi *myWifiIf = new esp8266_wifi(DEVICE_NAME,
                                            WIFI_SSID,
                                            WIFI_PASSWORD);
  _myDevice->addCommInterface(myWifiIf);

  /* Defines LED pin as an putput */
  pinMode(LED_BUILTIN, OUTPUT);

#if FIX_IO_CONTENT
  delay(100);
#endif

  Serial.setTimeout(1500);
}

void loop()
{
  /* Delay 50 ms */
#if ! FIX_IO_CONTENT
  delay(100);
#endif
  /* Let libMiletus handle requests. */
  _myDevice->handleEvents();
  /* Check for updates on DHT sensor. */
  updateSensors();
}
