#include <Servo.h>

Servo myServo, myServo2, myServo3, myServo4, myServo5, myServo6;


const int HOME_S1 = 65;
const int HOME_S3 = 45;
const int HOME_S4 = 90;
const int HOME_S5 = 34;
const int HOME_S6 = 76;


int currentS1 = HOME_S1;
int currentS3 = HOME_S3;
int currentS4 = HOME_S4;
int currentS5 = HOME_S5;
int currentS6 = HOME_S6;


void moveServoSmooth(Servo &servo, int fromAngle, int toAngle, int stepDelay = 10) {
  if (fromAngle < toAngle) {
    for (int a = fromAngle; a <= toAngle; a++) {
      servo.write(a);
      delay(stepDelay);
    }
  } else {
    for (int a = fromAngle; a >= toAngle; a--) {
      servo.write(a);
      delay(stepDelay);
    }
  }
}


void moveServoMirrored(Servo &s1, Servo &s2, int fromAngle, int toAngle, int stepDelay = 10) {
  if (fromAngle < toAngle) {
    for (int a = fromAngle; a <= toAngle; a++) {
      s1.write(a);
      s2.write(180 - a);
      delay(stepDelay);
    }
  } else {
    for (int a = fromAngle; a >= toAngle; a--) {
      s1.write(a);
      s2.write(180 - a);
      delay(stepDelay);
    }
  }
}


void goHome() {
  moveServoMirrored(myServo, myServo2, currentS1, HOME_S1);
  currentS1 = HOME_S1;

  moveServoSmooth(myServo3, currentS3, HOME_S3);
  currentS3 = HOME_S3;

  moveServoSmooth(myServo4, currentS4, HOME_S4);
  currentS4 = HOME_S4;

  moveServoSmooth(myServo5, currentS5, HOME_S5);
  currentS5 = HOME_S5;

  moveServoSmooth(myServo6, currentS6, HOME_S6);
  currentS6 = HOME_S6;
}

void setup() {
  Serial.begin(9600);

  myServo.attach(9);
  myServo2.attach(10);
  myServo3.attach(5);
  myServo4.attach(3);
  myServo5.attach(6);
  myServo6.attach(11);

  goHome();   

  Serial.println("Ready. Use 'servoX angle' or 'home'");
}

void loop() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    
    if (cmd == "home") {
      goHome();
      Serial.println("Moved to HOME position");
      return;
    }

    int spaceIndex = cmd.indexOf(' ');
    if (spaceIndex == -1) return;

    String servoName = cmd.substring(0, spaceIndex);
    int targetAngle = cmd.substring(spaceIndex + 1).toInt();
    targetAngle = constrain(targetAngle, 0, 180);

    if (servoName == "servo1") {
      moveServoMirrored(myServo, myServo2, currentS1, targetAngle);
      currentS1 = targetAngle;
    }
    else if (servoName == "servo3") {
      moveServoSmooth(myServo3, currentS3, targetAngle);
      currentS3 = targetAngle;
    }
    else if (servoName == "servo4") {
      moveServoSmooth(myServo4, currentS4, targetAngle);
      currentS4 = targetAngle;
    }
    else if (servoName == "servo5") {
      moveServoSmooth(myServo5, currentS5, targetAngle);
      currentS5 = targetAngle;
    }
    else if (servoName == "servo6") {
      moveServoSmooth(myServo6, currentS6, targetAngle);
      currentS6 = targetAngle;
    }

    Serial.print(servoName);
    Serial.print(" set to ");
    Serial.println(targetAngle);
  }
}
