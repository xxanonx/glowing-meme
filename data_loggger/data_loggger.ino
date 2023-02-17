//The reason of making this program was to log what my lighting panel PLC was doing over the weekend and if it was doing what I
//Programmed it to do. Every hour the arduino logs the state of two ouputs from the PLC. The arduino saves the log to it's EEPROM or
//electrically erasable programmable read-only memory

#include <EEPROM.h>

//Declare variables
const int lookin = {2};
const int lookout = {3};
const int send_button = {4};
const int arraysize = 72;   // 72 hours == 3 days
byte outside[arraysize];
byte inside[arraysize];
unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 3600000;  //the value is a number of milliseconds 3600000 == 1 hour
//int currenthour = 0;
int outaddress = 1;
int inaddress = (arraysize + 1);
bool rewrite_bit = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(lookout, INPUT);
  pinMode(lookin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  startMillis = millis();  //initial start time
  if (EEPROM.read(0) == 1) { //address 0 is the byte telling that the EEPROM save was successful
    Serial.println("Hold the button to Rewrite data, Do nothing to read data. You have 10 sec.");
    do {
      currentMillis = millis();
      if (digitalRead(send_button) == HIGH) {
        rewrite_bit = true;
        EEPROM.update(0, 0);
      }
      digitalWrite(LED_BUILTIN, HIGH);
      delay(400);
      digitalWrite(LED_BUILTIN, LOW);
      delay(400);
    } while ((currentMillis - startMillis <= 15000) && (rewrite_bit == false));
  }
  else rewrite_bit = true;
}


void send_info() {
  Serial.println("trying to print results");
  for (int i = 0; i <= (arraysize - 1); i++) {
    /*Serial.print(inside[i]);      //the first variable for plotting
    Serial.print(",");              //seperator
    Serial.println(outside[i]);          //the second variable for plotting including line break
    */
    Serial.print("Hour:\t");
    Serial.println(i + 1);
    Serial.print("Inside lights:\t");
    if (inside[i] == 10) Serial.println("OFF");
    else if (inside[i] == 250) Serial.println("ON");
    else Serial.println("ERROR");
    Serial.print("Outside lights:\t");
    if (outside[i] == 10) Serial.println("OFF");
    else if (outside[i] == 250) Serial.println("ON");
    else Serial.println("ERROR");
    Serial.println("");
    Serial.println("");
    delay(500);
  }
}



void rewrite_data() {
  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)

  if (currentMillis - startMillis >= period) { //test whether the period has elapsed
    //if ((outside[(arraysize - 1)] != 0) && (inside[(arraysize - 1)] != 0)) {
    if (outaddress > arraysize) {
      EEPROM.update(0, 1);
      while (1) {
        digitalWrite(LED_BUILTIN, HIGH);    //endless loop letting user know the cycle is done
        delay(1000);
        digitalWrite(LED_BUILTIN, LOW);
        delay(1000);
        Serial.println("ready");
        //send_info();
      }
    }
    else {
      byte inval;
      byte outval;
      if (digitalRead(lookin) == HIGH) {
        inval = 250;
        Serial.println("Inside on");
      }
      else if (digitalRead(lookin) == LOW) {
        inval = 10;
        Serial.println("Inside off");
      }
      if (digitalRead(lookout) == HIGH) {
        outval = 250;
        Serial.println("Outside on");
      }
      else if (digitalRead(lookout) == LOW) {
        outval = 10;
        Serial.println("Outside off");
      }
      //where to write data to EEPROM
      EEPROM.update(outaddress, outval);
      outaddress += 1;
      EEPROM.update(inaddress, inval);
      inaddress += 1;
    }
    startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
    //digitalWrite(LED_BUILTIN, HIGH);
    //delay(1000);
    //digitalWrite(LED_BUILTIN, LOW);
  }
  if (digitalRead(send_button) == HIGH) {
    digitalWrite(LED_BUILTIN, HIGH);
    //Serial.println("low");
  }
  else {
    digitalWrite(LED_BUILTIN, LOW);
    //Serial.println("High");
  }
  delay(100);
}


void read_data() {
  if (digitalRead(send_button) == HIGH) {
    for (int i = 1; i <= arraysize; i++) {
      outside[(i - 1)] = EEPROM.read(i);
      inside[(i - 1)] = EEPROM.read(i + arraysize);
    }
    send_info();
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
  if (rewrite_bit == false) read_data();
  else if (rewrite_bit == true) rewrite_data();
}
