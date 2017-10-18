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
*************************************************************************/
#ifndef ARDUINO_MQTT_H
#define ARDUINO_MQTT_H

#include "libMiletusCommIf.h"
#include <PubSubClient.h>
#include <string>
#include <Arduino.h>

#ifndef TOPIC_NAME
#define TOPIC_NAME "mySomeTopic"
#endif
#ifndef MQTT_USERNAME
#define MQTT_USERNAME "KJHsdfakjhKJHDcf"
#endif

#define FIRST_SEPARATOR '?'
#define OTHER_SEPARATOR '&'
#define SIZE_SEPARATOR 'B'


class arduino_mqtt : public MiletusDeviceCommIf
{
public:
  arduino_mqtt(Client& _network_client, const char* mqtt_server, int mqtt_port); 
  ~arduino_mqtt(){};
  int handleEvent(RequestT *);
  bool sendJsonToClient(std::string json);
  bool sendErrorToClient();
  void pubsub_callback(char* topic, uint8_t* payload, unsigned int length);
  static arduino_mqtt* callback_obj;

private:
	void reconnect();
  PubSubClient pubsub_client;
  RequestT * current_request;
};

#endif // ARDUINO_MQTT_H