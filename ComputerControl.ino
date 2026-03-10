#include <Servo.h>

Servo myServo, myServo2, myServo3, myServo4, myServo5, myServo6;

int redDistancePrevious = 0;
bool redFirstBrick = true;

int greenDistancePrevious = 0;
bool greenFirstBrick = true;

int blueDistancePrevious = 0;
bool blueFirstBrick = true;

const int HOME_S1 = 120;
const int HOME_S3 = 45;
const int HOME_S4 = 90;
const int HOME_S5 = 34;
const int HOME_S6 = 76;

const int RED_S1 = 120;
const int RED_S3 = 45;
const int RED_S4 = 90;
const int RED_S5 = 34;
const int RED_S6 = 0;

const int GREEN_S1 = 120;
const int GREEN_S3 = 23;
const int GREEN_S4 = 90;
const int GREEN_S5 = 34;
const int GREEN_S6 = 0;

const int BLUE_S1 = 120;
const int BLUE_S3 = 67;
const int BLUE_S4 = 90;
const int BLUE_S5 = 34;
const int BLUE_S6 = 0;

const int SCAN_S1 = 120;
const int SCAN_S3 = 110;
const int SCAN_S4 = 100;
const int SCAN_S5 = 20;
const int SCAN_S6 = 34;

int currentS1 = HOME_S1;
int currentS3 = HOME_S3;
int currentS4 = HOME_S4;
int currentS5 = HOME_S5;
int currentS6 = HOME_S6;

#define S0 2
#define S1 4
#define S2 7
#define S3 8
#define OUT 12

#define TRIG_PIN 13
#define ECHO_PIN A0

enum RobotState { SERVO_CONTROL, SCAN_MODE };
RobotState state = SERVO_CONTROL;

void moveServoSmooth(Servo &servo, int fromAngle, int toAngle, int stepDelay = 10) {
  if (fromAngle < toAngle) {
    for (int a = fromAngle; a <= toAngle; a++) { servo.write(a); delay(stepDelay); }
  } else {
    for (int a = fromAngle; a >= toAngle; a--) { servo.write(a); delay(stepDelay); }
  }
}

void moveServoMirrored(Servo &s1, Servo &s2, int fromAngle, int toAngle, int stepDelay = 10) {
  if (fromAngle < toAngle) {
    for (int a = fromAngle; a <= toAngle; a++) { s1.write(a); s2.write(180 - a); delay(stepDelay); }
  } else {
    for (int a = fromAngle; a >= toAngle; a--) { s1.write(a); s2.write(180 - a); delay(stepDelay); }
  }
}

void moveToPreset(int s1, int s3, int s4, int s5, int s6) {
  moveServoMirrored(myServo, myServo2, currentS1, s1); currentS1 = s1;
  moveServoSmooth(myServo3, currentS3, s3); currentS3 = s3;
  moveServoSmooth(myServo4, currentS4, s4); currentS4 = s4;
  moveServoSmooth(myServo5, currentS5, s5); currentS5 = s5;
  moveServoSmooth(myServo6, currentS6, s6); currentS6 = s6;
}

void setup() {
  Serial.begin(9600);
  myServo.attach(9); myServo2.attach(10); myServo3.attach(5); myServo4.attach(3); myServo5.attach(6); myServo6.attach(11);
  moveToPreset(HOME_S1, HOME_S3, HOME_S4, HOME_S5, HOME_S6);
  pinMode(S0, OUTPUT); pinMode(S1, OUTPUT); pinMode(S2, OUTPUT); pinMode(S3, OUTPUT); pinMode(OUT, INPUT);
  digitalWrite(S0, HIGH); digitalWrite(S1, LOW);
  pinMode(TRIG_PIN, OUTPUT); pinMode(ECHO_PIN, INPUT);
  Serial.println("Robot Ready.");
  Serial.println("Commands: servo / scanmode / home / red / green / blue / scan / servoX angle");
}

