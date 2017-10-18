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
#include "libMiletus.h"

#include "EH.h"
#include "TM.h"
#include "comm.h"
#include "vm.h"

#ifdef ARDUINO
#include <Arduino.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(D5, D6); // RX, TX
#endif

#if MEASURING
#ifndef ARDUINO
#include <papi.h>
#include <sys/time.h>
#endif
#endif

#if MEASURING
uint64_t global_vm_cycle_count_without_syscall = 0;
uint64_t global_vm_cycle_count_with_syscall = 0;

uint64_t current_mileis_program_received = 0;
uint64_t startup_time = 0;

#ifdef ARDUINO
#else
#include <papi.h>
int PAPIEvents[] = {PAPI_TOT_CYC, PAPI_TOT_INS};
#endif
#endif

MiletusProvider *MiletusDevice::provider = new MiletusProvider();

ThingMonitor tm;
// TODO check the possibility of create a new tm for each thread

bool MiletusDevice::successfulInstallation = false;
std::string MiletusDevice::inputDataPath = "";
std::string MiletusDevice::outputDataPath = "";

bool vm_started = false;

bool stateChanged(const char *component_trait_state_value)
{
  printf("StateChanged called: component_trait_state_value: %s\n", component_trait_state_value);
  return true;
}

// MILETUSDEVICE CLASS
MiletusDevice::MiletusDevice(string name)
{
// remove this futurely
#ifdef ARDUINO
  Serial.begin(115200);
  mySerial.begin(115200);
  mySerial.println("\nReboot..");
#endif

#if !FIX_IO_CONTENT
  char kTraits[] = R"({
    "_coisa": { 
      "commands": {
        "setConfig": {
          "minimalRole": "user",
          "parameters": {
            "sendByteCode": {
              "type": "string"
            },
            "finalPackage": {
              "type": "boolean"
            }
          }
        }
      },
      "state": {
        "sdtin": {
          "isRequired": false,
          "type": "string"
        },
        "stdout": {
          "isRequired": false,
          "type": "string"
        },
        "stderror": {
          "isRequired": false,
          "type": "string"
        }
      }
    }
  })";
#else
  char kTraits[] = R"({
    "_coisa": { 
      "commands": {
        "setConfig": {
          "minimalRole": "user",
          "parameters": {
            "sendByteCode": {
              "type": "string"
            },
            "finalPackage": {
              "type": "boolean"
            },
            "inputDataPath": {
              "type": "string"
            },
            "outputDataPath": {
              "type": "string"
            }
          }
        }
      },
      "state": {
        "sdtin": {
          "isRequired": false,
          "type": "string"
        },
        "stdout": {
          "isRequired": false,
          "type": "string"
        },
        "stderror": {
          "isRequired": false,
          "type": "string"
        }
      }
    }
  })";
#endif

  this->name = name;

  /* Load def. device traits. */
  this->loadJsonTraits(kTraits);

  /* Add a component to the device. */
  Component *base = new Component("base");
  list<const char *> traits_list;
  traits_list.push_back("_coisa");
  this->addComponent(base, traits_list);

  /* Map commant to handler. */
  base->setCommand("_coisa", "setConfig", MiletusDevice::InstallCoisaCode);
  // this->setStateCallback("kComponentName", "trait", "state" , stateChanged);
}

bool MiletusDevice::InstallCoisaCode(Command &command)
{
  vm_started = false;
  MiletusDevice::provider->printdbg("[MiletusDevice::InstallCoisaCode] Received _coisa.setConfig command.");
  const char *param = command.getParameter("sendByteCode");

  if (param != NULL) {
    std::string requested_code_to_execution(param, strlen(param));
    MiletusDevice::successfulInstallation = (bool)command.getParameter("finalPackage");
#ifdef ARDUINO
    uint64_t startup_time_local = millis();
#else
    uint64_t startup_time_local = 0; // TODO millis
    struct timeval now;
    if (gettimeofday(&now, NULL) == 0) {
      // printf("gettimeofday() successful.\n");
      // printf("time = %u.%06u\n", now.tv_sec, now.tv_usec);
      startup_time_local = (now.tv_sec * (uint64_t)1000) + (now.tv_usec / 1000);
    } else {
      printf("gettimeofday() failed, errno = %d\n", errno);
    }
#endif
    MiletusDevice::successfulInstallation &= tm.InstallCode(requested_code_to_execution);
#ifdef ARDUINO
    startup_time_local = millis() - startup_time_local;
#else
    // TODO millis
    // struct timeval now;
    if (gettimeofday(&now, NULL) == 0) {
      // printf("gettimeofday() successful.\n");
      // printf("time = %u.%06u\n", now.tv_sec, now.tv_usec);
      startup_time_local = (now.tv_sec * (uint64_t)1000) + (now.tv_usec / 1000) - startup_time_local;
    } else {
      printf("gettimeofday() failed, errno = %d\n", errno);
    }
#endif
    startup_time += startup_time_local;
    if ((bool)command.getParameter("finalPackage")) {
      tm.Restart();
#ifdef ARDUINO
      current_mileis_program_received = millis();
#else
      // TODO millis
      // struct timeval now;
      if (gettimeofday(&now, NULL) == 0) {
        // printf("gettimeofday() successful.\n");
        // printf("time = %u.%06u\n", now.tv_sec, now.tv_usec);
        current_mileis_program_received = (now.tv_sec * (uint64_t)1000) + (now.tv_usec / 1000);
      } else {
        printf("gettimeofday() failed, errno = %d\n", errno);
      }
#endif
    }

#if FIX_IO_CONTENT
    const char *inputPath = command.getParameter("inputDataPath");

    if (inputPath == NULL) {
      MiletusDevice::provider->printdbg("[MiletusDevice::InstallCoisaCode] inputDataPath parameter not found.");
      return false;
    }

    MiletusDevice::inputDataPath = std::string(inputPath, strlen(inputPath));

    const char *outputPath = command.getParameter("outputDataPath");

    if (outputPath == NULL) {
      MiletusDevice::provider->printdbg("[MiletusDevice::InstallCoisaCode] outputDataPath parameter not found.");
      return false;
    }

    MiletusDevice::outputDataPath = std::string(outputPath, strlen(outputPath));
#endif
    return true;
  }

  MiletusDevice::provider->printdbg("[MiletusDevice::InstallCoisaCode] sendByteCode parameter not found.");

  return false;
}

