#include <Adafruit_NeoPixel.h>
#include <Ultrasonic.h>
#define NUMPIXELS 47

Ultrasonic sonic(3, 2);
Adafruit_NeoPixel pixels(NUMPIXELS, 6, NEO_GRB + NEO_KHZ800);

int distance, prev_distance;
const int sample_size = 20;
int dis_ma[sample_size];
int enum_sample = 0;

int num_of_pix;
const int pix_sample_size = 7;
int pix_ma[pix_sample_size];
int pix_enum_sample = 0;

bool start_display = false;
bool from_red = true;

const int green_max = 15;
int green_cur = 0;

unsigned long prevmil, curmil;
unsigned long period = 100;

int safe_dist = 80;
int max_unsafe = 20;

void setup() {
  // put your setup code here, to run once:
  pixels.begin();
  Serial.begin(9600);
  delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:
  curmil = millis();

  //pixels.clear();
  dis_ma[enum_sample] = sonic.read();
  delay(10);
  enum_sample += 1;
  if (enum_sample == sample_size) {
    enum_sample = 0;
    start_display = true;
  }

  if (start_display) {
    distance = 0;
    for (int i = 0; i < sample_size; i++) {
      distance += dis_ma[i];
    }
    distance /= sample_size;
    //Serial.println(distance);

    if ((curmil >= prevmil + period) or (distance != prev_distance)) {
      pixels.clear();
      
      pix_ma[pix_enum_sample] = map(distance, max_unsafe, safe_dist, NUMPIXELS, 0);
      pix_enum_sample += 1;

      if (pix_enum_sample == pix_sample_size) {
        pix_enum_sample = 0;
      }
      num_of_pix = 0;
        for (int q = 0; q < pix_sample_size; q++) {
          num_of_pix += pix_ma[q];
        }
        num_of_pix /= pix_sample_size;
      
      if (num_of_pix > 0) {
        // Less than safe distance
        //distance <= safe_dist
        from_red = true;
        green_cur = 0;
        //num_of_pix = map(distance, max_unsafe, safe_dist, NUMPIXELS, 0);
        
        //
        Serial.println(num_of_pix);
        for (int u = 0; u < num_of_pix; u++) {
          //Serial.println(pixels.getPixelColor(u), BIN);
          pixels.setPixelColor(u, pixels.Color((2 + (2 * u)), 0, 0));
          //Serial.print("setting pixel: ");
          //Serial.println(u);
        }
        pixels.show();
      }
      else {
        //safe distance
        if (from_red){
          pixels.fill(pixels.Color(0, green_cur, 0));
          green_cur += 1;
          delay(20);
          if (green_cur == green_max){
            from_red = false;
          }
        }
        else{
          pixels.fill(pixels.Color(0, green_max, 0));
        }
        pixels.show();
      }

      prev_distance = distance;
      prevmil = curmil;
    }
  }

  /*

    if (distance <= (NUMPIXELS)){
    // Less than safe distance
    num_of_pix = map(sonic.read(),0, NUMPIXELS, NUMPIXELS, 0);
    Serial.print(num_of_pix);
    for(int u = 0; u <= num_of_pix; u++){
      pixels.setPixelColor(u, pixels.Color(255, 0, 0));
      //Serial.print("setting pixel: ");
      //Serial.println(u);
    }
    pixels.show();
    }
    else{
    //safe distance
    pixels.fill(pixels.Color(0, 50, 0));
    pixels.show();
    }*/
  //delay(50);
}
