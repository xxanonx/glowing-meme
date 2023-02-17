int full_speed = 120;
int half_speed = 90;
int turn_speed = 90;

//Right side
int enable_motorA = 3;
int motor_in1 = 4;
int motor_in2 = 5;
int IR_right = 10;
int right;
//Left side
int motor_in3 = 6;
int motor_in4 = 7;
int enable_motorB = 9;
int IR_left = 11;
int left;

void forward(int motor_speed, int enable, int input1, int input2){
  digitalWrite(input1, HIGH);
  digitalWrite(input2, LOW);
  analogWrite(enable, motor_speed);
}

void backward(int motor_speed, int enable, int input1, int input2){
  digitalWrite(input1, LOW);
  digitalWrite(input2, HIGH);
  analogWrite(enable, motor_speed);
}

void stop_motor(int enable, int input1, int input2){
  digitalWrite(input1, LOW);
  digitalWrite(input2, LOW);
  digitalWrite(enable, LOW);
}

void setup() {
  // put your setup code here, to run once:
  //Right side
  pinMode(enable_motorA, OUTPUT);
  pinMode(motor_in1, OUTPUT);
  pinMode(motor_in2, OUTPUT);
  pinMode(IR_right, INPUT);

  pinMode(motor_in3, OUTPUT);
  pinMode(motor_in4, OUTPUT);
  pinMode(enable_motorB, OUTPUT);
  pinMode(IR_left, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  /*
  //FORWARD
  forward(255, enable_motorA, motor_in1, motor_in2);
  forward(255, enable_motorB, motor_in3, motor_in4);
  delay(2000);
  //BACKWARD
  backward(255, enable_motorA, motor_in1, motor_in2);
  backward(255, enable_motorB, motor_in3, motor_in4);
  delay(2000);
  //SPIN LEFT
  forward(255, enable_motorA, motor_in1, motor_in2);
  backward(255, enable_motorB, motor_in3, motor_in4);
  delay(2000);
  //SPIN RIGHT
  backward(255, enable_motorA, motor_in1, motor_in2);
  forward(255, enable_motorB, motor_in3, motor_in4);
  delay(2000);
  stop_motor(enable_motorA, motor_in1, motor_in2);
  stop_motor(enable_motorB, motor_in3, motor_in4);
  */
  right = digitalRead(IR_right);
  left = digitalRead(IR_left);

  if (not right and not left){
    forward(half_speed, enable_motorA, motor_in1, motor_in2);
    forward(half_speed, enable_motorB, motor_in3, motor_in4);
  }
  else if (not right and left){
    forward(half_speed, enable_motorA, motor_in1, motor_in2);
    backward(turn_speed, enable_motorB, motor_in3, motor_in4);
    delay(10);
  }
  else if (right and not left){
    backward(turn_speed, enable_motorA, motor_in1, motor_in2);
    forward(half_speed, enable_motorB, motor_in3, motor_in4);
    delay(10);
  }
  else{
    stop_motor(enable_motorA, motor_in1, motor_in2);
    stop_motor(enable_motorB, motor_in3, motor_in4);
    delay(100);
  }

}