void MiletusDevice::RunCoisaCode()
{
  while (true) {
#if MEASURING
#ifdef ARDUINO
    uint64_t local_vm_cycle_count_start = ESP.getCycleCount();
#else
    // PAPI_start_counters(PAPIEvents, 2);
    uint64_t local_vm_cycle_count_start = 0;
#endif
#endif
    bool teste = tm.ExecuteStep();
#if MEASURING

#ifdef ARDUINO
    uint64_t local_vm_cycle_count_end = ESP.getCycleCount();
#else
    long long PAPIResults[2] = {0, 0};
    // PAPI_stop_counters(PAPIResults, 2);
    uint64_t local_vm_cycle_count_end = PAPIResults[0];
// uint64_t local_vm_inst_count_end = PAPIResults[1];
#endif
    global_vm_cycle_count_with_syscall += local_vm_cycle_count_end - local_vm_cycle_count_start;
    global_vm_cycle_count_without_syscall += local_vm_cycle_count_end - local_vm_cycle_count_start;
#endif
    if (teste) {
#if MEASURING
#ifdef ARDUINO
// Serial.printf("Global cycle Count with: %u\n", global_vm_cycle_count_with_syscall);
// Serial.printf("Global cycle Count without: %u\n", global_vm_cycle_count_without_syscall);
#else
// TODO millis print cycles
#endif
#endif
      break;
    }
    if (comm_call_return_type != 0) {
      auto i = this->components.begin();

      for (; i != components.end(); i++) {
        if ((*i)->name.compare(comm_call_return_id.component_name) == 0) {

          switch (comm_call_return_type) {
          case RETURN_STRING: {
#ifdef ARDUINO
            strcpy(user_string_buffer, (*i)->traits[comm_call_return_id.trait_name]
                                           .states[comm_call_return_id.state_name]
                                           .value.as<String>()
                                           .c_str());
#else
            strcpy(user_string_buffer, (*i)->traits[comm_call_return_id.trait_name]
                                           .states[comm_call_return_id.state_name]
                                           .value.as<string>()
                                           .c_str());
#endif
            break;
          }
          case RETURN_FLOAT: {
            number_type num;
            num.as_float =
                (*i)->traits[comm_call_return_id.trait_name].states[comm_call_return_id.state_name].value.as<float>();

            VM_memory[user_buffer_addr + 0] = (uint8_t)((num.as_int32 >> 24) & 0xFF);
            VM_memory[user_buffer_addr + 1] = (uint8_t)((num.as_int32 >> 16) & 0xFF);
            VM_memory[user_buffer_addr + 2] = (uint8_t)((num.as_int32 >> 8) & 0xFF);
            VM_memory[user_buffer_addr + 3] = (uint8_t)((num.as_int32 >> 0) & 0xFF);

            break;
          }
          case RETURN_INT32: {
            int32_t num =
                (*i)->traits[comm_call_return_id.trait_name].states[comm_call_return_id.state_name].value.as<int32_t>();

            VM_memory[user_buffer_addr + 0] = (uint8_t)((num >> 24) & 0xFF);
            VM_memory[user_buffer_addr + 1] = (uint8_t)((num >> 16) & 0xFF);
            VM_memory[user_buffer_addr + 2] = (uint8_t)((num >> 8) & 0xFF);
            VM_memory[user_buffer_addr + 3] = (uint8_t)((num >> 0) & 0xFF);

            break;
          }
          default: {
            printf("[MiletusDevice::RunCoisaCode] Invalid comm call return type.\n");
            break;
          }
          }

          break; // Stops for that is looking for the component.
        }
      }
    }
  }
}

