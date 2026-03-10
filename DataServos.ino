#include <Servo.h>

Servo myServo, myServo2, myServo3, myServo4, myServo5, myServo6;

const int HOME_S1 = 120, HOME_S3 = 45, HOME_S4 = 90, HOME_S5 = 34, HOME_S6 = 76;
const int SCAN_S1 = 120, SCAN_S3 = 110, SCAN_S4 = 100, SCAN_S5 = 20, SCAN_S6 = 34;
const int RED_S1 = 120, RED_S3 = 45, RED_S4 = 90, RED_S5 = 34, RED_S6 = 0;
const int GREEN_S1 = 120, GREEN_S3 = 23, GREEN_S4 = 90, GREEN_S5 = 34, GREEN_S6 = 0;
const int BLUE_S1 = 120, BLUE_S3 = 67, BLUE_S4 = 90, BLUE_S5 = 34, BLUE_S6 = 0;

int currentS1 = HOME_S1, currentS3 = HOME_S3, currentS4 = HOME_S4, currentS5 = HOME_S5, currentS6 = HOME_S6;

int readAnalogAngle(int pin) {
  int val = analogRead(pin);            
  int angle = map(val, 0, 1023, 0, 180); 
  return angle;
}

void printGripperAngle() {
  Serial.println(currentS6); 
}


void printForExcel() {
  Serial.print(readAnalogAngle(A5)); 
  Serial.print(",");
  Serial.println(readAnalogAngle(A4)); 
  //Serial.print(",");
  //Serial.println(readAnalogAngle(A3));
}

void moveServoSmooth(Servo &servo, int fromAngle, int toAngle, int stepDelay = 10) {
  if (fromAngle < toAngle) {
    for (int a = fromAngle; a <= toAngle; a++) {
      servo.write(a);
      delay(stepDelay);
      //printForExcel();
      printGripperAngle();
    }
  } else {
    for (int a = fromAngle; a >= toAngle; a--) {
      servo.write(a);
      delay(stepDelay);
      //printForExcel();
      printGripperAngle();
    }
  }
}

void moveServoMirrored(Servo &s1, Servo &s2, int fromAngle, int toAngle, int stepDelay = 10) {
  if (fromAngle < toAngle) {
    for (int a = fromAngle; a <= toAngle; a++) {
      s1.write(a);
      s2.write(180 - a);
      delay(stepDelay);
      //printForExcel();
      printGripperAngle();
    }
  } else {
    for (int a = fromAngle; a >= toAngle; a--) {
      s1.write(a);
      s2.write(180 - a);
      delay(stepDelay);
      //printForExcel();
      printGripperAngle();
    }
  }
}

void moveToPreset(int s1, int s3, int s4, int s5, int s6) {
  moveServoMirrored(myServo, myServo2, currentS1, s1); currentS1 = s1;
  moveServoSmooth(myServo3, currentS3, s3); currentS3 = s3;
  moveServoSmooth(myServo4, currentS4, s4); currentS4 = s4;
  moveServoSmooth(myServo5, currentS5, s5); currentS5 = s5;
  moveServoSmooth(myServo6, currentS6, s6); currentS6 = s6;
}

void sequence() {
moveToPreset(HOME_S1, HOME_S3, HOME_S4, HOME_S5, HOME_S6);
moveToPreset(SCAN_S1, SCAN_S3, SCAN_S4, SCAN_S5, SCAN_S6);
myServo6.write(0);

moveToPreset(BLUE_S1, BLUE_S3, BLUE_S4, BLUE_S5, 0);
moveToPreset(BLUE_S1, BLUE_S3, BLUE_S4, BLUE_S5, 0);
moveToPreset(BLUE_S1 - 33, BLUE_S3, BLUE_S4, BLUE_S5, 0);
moveToPreset(BLUE_S1 - 33, BLUE_S3, BLUE_S4, BLUE_S5, 130);

moveToPreset(SCAN_S1, SCAN_S3, SCAN_S4, SCAN_S5, SCAN_S6);
myServo6.write(0);

moveToPreset(BLUE_S1, BLUE_S3 - 10, BLUE_S4, BLUE_S5, 0);
moveToPreset(BLUE_S1 - 33, BLUE_S3 - 10, BLUE_S4, BLUE_S5, 0);
moveToPreset(BLUE_S1 - 33, BLUE_S3 - 10, BLUE_S4, BLUE_S5, 130);

moveToPreset(SCAN_S1, SCAN_S3, SCAN_S4, SCAN_S5, SCAN_S6);
myServo6.write(0);

moveToPreset(BLUE_S1, BLUE_S3 + 10, BLUE_S4, BLUE_S5, 0);
moveToPreset(BLUE_S1 - 33, BLUE_S3 + 10, BLUE_S4, BLUE_S5, 0);
moveToPreset(BLUE_S1 - 33, BLUE_S3 + 10, BLUE_S4, BLUE_S5, 130);

moveToPreset(SCAN_S1, SCAN_S3, SCAN_S4, SCAN_S5, SCAN_S6);
 
  
}

void setup() {
  Serial.begin(9600);
  myServo.attach(9);
  myServo2.attach(10);
  myServo3.attach(5);
  myServo4.attach(3);
  myServo5.attach(6);
  myServo6.attach(11);
  moveToPreset(HOME_S1, HOME_S3, HOME_S4, HOME_S5, HOME_S6);
}

void loop() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    if (cmd == "go") {
      sequence();
    }
  }
}