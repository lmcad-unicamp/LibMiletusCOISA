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
#include "arduino_mqtt.h"

// class MiletusDeviceCommIf;

arduino_mqtt* arduino_mqtt::callback_obj = 0;


void callback1(char* topic, uint8_t* payload, unsigned int length){
  Serial.println("\n******************CBK1******************");
  if (arduino_mqtt::callback_obj != NULL){
    ( (arduino_mqtt*) arduino_mqtt::callback_obj)->pubsub_callback(topic, payload, length);
  } else {
    Serial.println("Error calling callback");
  }
}

arduino_mqtt::arduino_mqtt(Client& _network_client, const char* mqtt_server, int mqtt_port) :
  pubsub_client(_network_client)
{
  initialized = true;
  pubsub_client.setServer(mqtt_server, mqtt_port);
  pubsub_client.setCallback(callback1);
  arduino_mqtt::callback_obj = this;
}

void arduino_mqtt::pubsub_callback(char* topic, uint8_t* payload, unsigned int length)
{
  current_request->status = UNKNOWN;
  String req = (const char*)payload;
  req = req.substring(0, length);
  String req_header = req.substring(0, req.indexOf(FIRST_SEPARATOR));
  String req_body = req.substring(req_header.length());

  Serial.print("req_header: ");
  Serial.println(req_header);
  Serial.print("req_body: ");
  Serial.println(req_body);


  if (req_header == "/info")
  {
    current_request->status = INFO;
    return;
  }
  else if (req_header == "/traits")
  {
    current_request->status = TRAITS;
    return;
  }
  else if (req_header == "/components")
  {
    current_request->status = COMPONENTS;
    return;
  }
  else if (req_header.indexOf("/commands")==0)
  {
    // current_request example: "/execute/command/?application=json&value={json_file_content}&end"
    if (req_body.indexOf("application=json")<1){
      Serial.println("BLE package is corrupted.\napplication=json < 1 ");
      return;
    }
    if (req_body.indexOf("value=") < 1){
      Serial.println("BLE package is corrupted.\nvalue < 1");
      return;
    }
    req_body = req_body.substring(req_body.indexOf("value=") + 6);
    
    Serial.print("req_body: ");
    Serial.println(req_body);
    
    char * packageBody = new char [req_body.length()+1];
    req_body.toCharArray(packageBody, req_body.length()+1); 
    // TODO: check if these +1 on toCharArray's second argument is mandatory.
    current_request->commandJson = packageBody;
    delete[] packageBody;

    current_request->status = COMMANDS;
    if (req_header.indexOf("/commands/execute")==0){
      current_request->commandID = COMMAND_EXECUTE;
    }
    else if (req_header.indexOf("/commands/status")==0){
      current_request->commandID = COMMAND_STATUS;
    }
    else if (req_header.indexOf("/commands/list")==0){
      current_request->commandID = COMMAND_LIST;
    }
    else if (req_header.indexOf("/commands/cancel")==0){
      current_request->commandID = COMMAND_CANCEL;
    }
  }
  else
  {
    current_request->status = UNKNOWN;
    return;
  }

  Serial.println("Done with BLE client");
  return;
}


void arduino_mqtt::reconnect() {
  // Loop until we're reconnected
  while (!pubsub_client.connected()) {
    // Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (pubsub_client.connect(MQTT_USERNAME)) {
      Serial.print("*");
      // Serial.println("connected");
      // Once connected, publish an announcement...
      pubsub_client.publish(TOPIC_NAME, "hello world");
      // ... and resubscribe
      pubsub_client.subscribe(TOPIC_NAME);
    } else {
      Serial.print("\nfailed, rc=");
      Serial.println(pubsub_client.state());
      // Serial.println("Rebooting");
      // ESP.restart();
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

int arduino_mqtt::handleEvent(RequestT * request){
  // Serial.print("=-=");
  current_request = request;
  request->status = NOCLIENT;
  if (!pubsub_client.connected()) {
    reconnect();
  }
  pubsub_client.loop();
  yield();
  return request->status;
}

bool arduino_mqtt::sendJsonToClient(std::string json){
  yield();// yield to allow ESP8266 background functions

  Serial.print("Publish message: ");
  Serial.println(json.c_str());
  // pubsub_client.publish(TOPIC_NAME, "Ola");
  pubsub_client.publish("TOPIC_NAME", json.c_str());
  yield();
  // Serial.printf("sendJsonToClient heap size: %u\n", ESP.getFreeHeap());
}

bool arduino_mqtt::sendErrorToClient(){
  yield();// yield to allow ESP8266 background functions
  pubsub_client.publish(TOPIC_NAME, "ERROR");
  yield();
  // Serial.printf("sendErrorToClient heap size: %u\n", ESP.getFreeHeap());
}