String readColor() {
  int readings = 6;
  int redCount = 0, greenCount = 0, blueCount = 0;
  for (int i = 0; i < readings; i++) {
    digitalWrite(S2, LOW); digitalWrite(S3, LOW); delay(50); int redFreq = pulseIn(OUT, LOW);
    digitalWrite(S2, HIGH); digitalWrite(S3, HIGH); delay(50); int greenFreq = pulseIn(OUT, LOW);
    digitalWrite(S2, LOW); digitalWrite(S3, HIGH); delay(50); int blueFreq = pulseIn(OUT, LOW);
    if (redFreq < greenFreq && redFreq < blueFreq) redCount++;
    else if (greenFreq < redFreq && greenFreq < blueFreq) greenCount++;
    else if (blueFreq < redFreq && blueFreq < greenFreq) blueCount++;
  }
  if (redCount > 1) return "RED";
  else if (greenCount > 1) return "GREEN";
  else if (blueCount > 1) return "BLUE";
  else return "Unknown";
}

void loop() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n'); cmd.trim();

    if (cmd == "scanmode") { state = SCAN_MODE; Serial.println("Switched to SCAN mode"); return; }
    if (cmd == "servo") { state = SERVO_CONTROL; Serial.println("Switched to SERVO CONTROL mode"); return; }

    if (state == SERVO_CONTROL) {
      if (cmd == "home") { moveToPreset(HOME_S1, HOME_S3, HOME_S4, HOME_S5, HOME_S6); Serial.println("Moved to HOME"); return; }
      if (cmd == "red") { 
        myServo6.write(0);
        currentS6 = 0;
        moveToPreset(RED_S1, RED_S3, RED_S4, RED_S5, 0); 
        moveToPreset(RED_S1-33, RED_S3, RED_S4, RED_S5, 0); 
        moveToPreset(RED_S1-33, RED_S3, RED_S4, RED_S5, 130); 
        Serial.println("Moved to RED"); return; }
      if (cmd == "green") {
        myServo6.write(0);
        currentS6 = 0;

        moveToPreset(GREEN_S1, GREEN_S3, GREEN_S4, GREEN_S5, 0); 
        moveToPreset(GREEN_S1-33, GREEN_S3, GREEN_S4, GREEN_S5, 0);
        moveToPreset(GREEN_S1-33, GREEN_S3, GREEN_S4, GREEN_S5, 130);
        Serial.println("Moved to GREEN"); return; }
      if (cmd == "blue") { 
        myServo6.write(0);
        currentS6 = 0;
        moveToPreset(BLUE_S1, BLUE_S3, BLUE_S4, BLUE_S5,0); 
        moveToPreset(BLUE_S1-33, BLUE_S3, BLUE_S4, BLUE_S5,0); 
        moveToPreset(BLUE_S1-33, BLUE_S3, BLUE_S4, BLUE_S5,130); 
        
        Serial.println("Moved to BLUE"); return; }
      if (cmd == "scan") { moveToPreset(SCAN_S1, SCAN_S3, SCAN_S4, SCAN_S5, SCAN_S6); Serial.println("Moved to SCAN"); return; }

      int spaceIndex = cmd.indexOf(' ');
      if (spaceIndex != -1) {
        String servoName = cmd.substring(0, spaceIndex);
        int targetAngle = cmd.substring(spaceIndex + 1).toInt();
        targetAngle = constrain(targetAngle, 0, 180);

        if (servoName == "servo1") { moveServoMirrored(myServo, myServo2, currentS1, targetAngle); currentS1 = targetAngle; }
        else if (servoName == "servo3") { moveServoSmooth(myServo3, currentS3, targetAngle); currentS3 = targetAngle; }
        else if (servoName == "servo4") { moveServoSmooth(myServo4, currentS4, targetAngle); currentS4 = targetAngle; }
        else if (servoName == "servo5") { moveServoSmooth(myServo5, currentS5, targetAngle); currentS5 = targetAngle; }
        else if (servoName == "servo6") { moveServoSmooth(myServo6, currentS6, targetAngle); currentS6 = targetAngle; }

        Serial.print(servoName); Serial.print(" set to "); Serial.println(targetAngle);
      }
    }
  }


  if (state == SCAN_MODE) {
    
    moveToPreset(SCAN_S1, SCAN_S3, SCAN_S4, SCAN_S5, SCAN_S6);
    myServo6.write(45);
    String color = readColor();

    digitalWrite(TRIG_PIN, LOW); delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH); delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH);
    int distance = duration * 0.034 / 2;

    Serial.print("Detected: "); Serial.print(color);
    Serial.print(" | Distance: "); Serial.print(distance); Serial.println(" cm");

   if (color == "RED") {

  myServo6.write(0);
  currentS6 = 0;

  moveToPreset(RED_S1, RED_S3, RED_S4, RED_S5, 0);

  if (redFirstBrick) {

    moveToPreset(RED_S1 - 33, RED_S3, RED_S4, RED_S5, 0);
    moveToPreset(RED_S1 - 33, RED_S3, RED_S4, RED_S5, 130);

    redDistancePrevious = distance;
    redFirstBrick = false;

  }
  else {

    if (distance > redDistancePrevious) {

      moveToPreset(RED_S1, RED_S3 - 10, RED_S4, RED_S5, 0);
      moveToPreset(RED_S1 - 33, RED_S3 - 10, RED_S4, RED_S5, 0);
      moveToPreset(RED_S1 - 33, RED_S3 - 10, RED_S4, RED_S5, 130);

    }
    else {

      moveToPreset(RED_S1, RED_S3 + 10, RED_S4, RED_S5, 0);
      moveToPreset(RED_S1 - 33, RED_S3 + 10, RED_S4, RED_S5, 0);
      moveToPreset(RED_S1 - 33, RED_S3 + 10, RED_S4, RED_S5, 130);

    }

   

    redDistancePrevious = distance;
  }

  state = SERVO_CONTROL;
}

    else if (color == "GREEN") {

  myServo6.write(0);
  currentS6 = 0;

  moveToPreset(GREEN_S1, GREEN_S3, GREEN_S4, GREEN_S5, 0);

  if (greenFirstBrick) {

    moveToPreset(GREEN_S1 - 33, GREEN_S3, GREEN_S4, GREEN_S5, 0);
    moveToPreset(GREEN_S1 - 33, GREEN_S3, GREEN_S4, GREEN_S5, 130);

    greenDistancePrevious = distance;
    greenFirstBrick = false;

  }
  else {

    if (distance > greenDistancePrevious) {

      moveToPreset(GREEN_S1, GREEN_S3 - 10, GREEN_S4, GREEN_S5, 0);
      moveToPreset(GREEN_S1 - 33, GREEN_S3 - 10, GREEN_S4, GREEN_S5, 0);
      moveToPreset(GREEN_S1 - 33, GREEN_S3 - 10, GREEN_S4, GREEN_S5, 130);

    }
    else {

      moveToPreset(GREEN_S1, GREEN_S3 + 10, GREEN_S4, GREEN_S5, 0);
      moveToPreset(GREEN_S1 - 33, GREEN_S3 + 10, GREEN_S4, GREEN_S5, 0);
      moveToPreset(GREEN_S1 - 33, GREEN_S3 + 10, GREEN_S4, GREEN_S5, 130);

    }

    greenDistancePrevious = distance;
  }

  state = SERVO_CONTROL;
}

    else if (color == "BLUE") {

  myServo6.write(0);
  currentS6 = 0;

  moveToPreset(BLUE_S1, BLUE_S3, BLUE_S4, BLUE_S5, 0);

  if (blueFirstBrick) {

    moveToPreset(BLUE_S1 - 33, BLUE_S3, BLUE_S4, BLUE_S5, 0);
    moveToPreset(BLUE_S1 - 33, BLUE_S3, BLUE_S4, BLUE_S5, 130);

    blueDistancePrevious = distance;
    blueFirstBrick = false;

  }
  else {

    if (distance > blueDistancePrevious) {

      moveToPreset(BLUE_S1, BLUE_S3 - 10, BLUE_S4, BLUE_S5, 0);
      moveToPreset(BLUE_S1 - 33, BLUE_S3 - 10, BLUE_S4, BLUE_S5, 0);
      moveToPreset(BLUE_S1 - 33, BLUE_S3 - 10, BLUE_S4, BLUE_S5, 130);

    }
    else {

      moveToPreset(BLUE_S1, BLUE_S3 + 10, BLUE_S4, BLUE_S5, 0);
      moveToPreset(BLUE_S1 - 33, BLUE_S3 + 10, BLUE_S4, BLUE_S5, 0);
      moveToPreset(BLUE_S1 - 33, BLUE_S3 + 10, BLUE_S4, BLUE_S5, 130);

    }

    blueDistancePrevious = distance;
  }

  state = SERVO_CONTROL;
}

    delay(500);
  }
}
