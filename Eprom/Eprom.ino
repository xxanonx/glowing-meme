byte addr0 = 2, addr1 = 3, addr2 = 4, addr3 = 5, addr4 = 6, addr5 = 7, addr6 = 8;
byte address[7] = {addr0, addr1, addr2, addr3, addr4, addr5, addr6};

byte inout0 = A0, inout1 = A1, inout2 = A2, inout3 = A3, inout4 = A6, inout5 = A7, inout6 = 9, inout7 = 10;
byte write_read_value[8] = {inout0, inout1, inout2, inout3, inout4, inout5, inout6, inout7};

byte OE = 11;
byte CE = 12;

bool program_enable = false;
const char ending_char = 4; //end of transmission
int last_loc;
const int max_address = 127;


#include "functions.h"


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  for (int i = 0; i < 6; i++) {
    pinMode(address[i], OUTPUT);
    digitalWrite(address[i], LOW);
  }
  pinMode(CE, OUTPUT);
  digitalWrite(CE, HIGH);
  pinMode(OE, OUTPUT);
  digitalWrite(OE, LOW);

  count_eprom();

  Serial.println("Ready for reading");
}


void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    String message = Serial.readString();
    //Serial.println("Arduino got " + message);


    if (message.substring(0, 4) == "help") {
      //HELP
      Serial.println("\n\nCommand List:\n");
      Serial.println("\thelp");
      Serial.println("\t/r message#\t(reads message number)");
      Serial.println("\t/w message\t(writes message)");
      Serial.println("\t/c\t\t(counts messages)");
    }


    else if (message.substring(0, 3) == "/r ") {
      //READ
      long mes_num;
      message.remove(0, 3);
      mes_num = message.toInt();

      //maybe check how many messages there are and see if the mes_num is less than that

      Serial.print("Reading message number: ");
      Serial.println(mes_num);
      set_address(mes_num);
      read_eprom();
    }


    else if (message.substring(0, 3) == "/w ") {
      //WRITE
      message.remove(0, 3);

      //add some ending character
      char mes_char[(message.length() + 1)];
      for (unsigned int i = 0; i < message.length(); i++) {
        mes_char[i] = message.charAt(i);
      }
      mes_char[(message.length() + 1)] = ending_char;

      if (((sizeof(mes_char)) + last_loc + 1) <= max_address){
        set_values_rw(true);
        Serial.print("Writing message: ");
        Serial.println(message);
        last_loc += 1;
        for (int i = 0; i < (sizeof(mes_char)); i++) {
          if(set_address((last_loc + i))){
            Serial.println(write_eprom(mes_char[i]) ? "char sucessful" : "char failed");
          }
        }
        count_eprom();
        //find last message and write to addresses after it
        //function for writing message and what address
        //serial print message number
      }
    }


    else if (message.substring(0, 2) == "/c") {
      //COUNT
      //Index through all addresses until all zeros and count amount of ending characters
      count_eprom(true);
    }


    else {
      //UNKNOWN
      Serial.println("That command \"" + message + "\"is not recognized");
    }
  }
}
