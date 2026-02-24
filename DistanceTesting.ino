// --- TCS3200 Colour Sensor Pins ---
#define S0 2
#define S1 4
#define S2 7
#define S3 8
#define OUT 12

// --- HC-SR04 Distance Sensor Pins ---
#define TRIG_PIN 13
#define ECHO_PIN A0  // analog pin used as digital input

int redFreq = 0;
int greenFreq = 0;
int blueFreq = 0;

long duration;
int distance;

void setup() {
  Serial.begin(9600);

  // --- Colour sensor setup ---
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);

  // Set 20% frequency scaling
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  // --- Distance sensor setup ---
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.println("LEGO Colour + Distance Detection Ready");
}

void loop() {
  // ------------------ Colour Sensor ------------------
  // RED
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  delay(50);
  redFreq = pulseIn(OUT, LOW);

  // GREEN
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  delay(50);
  greenFreq = pulseIn(OUT, LOW);

  // BLUE
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  delay(50);
  blueFreq = pulseIn(OUT, LOW);

  // Classification (RED, GREEN, BLUE only)
  String detectedColor = "Unknown";

  if (redFreq < greenFreq && redFreq < blueFreq) {
      detectedColor = "RED";
  }
  else if (greenFreq < redFreq && greenFreq < blueFreq) {
      detectedColor = "GREEN";
  }
  else if (blueFreq < redFreq && blueFreq < greenFreq) {
      detectedColor = "BLUE";
  }

  // ------------------ Distance Sensor ------------------
  // Trigger pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read echo pulse duration
  duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate distance in cm
  distance = duration * 0.034 / 2;

  // ------------------ Print Results ------------------
  Serial.print("R: "); Serial.print(redFreq);
  Serial.print(" G: "); Serial.print(greenFreq);
  Serial.print(" B: "); Serial.print(blueFreq);
  Serial.print(" -> Colour: "); Serial.print(detectedColor);
  Serial.print(" | Distance: "); Serial.print(distance);
  Serial.println(" cm");

  delay(300); // short pause before next reading
}