/*void MiletusDevice::UpdateCoisaStateChange(const std::weak_ptr<base::ListValue>& dictionary) {
  auto commands = dictionary.lock();
  if (!commands)
    return;

  // LOG(INFO) << "*********** UpdateCoisaStateChange notification received ***********";
  // LOG(INFO) << "UpdateCoisaStateChange notification received";

  for (size_t i = 0; i < commands->GetSize(); i++) {
    const base::Value* this_value = nullptr;
    const base::DictionaryValue* state_dict = nullptr;
    if (!commands->Get(i, &this_value) || !this_value->GetAsDictionary(&state_dict)) {
      // LOG(WARNING) << "Not a dictionary: " << *this_value;
      continue;
    }
    // VLOG(5) << "Dictionary of states: " << *state_dict;
    const base::DictionaryValue* patch_dict;
    if (state_dict->GetDictionary("patch", &patch_dict)) {
      // LOG(INFO) << "\nDictionary of patchs: " << *patch_dict;
      // OUTPUT: Dictionary of patchs: { "onOff": { "state": "off" } }
      //Dictionary of patchs: { "_engine": { "onOff": false }
//}


      const base::DictionaryValue* selected_trait_dict;
      if (patch_dict->GetDictionary("onOff", &selected_trait_dict)) {
        // LOG(INFO) << "\n***selected_trait_dict (onOff): " << *selected_trait_dict;
        std::string onOff_state;
        if (selected_trait_dict->GetString("state", &onOff_state)) {
          // LOG(INFO) << "***OnOff state changed to: " << onOff_state;
          if (strcmp(onOff_state.c_str(), "off") == 0){
            // LOG(INFO) << "***Inserting event to EH (off)";
            // insert_event(1, (char *)"onOff.state.off");
          }
          if (strcmp(onOff_state.c_str(), "on") == 0){
            // LOG(INFO) << "***Inserting event to EH (on)";
            insert_event(1, (char *)"LONN");
          }
        }
      }
      // CREATION
      else if (patch_dict->GetDictionary("_robot", &selected_trait_dict)) {
        // LOG(INFO) << "\n***selected_trait_dict (robot): " << *selected_trait_dict;
        double threshold;
        if (selected_trait_dict->GetDouble("proximity", &threshold)) {
          // LOG(INFO) << "***proximity state changed to: " << threshold;
          // LOG(INFO) << "***limiar: " << robot_proximit_threshold;
          if (threshold < robot_proximit_threshold){
            // LOG(INFO) << "***Inserting event to EH (robot.on.proximity)";
            insert_event(4, (char *)"ROPR");
          }
        }
        int timer;
        if (selected_trait_dict->GetInteger("timer", &timer)) {
          // LOG(INFO) << "***timer state changed to: " << timer;
          // LOG(INFO) << "***limiar: " << robot_timer_threshold;
          if (timer && robot_timer_threshold && (timer % robot_timer_threshold) == 0){
            // LOG(INFO) << "***Inserting event to EH (robot.on.proximity)";
            insert_event(5, (char *)"RESE");
          }
        }
      }
      else if (patch_dict->GetDictionary("_room", &selected_trait_dict)) {
        // Ascertain the state changes to put a event on EH
        // LOG(INFO) << "\n***selected_trait_dict (room): " << *selected_trait_dict;
        double temperature;
        if (selected_trait_dict->GetDouble("temperature", &temperature)) {
          // LOG(INFO) << "***temperature state changed to: " << temperature;
          // LOG(INFO) << "***limiar above: " << room_temperature_above_threshold;
          // LOG(INFO) << "***limiar below: " << room_temperature_below_threshold;
          if (room_temperature_above_threshold < temperature){
            insert_event(8, (char *)"ROTA");
          }
          if (room_temperature_below_threshold > temperature){
            insert_event(9, (char *)"ROTB");
          }
        }
        bool presence;
        if (selected_trait_dict->GetBoolean("presence", &presence)) {
          // LOG(INFO) << "\n***presence state changed to: " << presence  <<
                 // " \nroom_presence_threshold: " << room_presence_threshold;
          if (room_presence_threshold|(1<<presence)){
            if (presence){
              // LOG(INFO) << "***Inserting event to EH (room.presence.on)";
              insert_event(10, (char *)"ROPY");
            }else{
              // LOG(INFO) << "***Inserting event to EH (room.presence.off)";
              insert_event(11, (char *)"ROPN");
            }
          }
        }
      }
      else if (patch_dict->GetDictionary("_engine", &selected_trait_dict)) {
        bool engineOnOff;
        // LOG(INFO) << "\n***selected_trait_dict (_engine): " << *selected_trait_dict;
        if (selected_trait_dict->GetBoolean("onOff", &engineOnOff)) {
          // LOG(INFO) << "***engineOnOff state changed to: " << engineOnOff;
          if (engineOnOff){
            // LOG(INFO) << "***engineOnOff inserting CAE0";
            insert_event(6, (char *)"CAE0");
          }
          else{
            // LOG(INFO) << "***engineOnOff inserting CAE1";
            insert_event(7, (char *)"CAE1");
          }
        }else{
          // LOG(INFO) << "FAIL to get onOff state";
        }
      }
      else if (patch_dict->GetDictionary("_general", &selected_trait_dict)) {
        // Ascertain the state changes to put a event on EH
        // LOG(INFO) << "\n***selected_trait_dict (general): " << *selected_trait_dict;
        double hour;
        if (selected_trait_dict->GetDouble("hour", &hour)) {
          // LOG(INFO) << "***hour state changed to: " << hour;
          // LOG(INFO) << "***limiar 1: " << car_hour_threshold1;
          // LOG(INFO) << "***limiar 2: " << car_hour_threshold2;
          if (car_hour_threshold1 == hour){
            insert_event(6, (char *)"CAH1");
          }
          if (car_hour_threshold2 == hour){
            insert_event(7, (char *)"CAH2");
          }
        }
      }
    }
  }
}*/

