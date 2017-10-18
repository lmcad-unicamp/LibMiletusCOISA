volatile float nomevariavel1float = 0;
volatile int nomevariavel2int = 0;
void setup() {
  Serial.begin(115200);
  uint64_t local_vm_cycle_count_start = ESP.getCycleCount();
  
  for(unsigned int i = 0; i<100000; i++){
    nomevariavel2int++;
    nomevariavel2int += nomevariavel1float;
    nomevariavel1float+ 4.11;
    nomevariavel1float -= 2;
    nomevariavel2int += 3;
    nomevariavel1float *= 0.23;
    nomevariavel2int -=4;
  }
  uint64_t local_vm_cycle_count_end = ESP.getCycleCount();
  Serial.printf("Total cycles: %u", local_vm_cycle_count_end - local_vm_cycle_count_start); 
}

void loop() {
  // put your main code here, to run repeatedly:

}
