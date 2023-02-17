//FUNCTIONS
void mode_selection(int mode = 0){
  if (2 >= mode >= 0){
    delay(1);
    switch(mode){
      case 0:
        //stand by
        digitalWrite(CE, HIGH);
        digitalWrite(OE, LOW);

      case 1:
        //read
        digitalWrite(CE, LOW);
        digitalWrite(OE, LOW);

      case 2:
        //write
        digitalWrite(CE, LOW);
        digitalWrite(OE, HIGH);
    }
  delay(2);
  }
}


void set_values_rw(bool write_values = false) {
  if (write_values) {
    for (int i = 0; i < 7; i++) {
      pinMode(write_read_value[i], OUTPUT);
      digitalWrite(write_read_value[i], LOW);
    }
  }
  else if (!write_values) {
    for (int i = 0; i < 7; i++) {
      pinMode(write_read_value[i], INPUT);
    }
  }
}


bool set_address(unsigned int addr_num) {
  if (addr_num <= max_address) {
    String addr_str = String(addr_num, BIN);
    //Serial.println(addr_str);
    int add_len = addr_str.length();
    if (add_len < 7) {
      for (int i = 7; i > add_len; i--) {
        addr_str = String("0" + addr_str);
      }
    }
    add_len = addr_str.length();
    //Serial.println(add_len);
    //Serial.println(addr_str);

    char addr_char[add_len];
    for (unsigned int i = 0; i < add_len; i++) {
      addr_char[i] = addr_str.charAt(i);
    }
    for (int i = 0; i < add_len; i++) {
      int reverse = (add_len - 1 - i);
      //Serial.println(addr_char[reverse]);
      if (addr_char[reverse] == '1') {
        digitalWrite(address[i], HIGH);
      }
      else {
        digitalWrite(address[i], LOW);
      }

    }
    return true;
  }
  else {
    Serial.println("address too high");
    return false;
  }
}


char read_eprom() {
  set_values_rw();
  mode_selection(1);
  char character[8];
  char chan;
  for (int i = 0; i < 7; i++) {
    if (digitalRead(write_read_value[i]) == HIGH) {
      character[i] = '1';
      chan += pow(2, i);
    }
    else {
      character[i] = '0';
    }
  }
  mode_selection();
  Serial.print(character);
  Serial.print("\t\t");
  Serial.println(chan);
  return chan;
}


bool write_eprom(char letter) {
  char let_temp = letter;
  set_values_rw(true);
  for (int i = 7; i >= 0; i--) {

    if ((let_temp - (pow(2, i))) >= 0) {
      digitalWrite(write_read_value[i], HIGH);
      let_temp -= pow(2, i);
    }
    else {
      digitalWrite(write_read_value[i], LOW);
    }
  }
  
  mode_selection(2);
  delay(5);
  mode_selection(0);

  for (int i = 0; i < 7; i++) {
    digitalWrite(write_read_value[i], LOW);
  }

  if (letter == read_eprom()) {
    return true;
  }
  else {
    return false;
  }
}


int count_eprom(bool verb = false) {
  set_values_rw();
  int count = 0;
  int loc;
  for (int i = 0; i <= max_address; i++) {
    if (set_address(i)) {
      if (ending_char == read_eprom()) {
        count += 1;
        loc = i;
      }
    }
  }
  last_loc = loc;
  if (verb){
    Serial.print("The number of messages is: ");
    Serial.println(count);
  }
  return count;
}