bool MiletusDevice::addCommInterface(MiletusDeviceCommIf *comm_if, bool def)
{
  // default node is always the first in the list
  if (def)
    this->comm_ifs.push_front(comm_if);
  else
    this->comm_ifs.push_back(comm_if);

  return true;
}

void MiletusDevice::initJson()
{
  jsonTraits = &(jsonBuffer.createObject());
  (*jsonTraits)["fingerprint"] = FINGERPRINT;
  jsonNestedTraits = &(jsonTraits->createNestedObject("traits"));
  jsonInit = true;
}

void MiletusDevice::merge(JsonObject *dest, JsonObject &src)
{
  for (auto kvp : src) {
    (*dest)[kvp.key] = kvp.value;
  }
}

void MiletusDevice::dumpTraits()
{
// TODO: Remove the use of Serial
#ifdef ARDUINO
  MiletusDevice::provider->printdbg("JsonTraits:");
  jsonTraits->prettyPrintTo(Serial);
  MiletusDevice::provider->printdbg("JsonNestedTraits:");
  jsonNestedTraits->prettyPrintTo(Serial);
#endif
}

void MiletusDevice::loadJsonTraits(const char *json)
{
  if (!jsonInit)
    initJson();
  JsonObject &jsonObjectTrait = jsonBuffer.parseObject(json);
  if (!jsonObjectTrait.success()) {
    MiletusDevice::provider->printdbg("[MiletusDevice::loadJsonTraits] Error parsing JSON trait.");
    return;
  }
  merge(jsonNestedTraits, jsonObjectTrait);
}

bool MiletusDevice::addComponent(Component *comp, list<const char *> entries)
{
  int n = entries.size();
  for (int i = 0; i < n; i++) {
    comp->addTrait(entries.back(), this->jsonTraits);
    entries.pop_back();
  }
  components.push_back(comp);
  return true;
}

list<Component *> MiletusDevice::getComponentList()
{
  return this->components;
}

void MiletusDevice::handleEvents()
{
  list<MiletusDeviceCommIf *>::iterator i = comm_ifs.begin();
  list<MiletusDeviceCommIf *>::iterator ie = comm_ifs.end();
  if (MiletusDevice::successfulInstallation && !vm_started) {
    vm_started = true;
    this->RunCoisaCode();
  }
  if (MiletusDevice::successfulInstallation) {
    while (consume_event() == 1) {
      this->RunCoisaCode();
    }
  }
  while (i != ie) {
    MiletusDeviceCommIf *comm = *i;
    if (comm->initialized) {
      RequestT request;
      request.media = *i;
      comm->handleEvent(&request);
      if (request.status >= INFO) {
        processRequest(&request);
      }
    }
    i++;
  }
}

string MiletusDevice::getComponentsJson()
{
  string cjson = "{";
  cjson += "\"fingerprint\":";
  cjson += FINGERPRINT;
  cjson += ",";
  cjson += "\"components\":{\n";
  bool first = true;
  for (auto& i : components) {
    if (first)
      first = true;
    else
      cjson += ",";

    cjson += i->getJson();
  }
  cjson += "\n}\n}";
  return cjson;
}

void MiletusDevice::processRequest(RequestT *request)
{
  string response = "";
  switch (request->status) {
  case INFO:
    MiletusDevice::provider->printdbg("INFO");
    _buffer = new char[jsonInfo->measureLength() + 1];
    jsonInfo->printTo(_buffer, jsonInfo->measureLength() + 1);
    request->media->sendJsonToClient(_buffer);
    free(_buffer);
    break;
  case TRAITS:
    MiletusDevice::provider->printdbg("TRAITS");
    _buffer = new char[(*jsonTraits).measureLength() + 1];
    (*jsonTraits).printTo(_buffer, (*jsonTraits).measureLength() + 1);
    request->media->sendJsonToClient(_buffer);
    free(_buffer);
    break;
  case COMPONENTS:
    MiletusDevice::provider->printdbg("COMPONENTS");
    response = getComponentsJson();
    request->media->sendJsonToClient(response);
    break;
  case COMMANDS:
    MiletusDevice::provider->printdbg("COMMANDS");
    switch (request->commandID) {
    case COMMAND_EXECUTE:
      response = executeCommand(request->commandJson);
      request->media->sendJsonToClient(response);
    case COMMAND_STATUS:
    case COMMAND_LIST:
    case COMMAND_CANCEL:
      break;
    default:
      request->media->sendErrorToClient();
      break;
    }
    break;
  default:
    MiletusDevice::provider->printdbg("[MiletusDevice::processRequest] Error: status unknown.");
  }
}

