//Main code for project
#include <Servo.h>
//Each servo created
Servo myServo, myServo2, myServo3, myServo4, myServo5, myServo6;
//Stores previous distances from previous code devolpment
int redDistancePrevious = 0;
bool redFirstBrick = true;

int greenDistancePrevious = 0;
bool greenFirstBrick = true;

int blueDistancePrevious = 0;
bool blueFirstBrick = true;
//Each postion angle for each servos
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
//Intial postion on startup
int currentS1 = HOME_S1;
int currentS3 = HOME_S3;
int currentS4 = HOME_S4;
int currentS5 = HOME_S5;
int currentS6 = HOME_S6;
//Attaches pins for sensors
#define S0 2
#define S1 4
#define S2 7
#define S3 8
#define OUT 12

#define TRIG_PIN 13
#define ECHO_PIN A0
//Creates state manchine
enum RobotState { SERVO_CONTROL, SCAN_MODE };
RobotState state = SERVO_CONTROL;
//Functions to increment servo movement given servo current and desired angle and delay between servo increments
void moveServoSmooth(Servo &servo, int fromAngle, int toAngle, int stepDelay = 10) {
  if (fromAngle < toAngle) {
    for (int a = fromAngle; a <= toAngle; a++) { servo.write(a); delay(stepDelay); }
  } else {
    for (int a = fromAngle; a >= toAngle; a--) { servo.write(a); delay(stepDelay); }
  }
}
//Functions to increment servo movement given servo current and desired angle and delay between servo increments, same as above but angle is flipped and sent to opposite shoulder servo
void moveServoMirrored(Servo &s1, Servo &s2, int fromAngle, int toAngle, int stepDelay = 10) {
  if (fromAngle < toAngle) {
    for (int a = fromAngle; a <= toAngle; a++) { s1.write(a); s2.write(180 - a); delay(stepDelay); }
  } else {
    for (int a = fromAngle; a >= toAngle; a--) { s1.write(a); s2.write(180 - a); delay(stepDelay); }
  }
}
//Function which uses moveServo increments each servo one at a time given desired angles. Saves the current to be used again
void moveToPreset(int s1, int s3, int s4, int s5, int s6) {
  moveServoMirrored(myServo, myServo2, currentS1, s1); currentS1 = s1;
  moveServoSmooth(myServo3, currentS3, s3); currentS3 = s3;
  moveServoSmooth(myServo4, currentS4, s4); currentS4 = s4;
  moveServoSmooth(myServo5, currentS5, s5); currentS5 = s5;
  moveServoSmooth(myServo6, currentS6, s6); currentS6 = s6;
}
//On startup servo attached, serial monitor starts, pin startup for sensors and prints serial monitor messages
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
//Function which reads colour and returns average colour value based on smallest pulse width
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
//Main loop which reads serial monitor switches to different states based on commands typed
void loop() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n'); cmd.trim();

    if (cmd == "scanmode") { state = SCAN_MODE; Serial.println("Switched to SCAN mode"); return; }
    if (cmd == "servo") { state = SERVO_CONTROL; Serial.println("Switched to SERVO CONTROL mode"); return; }
    //State servo control desired postion or servo angles can be typed used for testing
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
      //Reads serial command when looking for angle e.g. servo1 76 gets targetAngle = 76 servoName = servo1
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
        //Lets user no where servo has moved
        Serial.print(servoName); Serial.print(" set to "); Serial.println(targetAngle);
      }
    }
  }

