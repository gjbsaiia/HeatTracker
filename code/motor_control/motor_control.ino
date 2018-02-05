//Griffin Saiia, Motor Control

// connect motor controller pins to Arduino digital pins
// motor 1
int enA = 10;
int in1 = 9;
int in2 = 8;
// motor 2
int enB = 5;
int in3 = 7;
int in4 = 6;
//control from RPi
char mode = 'a';


void setup()
{
  // set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  Serial.begin(9600);
}

void serialEvent(){
  while(Serial.available()){
    mode = (char)(Serial.read());
  }
}

void off(){
  //turn off motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enA, 0);
  analogWrite(enB, 0);
}

void drive(){
  //turn on motor 1
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  //turn on motor 2
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  //set speed
  analogWrite(enA, 250);
  analogWrite(enB, 250);
  delay(15000);
  off();
}

void left(){
  //turn on motor 2
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, 200);
  analogWrite(enB, 250);
  delay(6000);
  off();
}

void right(){
  //turn on motor 1
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, 200);
  analogWrite(enA, 250);
  delay(6000);
  off();
}

void reverse(){
  //turn on motor 1
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  //turn on motor 2
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  //set speed
  analogWrite(enA, 250);
  analogWrite(enB, 250);
  delay(15000);
  off();
}

void loop()
{
  if(mode == '1'){
    mode = 'a';
    drive();
  }
  if(mode == '2'){
    mode = 'a';
    left();
  }
  if(mode == '3'){
    mode = 'a';
    right();
  }
  if(mode == '4'){
    mode = 'a';
    reverse();
  }
  if(mode == '5'){
    mode = 'a';
    off();
  }
}