string MiletusDevice::executeCommand(string jsonCommand)
{
  // Find the right command, parse the received JSON and them run.
  // find the right component in the list components
  // break the name in trait.command
  DynamicJsonBuffer jsonBuffer;

  JsonObject &jsonObjectCommand = jsonBuffer.parseObject(jsonCommand.c_str());

  if (!jsonObjectCommand.success()) {
    return Command::createJsonError("[MiletusDevice::executeCommand] Error parsing JSON command.");
  }

  if (!jsonObjectCommand.containsKey("component")) {
    return Command::createJsonError("[MiletusDevice::executeCommand] JSON received does not have a component field.");
  }

  if (!jsonObjectCommand.containsKey("name")) {
    return Command::createJsonError("[MiletusDevice::executeCommand] JSON received does not have a name field.");
  }

  string name = (const char *)jsonObjectCommand["name"];
  const char *c;
  unsigned int str_aux;

  // below is a hack so we can avoid the memchr bug
  for (c = name.c_str(), str_aux = 0; str_aux < name.length(); str_aux++) {
    if (*(c + str_aux) == '.')
      break;
  }

  string trait = name.substr(0, str_aux);
  string command = name.substr(str_aux + 1);

  // maybe change components to a std:map
  auto i = components.begin();
  for (; i != components.end(); i++) {
    if (strcmp((*i)->name.c_str(), (const char *)jsonObjectCommand["component"]) == 0) {
      break;
    }
  }

  if (i == components.end()) {
    return Command::createJsonError("[MiletusDevice::executeCommand] Requested component not found.");
  }

  // maybe change commands to a std:map
  auto j = (*i)->traits[trait].commands.begin();
  for (; j != (*i)->traits[trait].commands.end(); j++) {
    if (strcmp((*j).name.c_str(), command.c_str()) == 0) {
      break;
    }
  }

  if (j == (*i)->traits[trait].commands.end()) {
    return Command::createJsonError("[MiletusDevice::executeCommand] Requested command not found.");
  }

  // TODO: check if all params are required
  if (jsonObjectCommand.containsKey("parameters")) {
    JsonObject &thisCommandParameter = jsonObjectCommand["parameters"];

    for (auto itParam : thisCommandParameter) {
      if (!(*j).parameters.count(itParam.key)) {
        string error =
            "[MiletusDevice::executeCommand] Parameter does not contain key " + std::string(itParam.key) + ".";
        return Command::createJsonError(error);
      }

      (*j).parameters[itParam.key].currentValue = itParam.value;
    }
  }

  if ((*j).run()) {
    return (*j).createJsonSuccess();
  }

  return (*j).getResponse();
}

void MiletusDevice::createInfo(string deviceName)
{
  std::string tmp = R"({
    "AccountId": "",
    "Id": "",
    "Name": "",
    "Description": "",
    "ModelManifestId": "LIBMI",
    "UiDeviceKind": "vendor",
    "Location": "",
    "LocalId": "",
    "DiscoveryTransport": {
      "hasCloud": false,
      "hasWifi": false,
      "hasLan": true,
      "hasBle": false,
      "LanTransport": {
        "ConnectionStatus": "online",
        "HttpAddress": "192.168.0.1",
        "HttpInfoPort": 8080,
        "HttpPort": 8081,
        "HttpUpdatesPort": 8082,
        "HttpsPort": 8083,
        "HttpsUpdatesPort": 8084
      },
      "CloudTransport": {
        "ConnectionStatus": "",
        "Id": ""
      },
      "WifiTransport": {
        "ConnectionStatus": "",
        "Ssid": ""
      },
      "BleTransport": {
        "ConnectionStatus": "",
        "Address": ""
      }
    }
  })";

  jsonInfoDefs = tmp.c_str();
  jsonInfo = &(infoJsonBuffer.parseObject(jsonInfoDefs));

  if (!jsonInfo->success()) {
    MiletusDevice::provider->printdbg("[MiletusDevice::createInfo] parseObject() failed.");
    return;
  }

  (*jsonInfo)["Name"] = deviceName.c_str();

  // The divice Id is set before a provider is defined
  // TODO: Get the ID from a persistent memory
  // The divice Id is set before a provider is defined
  char buffer[10];
  sprintf(buffer, "%04x", MiletusDevice::provider->getRand());
  (*jsonInfo)["Id"] = buffer;
}

void MiletusDevice::setProvider(MiletusProvider *p) {
  delete[] MiletusDevice::provider;
  MiletusDevice::provider = p;
  this->createInfo(this->name);
}

bool MiletusDevice::setState(const char *component, const char *trait, const char *states, JsonVariant value) {
  for (auto I : components) {
    if (strcmp(I->name.c_str(), component) == 0)
      return I->setState(trait, states, value);
  }
  return false;
}

bool MiletusDevice::setStateCallback(const char *component, const char *trait, const char *states,
                                     bool (*f)(const char *)) {
  for (auto I : components) {
    if (strcmp(I->name.c_str(), component) == 0)
      return I->setStateCallback(trait, states, f);
  }
  return false;
}

