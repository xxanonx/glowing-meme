//RELAY OUTPUTS
int button_light_r1 = 52;
int horn_r2 = 50;
int speakers_enable_relay3 = 48;
/*
  int spare_relay4 = 46;
  int spare_relay5 = 44;
  int spare_relay6 = 42;
  int spare_relay7 = 40;
  int spare_relay8 = 38;
*/

//5 VOLT OUTPUTS
int music_pwm = 2;

//24 VOLT INPUTS
int push_button_24i1 = 53;
int speaker_selector_24i2 = 51;
/*
  int spare_24v_input3 = 49;
  int spare_24v_input4 = 47;
  int spare_24v_input5 = 45;
  int spare_24v_input6 = 43;
  int spare_24v_input7 = 41;
  int spare_24v_input8 = 39;
*/

//Memory bools and ints and shit
bool speakers_enable_mb = false;
bool pulse_on_250, pulse_on_100;
unsigned long currentMillis, previousMillis_250, previousMillis_100, interval_250ms = 250, interval_100ms = 100;
bool button_light_on;


//MUSIC RELATED
int track_num, count_track_num;
bool enable_play;
bool first_blink, start_pause, blink_pulse;
unsigned long song_sel_cur_millis, song_sel_prev_millis, song_sel_play_song_time = 1000, blink_sleep_prev_millis, blink_sleep_time = 60000;
bool pushbutton_falling_edge, pushbutton_rising_edge;
unsigned long show_song_pause, previous_millis_song_blink;


void pulse_functions() {
  //unsigned long currentMillis, previousMillis_250, previousMillis_500, interval_250ms = 250, interval_500ms = 500;
  //bool timer_off_250 = true, timer_off_500 = true;

  currentMillis = millis();
  if (currentMillis - previousMillis_250 >= interval_250ms) {
    previousMillis_250 = currentMillis;
    if (pulse_on_250 == true) {
      pulse_on_250 = false;
    }
    else {
      pulse_on_250 = true;
    }
  }

  if (currentMillis - previousMillis_100 >= interval_100ms) {
    previousMillis_100 = currentMillis;
    if (pulse_on_100 == true) {
      pulse_on_100 = false;
      //digitalWrite(LED_BUILTIN, LOW);
    }
    else {
      pulse_on_100 = true;
      //digitalWrite(LED_BUILTIN, HIGH);
    }
  }
}



void blink_track_number(int number) {

  if (first_blink or (start_pause and ((currentMillis - previous_millis_song_blink) >= show_song_pause))) {
    if (first_blink) {
      first_blink = false;
      //Serial.println("first blink");
    }
    count_track_num = number;
    start_pause = false;
    //Serial.print("count_track_num = ");
    //Serial.println(count_track_num);
  }

  if (not start_pause) {
    if ((pulse_on_250) and (count_track_num > 0)) {
      if (blink_pulse) {
        count_track_num -= 1;
        button_light_on = true;
        blink_pulse = false;
        //Serial.print("count is at: ");
        //Serial.println(count_track_num);
      }
    }
    else {
      if (not pulse_on_250) {
        button_light_on = false;
        blink_pulse = true;
        if ((count_track_num == 0)) {
          previous_millis_song_blink = currentMillis;
          start_pause = true;
          show_song_pause = number * interval_250ms * 2;
        }
      }
    }
  }
}


//INCLUDES
#include "Doom.h"

void setup() {
  // put your setup code here, to run once:
  pinMode(push_button_24i1, INPUT_PULLUP);
  pinMode(speaker_selector_24i2, INPUT);
  pinMode(button_light_r1, OUTPUT);
  pinMode(horn_r2, OUTPUT);
  pinMode(speakers_enable_relay3, OUTPUT);
  pinMode(music_pwm, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  pulse_functions();

  if (digitalRead(speaker_selector_24i2) == LOW) {
    //if horn selected
    if (speakers_enable_mb) {
      speakers_enable_mb = false;
      button_light_on = false;
    }
  }

  else {
    //if speaker selected
    if (not speakers_enable_mb) {
      speakers_enable_mb = true;
      enable_play = false;
      track_num = 6;
      pushbutton_falling_edge = true;
      pushbutton_rising_edge = false;
      button_light_on = false;
    }

    else if (enable_play) {
      //Serial.print("playing track ");
      //Serial.println(track_num);
      play(track_num);
      enable_play = false;
    }

    else {
      //CHOOSE SONG AND PLAY IT
      song_sel_cur_millis = millis();

      if ((digitalRead(push_button_24i1) == HIGH) and (not pushbutton_rising_edge)) {
        //RISING EDGE PULSE OF BUTTON WHILE SPEAKERS ENABLED
        pushbutton_rising_edge = true;
        song_sel_prev_millis = song_sel_cur_millis;
        blink_sleep_prev_millis =  song_sel_cur_millis;
        pushbutton_falling_edge = false;
      }
      else if ((digitalRead(push_button_24i1) == HIGH) and (pushbutton_rising_edge)) {
        //BUTTON PUSHED BUT NO EDGE WHILE SPEAKERS ENABLED
        button_light_on = true;
      }
      else if ((digitalRead(push_button_24i1) == LOW) and (not pushbutton_falling_edge)) {
        //FALLING EDGE PULSE OF BUTTON WHILE SPEAKERS ENABLED
        pushbutton_falling_edge = true;
        pushbutton_rising_edge = false;
        if (song_sel_cur_millis - song_sel_prev_millis > song_sel_play_song_time) {
          enable_play = true;
        }
        else if (song_sel_cur_millis - song_sel_prev_millis > 20) {
          track_num += 1;
          first_blink = true;
          if (track_num > 6) {
            track_num = 1;
          }
          //Serial.println(track_num);
        }
      }
      else if ((digitalRead(push_button_24i1) == LOW) and (pushbutton_falling_edge)) {
        //blink light the amount of time as the track number
        if (not (song_sel_cur_millis - blink_sleep_prev_millis > blink_sleep_time)) {
          blink_track_number(track_num);
        }
        else{
          button_light_on = false;
        }
    }
  }
}

//BELOW OUTPUTS! VERY IMPORTANT!

//HORN OUTPUT
if ((digitalRead(push_button_24i1) == HIGH) and (speakers_enable_mb == false)) {
  digitalWrite(horn_r2, LOW);
}
else {
  digitalWrite(horn_r2, HIGH);
}

//BUTTON LIGHT OUTPUT
if (((digitalRead(push_button_24i1) == HIGH) and (speakers_enable_mb == false)) or (button_light_on)) {
  if ((pulse_on_250 == true) or button_light_on) {
    digitalWrite(button_light_r1, LOW);
  }
  else {
    digitalWrite(button_light_r1, HIGH);
  }
}
else {
  digitalWrite(button_light_r1, HIGH);
}

//SPEAKER OUTPUT
if (speakers_enable_mb) {
  digitalWrite(speakers_enable_relay3, LOW);
}
else {
  digitalWrite(speakers_enable_relay3, HIGH);
}

}
