#include <JameSpecialBool.h>

Timer outlier_found_off_delay(1000, 0);
const bool troubleshoot = false;

int anal = A0;
int val = 0;
const int sizeOfLast = 20;
int Last[sizeOfLast];
int indexOfLast;
int sortLast[sizeOfLast];
int calc_Q1, calc_Q2, calc_Q3, IQR, UpperFence, LowerFence;

void sort() {
  int smallest, index, numSorted;
  int tempSort[sizeOfLast];
  //Serial.println("copying for sorting");
  numSorted = 0;

  for (index = 0; index < sizeOfLast; index++) {
    tempSort[index] = Last[index];
    //Serial.print(tempSort[index]);
    //Serial.print(",");
  }
  //Serial.println("\n");

  do {
    smallest = 0;
    for (index = 0; index < sizeOfLast; index++) {
      //Serial.print(tempSort[index]);
      //Serial.print(",");
    }
    //Serial.println("\n");
    for (index = 0; index < sizeOfLast; index++) {
      if (((tempSort[index]) < 0)) {
        if (smallest == index) {
          smallest += 1;
        }
        if (smallest >= sizeOfLast) {
          smallest = 0;
        }
        if (troubleshoot) Serial.println("SKIP!!!");
        continue;
      }
      if (troubleshoot){
        Serial.print(tempSort[index]);
        Serial.print(",");
        Serial.print(index);
        Serial.print(",");
        Serial.print(tempSort[smallest]);
        Serial.print(",");
        Serial.println(smallest);
      }
      if (tempSort[smallest] >= 0) {
        if (tempSort[index] == min(tempSort[index], tempSort[smallest])) {
          smallest = index;
        }
      }
    }
    if (tempSort[smallest] >= 0) {
      //Serial.println("Writing to sorted list");
      sortLast[numSorted] = tempSort[smallest];
      tempSort[smallest] = -10;
      //Serial.print(tempSort[smallest]);
      //Serial.print(",");
      //Serial.println(sortLast[numSorted]);
      numSorted += 1;

    }
  } while (numSorted < sizeOfLast);
  if (troubleshoot){
    for (int index = 0; index < sizeOfLast; index++) {
      Serial.print(sortLast[index]);
      Serial.print(",");
    }
    Serial.print("\n");
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(74880);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("begin");
}

void loop() {
  // put your main code here, to run repeatedly:
  val = analogRead(anal);
  Last[indexOfLast] = val;
  indexOfLast += 1;
  if (indexOfLast >= sizeOfLast) {
    indexOfLast = 0;
  }

  sort();

  //for (int index = 0; index < sizeOfLast; index++) {
  //Serial.print(int(sortLast[index]));
  //Serial.print(",");
  //}
  //Serial.print("\n");
  //delay(500);
  calc_Q2 = (sortLast[int(sizeOfLast * 0.5)]);
  calc_Q1 = (sortLast[int(sizeOfLast * 0.25)]);
  calc_Q3 = (sortLast[int(sizeOfLast * 0.75)]);
  IQR = (calc_Q3 - calc_Q1);
  IQR = max(IQR, 10);
  LowerFence = (calc_Q1 - (1.5 * IQR));
  UpperFence = (calc_Q3 + (1.5 * IQR));
  digitalWrite(LED_BUILTIN, outlier_found_off_delay.input(((val < LowerFence) or (val > UpperFence))));

  if (troubleshoot){
    Serial.print("LowerFence: ");
    Serial.print(LowerFence);
    Serial.print("\t UpperFence: ");
    Serial.println(UpperFence);
  }
  Serial.println(val);
  delay(15);
}