bool MiletusDevice::getSuccessfulInstallation() {
  return MiletusDevice::successfulInstallation;
}

std::string MiletusDevice::getInputDataPath() {
  return MiletusDevice::inputDataPath;
}

std::string MiletusDevice::getOutputDataPath() {
  return MiletusDevice::outputDataPath;
}

// COMMAND CLASS
bool Command::appendResult(string key, JsonVariant value)
{
  if (!responseJsonInitialized) {
    createJsonResponse();
    resultsJson = &responseJson->createNestedObject("results");
  }
  // TODO: Keep this value in memmory till this command die;
  (*resultsJson)[key.c_str()] = value;
  return true;
}

void Command::createJsonResponse()
{
  responseJson = &(jsonBuffer.createObject());
  (*responseJson)["state"] = "inProgress";
  responseJsonInitialized = true;
}

string Command::createJsonSuccess()
{
  if (!responseJsonInitialized) {
    createJsonResponse();
  }
  (*responseJson)["state"] = "done";
  string response;
#ifdef ARDUINO
  String responseA;
  responseJson->printTo(responseA);
  response = responseA.c_str();
#else
  responseJson->printTo(response);
#endif
  return response;
}

void Command::setParentComponent(Component *component)
{
  this->parentComponent = component;
}

void Command::setUserFunction(bool (*f)(Command &))
{
  this->userFunction = f;
  this->initialized = true;
}

bool Command::run()
{
  if (!initialized) {
    this->createJsonError("[Command::run] Attempt to run a non-initialized command.");
    MiletusDevice::provider->printdbg(this->name.c_str());
    return false;
  }
  return this->userFunction(*this);
}

void Command::abort(string errorMessage)
{
  this->response = this->createJsonError(errorMessage);
}

string Command::getJson()
{
  string json = "{\n\"";
  json += name;
  json += "\": {\n\"minimalRole\": \"";
  switch (role) {
  case viewer:
    json += "viewer\",";
    break;
  case user:
    json += "user\",";
    break;
  case manager:
    json += "manager\",";
    break;
  case owner:
    json += "owner\",";
    break;
  case robot:
    json += "robot\",";
    break;
  default:
    json += "unknown\",";
    break;
  }
  json += "\n\"initialized\":";
  json += initialized ? "true," : "false,";
  if (parameters.size() > 0) {
    json += "\n\"parameters\": {";
    for (auto i = parameters.begin(); i != parameters.end(); i++) {
      json += "\n\"";
      json += i->first;
      json += "\": {\n\"type\": \"";
      switch (i->second.type) {
      case miBoolean:
        json += "boolean\"";
        break;
      case miNumber:
        json += "number\"";
        break;
      case miString:
        json += "string\"";
        break;
      default:
        json += "unknown\"";
        break;
      }
      if (i->second.possibleValues.size() > 0) {
        json += ",\n\"values\": ";
        auto j = i->second.possibleValues.begin();
        for (; j != i->second.possibleValues.end(); j++) {
          if (j != i->second.possibleValues.begin())
            json += " ,";
          json += "\"";
          json += *j;
          json += "\"";
        }
      }
    }
    json += "\n}\n";
  }
  json += "}\n";
  return json;
}

string Command::getResponse()
{
  return this->response;
}

string Command::createJsonError(string error)
{
  MiletusDevice::provider->printdbg(std::string("[Command::createJsonError] Request error: " + error + ".").c_str());
  char *jsonResponse;
  DynamicJsonBuffer jsonBuffer;
  JsonObject &jsonObjectError = jsonBuffer.createObject();
  jsonObjectError["state"] = "error";
  ((JsonObject &)jsonObjectError.createNestedObject("error"))["code"] = error.c_str();
  jsonResponse = new char[jsonObjectError.measureLength() + 1];
  jsonObjectError.printTo(jsonResponse, jsonObjectError.measureLength() + 1);
  string jerror(jsonResponse);
  free(jsonResponse);
  return jerror;
}

JsonVariant Command::getParameter(string paramName)
{
  return this->parameters[paramName].currentValue;
}