//Scan mode state
if (state == SCAN_MODE) {
    
  //Returns the colour detected and starts timer to measuring sensing time
  unsigned long start = millis();

  String color = readColor();
  unsigned long end = millis();
  Serial.print("Colour Classification time (ms): ");
  Serial.println(end - start);

  int distance;

  unsigned long start2 = millis();
//Gets average value from distance sensor
for (int i = 0; i < 4; i++) {
  digitalWrite(TRIG_PIN, LOW); 
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH); 
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;

  delay(30); 
}
  //Measured timing again for distance sensor
  unsigned long end2 = millis();
  Serial.print("Distance Classification time (ms): ");
  Serial.println(end2 - start2);
  //Prints detected colour and size for user
  Serial.print("Detected: "); Serial.print(color);
  Serial.print(" | Distance: "); Serial.print(distance); Serial.println(" cm");
  //Moves the arm to scan position
  unsigned long start3 = millis();
  moveToPreset(SCAN_S1, SCAN_S3, SCAN_S4, SCAN_S5, SCAN_S6);
  myServo6.write(45);
  delay(300);
  //Depending on colour moves to drop location and based on distance different threshold determine small medium or large sized bricks. Repeated logic for green and blue
  if (color == "RED") {

    myServo6.write(0);
    currentS6 = 0;

    moveToPreset(RED_S1, RED_S3, RED_S4, RED_S5, 0);

    if (distance <= 6) {

      moveToPreset(RED_S1, RED_S3 - 10, RED_S4, RED_S5, 0);
      moveToPreset(RED_S1 - 33, RED_S3 - 10, RED_S4, RED_S5, 0);
      moveToPreset(RED_S1 - 33, RED_S3 - 10, RED_S4, RED_S5, 130);
      

    }
    else if (distance <= 7) {

      moveToPreset(RED_S1 - 33, RED_S3, RED_S4, RED_S5, 0);
      moveToPreset(RED_S1 - 33, RED_S3, RED_S4, RED_S5, 130);
      

    }
    else {
      moveToPreset(RED_S1, RED_S3 + 10, RED_S4, RED_S5, 0);
      moveToPreset(RED_S1 - 33, RED_S3 + 10, RED_S4, RED_S5, 0);
      moveToPreset(RED_S1 - 33, RED_S3 + 10, RED_S4, RED_S5, 130);
      
    }
  }

  else if (color == "GREEN") {
    myServo6.write(0);
    currentS6 = 0;

    moveToPreset(GREEN_S1, GREEN_S3, GREEN_S4, GREEN_S5, 0);

    if (distance <= 6) {
        moveToPreset(GREEN_S1, GREEN_S3 - 10, GREEN_S4, GREEN_S5, 0);
        moveToPreset(GREEN_S1 - 33, GREEN_S3 - 10, GREEN_S4, GREEN_S5, 0);
        moveToPreset(GREEN_S1 - 33, GREEN_S3 - 10, GREEN_S4, GREEN_S5, 130);
    }
    else if (distance <= 7) {
        moveToPreset(GREEN_S1 - 33, GREEN_S3, GREEN_S4, GREEN_S5, 0);
        moveToPreset(GREEN_S1 - 33, GREEN_S3, GREEN_S4, GREEN_S5, 130);
    }
    else {
        moveToPreset(GREEN_S1, GREEN_S3 + 10, GREEN_S4, GREEN_S5, 0);
        moveToPreset(GREEN_S1 - 33, GREEN_S3 + 10, GREEN_S4, GREEN_S5, 0);
        moveToPreset(GREEN_S1 - 33, GREEN_S3 + 10, GREEN_S4, GREEN_S5, 130);
    }
}





  else if (color == "BLUE") {
    myServo6.write(0);
    currentS6 = 0;

    moveToPreset(BLUE_S1, BLUE_S3, BLUE_S4, BLUE_S5, 0);

    if (distance <= 6) {
        moveToPreset(BLUE_S1, BLUE_S3 - 10, BLUE_S4, BLUE_S5, 0);
        moveToPreset(BLUE_S1 - 33, BLUE_S3 - 10, BLUE_S4, BLUE_S5, 0);
        moveToPreset(BLUE_S1 - 33, BLUE_S3 - 10, BLUE_S4, BLUE_S5, 130);
    }
    else if (distance <= 7) {
        moveToPreset(BLUE_S1 - 33, BLUE_S3, BLUE_S4, BLUE_S5, 0);
        moveToPreset(BLUE_S1 - 33, BLUE_S3, BLUE_S4, BLUE_S5, 130);
    }
    else {
        moveToPreset(BLUE_S1, BLUE_S3 + 10, BLUE_S4, BLUE_S5, 0);
        moveToPreset(BLUE_S1 - 33, BLUE_S3 + 10, BLUE_S4, BLUE_S5, 0);
        moveToPreset(BLUE_S1 - 33, BLUE_S3 + 10, BLUE_S4, BLUE_S5, 130);
    }
}
//Timers created to measure actuation delay and total time for sensing and sorting
  unsigned long end3 = millis();
  Serial.print("Actuation time (ms): ");
  Serial.println(end3 - start3);

  
  Serial.print("Total time (ms): ");
  Serial.println(end3 - start);

  state = SERVO_CONTROL;
  delay(500);
}
}
