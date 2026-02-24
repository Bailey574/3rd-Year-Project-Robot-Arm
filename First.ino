#include <Servo.h>

Servo myServo, myServo2, myServo3, myServo4, myServo5, myServo6;

int potPin  = A0;
int potPin2 = A1;
int potPin3 = A2;
int potPin4 = A3;
int potPin5 = A4;   // NEW pot for servo 6

void setup() {
  myServo.attach(9);
  myServo2.attach(10);
  myServo3.attach(5);
  myServo4.attach(3);
  myServo5.attach(6);
  myServo6.attach(11);   // NEW servo on pin 11
}

void loop() {

  // ---- SERVO 1 & MIRRORED SERVO 2 ----
  int val = analogRead(potPin);
  val = map(val, 0, 1023, 0, 180);
  int oppositeVal = 180 - val;

  myServo.write(val);
  myServo2.write(oppositeVal);

  // ---- SERVO 3 ----
  int val2 = analogRead(potPin2);
  val2 = map(val2, 0, 1023, 0, 180);
  myServo3.write(val2);

  // ---- SERVO 4 ----
  int val3 = analogRead(potPin3);
  val3 = map(val3, 0, 1023, 0, 180);
  myServo4.write(val3);

  // ---- SERVO 5 ----
  int val4 = analogRead(potPin4);
  val4 = map(val4, 0, 1023, 0, 180);
  myServo5.write(val4);

  // ---- SERVO 6 (NEW) ----
  int val5 = analogRead(potPin5);
  val5 = map(val5, 0, 1023, 0, 180);
  myServo6.write(val5);

  delay(20);
}