// COMPONENT CLASS
void Component::addTrait(const char *trait, JsonObject *jsonTraits)
{
  JsonObject &traitObj = (*jsonTraits)["traits"];
  if (!traitObj.containsKey(trait)) {
    // we should rise an error here
    MiletusDevice::provider->printdbg(
        std::string("[Component::addTrait] Trait does not exist: " + std::string(trait) + ".").c_str());
    return;
  }
  // TODO: remove new
  Trait *myTrait = new Trait();
  JsonObject &selectedTrait = traitObj[trait];
  if (selectedTrait.containsKey("commands")) {
    // There is a command in this trait
    JsonObject &traitCommands = selectedTrait["commands"];
    // traitCommands.prettyPrintTo(Serial);
    for (auto itCommand : traitCommands) {
      Command *myCommand = new Command(itCommand.key);

      JsonObject &selectedCommand = traitCommands[itCommand.key];
      if (selectedCommand.containsKey("parameters")) {
        JsonObject &thisCommandParameter = selectedCommand["parameters"];
        for (auto itParam : thisCommandParameter) {
          Parameter *thisParam = new Parameter();
          JsonObject &selectedCommandParameter = thisCommandParameter[itParam.key];
          if (!selectedCommandParameter.containsKey("type")) {
            // rise an error
          }
          const char *parameterType = selectedCommandParameter["type"];
          thisParam->type = Type::getTypeFromChar(parameterType);
          // Serial.print("Type: ");
          // Serial.println(thisParam->type);
          if (selectedCommandParameter.containsKey("enum")) {
            JsonObject &thisParamEnum = selectedCommandParameter["enum"];
            for (auto itParamEnum : thisParamEnum)
              thisParam->possibleValues.push_back(itParamEnum.value.as<char *>());
          }
          myCommand->parameters[itParam.key] = *thisParam;
          delete thisParam;
        }
      }
      myCommand->setParentComponent(this);
      myTrait->commands.push_back(*myCommand);
      delete myCommand;
    }
  }
  if (selectedTrait.containsKey("state")) {
    JsonObject &selectedTraitState = selectedTrait["state"];

    // selectedTraitState.prettyPrintTo(Serial);
    for (auto itState = selectedTraitState.begin(); itState != selectedTraitState.end(); ++itState) {
      State *myState = new State();
      myState->name = (const char *)itState->key;
      JsonObject &selectedStateParameter = selectedTraitState[itState->key];

      if (!selectedStateParameter.containsKey("type")) {
        // TODO: rise an error
        MiletusDevice::provider->printdbg(
            std::string("[Component::addTrait] " + std::string(itState->key) + " state does not contain type field.")
                .c_str());
      }
      const char *stateType = selectedStateParameter["type"];
      myState->type = Type::getTypeFromChar(stateType);
      // Serial.print("Type: ");
      // Serial.println(myState->type);
      if (selectedStateParameter.containsKey("enum")) {
        JsonObject &thisStateEnum = selectedStateParameter["enum"];
        for (auto itParamEnum = thisStateEnum.begin(); itParamEnum != thisStateEnum.end(); ++itParamEnum) {
          myState->possibleValues.push_back(itParamEnum->value.as<char *>());
        }
      }
      if (selectedStateParameter.containsKey("isRequired")) {
        myState->isRequired = selectedStateParameter["isRequired"];
      } else {
        myState->isRequired = false;
      }
      if (selectedStateParameter.containsKey("minimum")) {
        myState->minimum = selectedStateParameter["minimum"];
      }
      if (selectedStateParameter.containsKey("maximum")) {
        myState->maximum = selectedStateParameter["maximum"];
      }
      if (selectedStateParameter.containsKey("unity")) {
        myState->unity = (const char *)selectedStateParameter["unity"];
      }
      myState->setParentComponent(this);
      myTrait->states[itState->key] = *myState;
      delete myState;
    }
  }
  traits[trait] = *myTrait;
  delete myTrait;
}

bool Component::setCommand(const char *trait, const char *cmd, bool (*f)(Command &))
{
  std::map<string, Trait>::iterator i = this->traits.find(trait);
  if (i == traits.end()) {
    MiletusDevice::provider->printdbg(
        std::string("[Component::setCommand] Error finding trait: " + std::string(trait) + ".").c_str());
    return false;
  }
  std::list<Command> *cmds = &this->traits[trait].commands;
  for (auto ic : *cmds) {
    if (ic.name.compare(cmd) == 0) {
      ic.setUserFunction(f);
      return true;
    }
  }
  MiletusDevice::provider->printdbg(
      std::string("[Component::setCommand] Error finding command: " + std::string(cmd) + ".").c_str());
  return false;
}

State *Component::getState(const char *trait, const char *state)
{
  std::map<string, Trait>::iterator i = this->traits.find(trait);
  if (i == traits.end()) {
    MiletusDevice::provider->printdbg(
        std::string("[Component::getState] Error finding trait: " + std::string(trait) + ".").c_str());
    return NULL;
  }
  std::map<string, State>::iterator ii = traits[trait].states.find(state);
  if (ii == traits[trait].states.end()) {
    MiletusDevice::provider->printdbg(
        std::string("[Component::getState] Error finding state: " + std::string(state) + ".").c_str());
    return NULL;
  }
  return &traits[trait].states[state];
}

bool Component::setStateCallback(const char *trait, const char *state, bool (*f)(const char *))
{
  std::map<string, Trait>::iterator i = this->traits.find(trait);
  if (i == traits.end()) {
    MiletusDevice::provider->printdbg(
        std::string("[Component::setStateCallback] Error finding trait: " + std::string(trait) + ".").c_str());
    return false;
  }
  std::map<string, State>::iterator ii = traits[trait].states.find(state);
  if (ii == traits[trait].states.end()) {
    MiletusDevice::provider->printdbg(
        std::string("[Component::setStateCallback] Error finding state: " + std::string(state) + ".").c_str());
    return false;
  }
  traits[trait].states[state].callback = f;
  return true;
}

