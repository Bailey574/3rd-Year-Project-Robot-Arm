#define S0 2
#define S1 4
#define S2 7
#define S3 8
#define OUT 12

int redFreq = 0;
int greenFreq = 0;
int blueFreq = 0;

void setup() {
  Serial.begin(9600);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);

  // 20% frequency scaling
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  Serial.println("TCS3200 RED/GREEN/BLUE Detection Ready");
}

void loop() {
  // ----- RED -----
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  delay(50);
  redFreq = pulseIn(OUT, LOW);

  // ----- GREEN -----
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  delay(50);
  greenFreq = pulseIn(OUT, LOW);

  // ----- BLUE -----
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  delay(50);
  blueFreq = pulseIn(OUT, LOW);

  // ---- Classification ----
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

  // ---- Print Results ----
  Serial.print("R: "); Serial.print(redFreq);
  Serial.print(" G: "); Serial.print(greenFreq);
  Serial.print(" B: "); Serial.print(blueFreq);
  Serial.print(" -> ");
  Serial.println(detectedColor);

  delay(300);
}
