//The reason of making this program was to log what my lighting panel PLC was doing over the weekend and if it was doing what I
//Programmed it to do. Every hour the arduino logs the state of two ouputs from the PLC. The arduino saves the log to it's EEPROM or
//electrically erasable programmable read-only memory

#include <EEPROM.h>

//Declare variables
const int temp = A0;
const int arraysize = 24;   // 72 hours == 3 days
byte temp_array[arraysize];
unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 600000;  //the value is a number of milliseconds 3600000 == 1 hour
int address = 1;
bool rewrite_bit = false;
bool read_eeprom = false;

//const float
void serial_interact(){
  if (Serial.available() > 0) {
    String message = Serial.readString();
  
    if (message.substring(0, 8) == "/Rewrite") {
      rewrite_bit = true;
      EEPROM.update(0, 0);
    }
    if (message.substring(0, 5) == "/Read") {
      read_eeprom = true;
    }
  }
}


void setup() {
  // put your setup code here, to run once:
  pinMode(temp, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  startMillis = millis();  //initial start time
  if (EEPROM.read(0) == 1) { //address 0 is the byte telling that the EEPROM save was successful
    Serial.println("EEPROM save complete, send \"/Rewrite\" to rewrite EEPROM, or send \"/Read\" to read EEPROM, you have 20 seconds...");
    do {
      currentMillis = millis();
      serial_interact();
      if (read_eeprom) break;

    } while ((currentMillis - startMillis <= 20000) && (rewrite_bit == false));
  }
  else{
    rewrite_bit = true;
    Serial.println("Overwriting EEPROM");
  }
  
}



void send_info() {
  Serial.println("trying to print results");
  for (int i = 0; i <= (arraysize - 1); i++) {
    Serial.print("Hour:\t");
    Serial.println(i + 1);
    Serial.print(temp_array[i]);
    Serial.println("\n\n");
    delay(500);
  }
}



void rewrite_data() {
  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)

  if (currentMillis - startMillis >= period) { //test whether the period has elapsed
    //if ((outside[(arraysize - 1)] != 0) && (inside[(arraysize - 1)] != 0)) {
    if (address > arraysize) {
      EEPROM.update(0, 1);
      while (1) {
        digitalWrite(LED_BUILTIN, HIGH);    //endless loop letting user know the cycle is done
        delay(1000);
        digitalWrite(LED_BUILTIN, LOW);
        delay(1000);
      }
    }
    else {
      int temp_avg_val = 0;
      digitalWrite(LED_BUILTIN, LOW);
      for (int i = 0; i < 10; i++){
        delay(10);
        float temp_val = 0;
        temp_val = analogRead(temp); //0-1023 each increment is 0.00488 V
        temp_val = ((temp_val * 0.00488) / 0.010);
        temp_val = ((temp_val * 1.8) + 32);
        temp_avg_val += int(temp_val);
      }
      temp_avg_val /= 10;
      EEPROM.update(address, byte(temp_avg_val));
      address += 1;
      digitalWrite(LED_BUILTIN, HIGH);
    }
    startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
  }
}


void read_data() {
  if (read_eeprom) {
    for (int i = 1; i <= arraysize; i++) {
      temp_array[(i - 1)] = EEPROM.read(i);
    }
    send_info();
    read_eeprom = false;
  }
  else {
    Serial.println("ready to read");
    digitalWrite(LED_BUILTIN, HIGH);    //endless loop letting user know the cycle is done
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
  }
}



void loop() {
  // put your main code here, to run repeatedly:
  if (rewrite_bit == false){
    read_data();
    serial_interact();
  }
  else rewrite_data();
  /*
    delay(10);
    float temp_val = 0;
    temp_val = analogRead(temp); //0-1023 each increment is 0.00488 V
    temp_val = ((temp_val * 0.00488) / 0.010);
    temp_val = ((temp_val * 1.8) + 32);
  */

}