bool Component::setState(const char *trait, const char *state, JsonVariant value)
{
  std::map<string, Trait>::iterator i = this->traits.find(trait);
  if (i == traits.end()) {
    MiletusDevice::provider->printdbg(
        std::string("[Component::setState] Error finding trait: " + std::string(trait) + ".").c_str());
    return false;
  }
  std::map<string, State>::iterator ii = traits[trait].states.find(state);
  if (ii == traits[trait].states.end()) {
    MiletusDevice::provider->printdbg(
        std::string("[Component::setState] Error finding state: " + std::string(state) + ".").c_str());
    return false;
  }
  traits[trait].states[state].value = value;
#ifdef ARDUINO
  String component_trait_state_value;
#else
  string component_trait_state_value;
#endif
  component_trait_state_value = this->name.c_str();
  component_trait_state_value += ".";
  component_trait_state_value += trait;
  component_trait_state_value += ".";
  component_trait_state_value += state;
  component_trait_state_value += "=";
#ifdef ARDUINO
  component_trait_state_value += value.as<String>().c_str();
#else
  component_trait_state_value += value.as<string>();
#endif
  on_some_state_changed(component_trait_state_value.c_str());
  if (traits[trait].states[state].callback != NULL) {
    if (!traits[trait].states[state].callback(component_trait_state_value.c_str()))
      return false;
  }
  return true;
}

string Component::getJson()
{
  string json = "";

  // json += "{\n\"";
  json += "\"";
  json += this->name;
  json += "\":\n{";
  json += "\"traits\":\n[";
  auto i = traits.begin();
  for (; i != traits.end(); i++) {
    if (i != traits.begin()) {
      json += ",";
    }
    json += "\"";
    json += i->first;
    json += "\"";
  }
  json += "\n],\n";
  json += "\"state\": {";
  i = traits.begin();
  for (; i != traits.end(); i++) {
    if (i != traits.begin()) {
      json += ",";
    }
    json += "\n\"";
    json += i->first;
    json += "\":\n{";
    json += i->second.getJson();
  }
  json += "\n}\n";
  json += "}\n";
  return json;
}

// STATE CLASS
void State::setParentComponent(Component *component)
{
  this->parentComponent = component;
}

string State::getJson()
{
  string json = "{";
  json += "\n\"value\": ";
  if (value == 0) {
    json += "\"null\"";
  } else {
#ifdef ARDUINO
    json += value.as<String>().c_str();
#else
    json += value.as<string>();
#endif
  }
  json += ",";
  if (possibleValues.size() > 0) {
    json += "\n\"values\": ";
    bool first = true;
    for (auto i : possibleValues) {
      if (!first)
        json += " ,";
      else
        first = true;

      json += "\"";
      json += i;
      json += "\"";
    }
  }
  if (isRequired) {
    json += "\n\"isRequired\": true,";
  } else {
    json += "\n\"isRequired\": false,";
  }
  switch (type) {
  case miBoolean:
    json += "\n\"type\": \"boolean\",";
    break;
  case miNumber:
    json += "\n\"type\": \"number\",";
    break;
  case miString:
    json += "\n\"type\": \"string\",";
    break;
  default:
    json += "\n\"type\": \"unknown\",";
    break;
  }

  json += "\n\"maximum\": ";
#ifdef ARDUINO
  String buffer;
#else
  std::string buffer;
#endif

  size_t n;

  n = maximum.printTo(buffer);

  if (n == 0)
    json += "\"\",";
  else
    json += buffer.c_str();

  json += "\n\"minimum\": ";

  n = minimum.printTo(buffer);

  if (n == 0)
    json += "\"\",";
  else
    json += buffer.c_str();

  json += "\n\"unity\": ";

  if (unity.empty())
    json += "\"\"";
  else
    json += unity;

  json += "\n}";

  return json;
}

// TYPE CLASS
miletusType Type::getTypeFromChar(const char *type)
{
  /*  Serial.println("Comparing types: ");
    Serial.println(type);
    Serial.println(strcmp(type, "boolean"));
    Serial.println(strcmp(type, "number"));
    Serial.println(strcmp(type, "string"));
    Serial.println(strcmp(type, "integer"));
    Serial.println(strcmp(type, "object"));*/

  if (strcmp(type, "boolean") == 0) {
    return miBoolean;
  }
  if (strcmp(type, "number") == 0) {
    return miNumber;
  }
  if (strcmp(type, "string") == 0) {
    return miString;
  }
  if (strcmp(type, "integer") == 0) {
    return miInteger;
  }
  if (strcmp(type, "object") == 0) {
    return miObject;
  }
  return miUnknown;
}

// TRAIT CLASS
string Trait::getJson()
{
  string json = "";
  std::map<string, State>::iterator j = states.begin();
  while (j != states.end()) {
    json += "\"";
    json += j->first;
    json += "\": \n";
    if (j->second.value == 0) {
      json += "\"null\"";
    } else {
#ifdef ARDUINO
      json += j->second.value.as<String>().c_str();
#else
      json += j->second.value.as<string>();
#endif
    }
    j++;
    if (j != states.end())
      json += ",\n";
  }
  json += "\n}";
  return json;
}
