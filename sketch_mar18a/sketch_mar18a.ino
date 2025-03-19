void setup() {
  pinMode(A0, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(A0, LOW);
  digitalWrite(A3, LOW);
}

void loop() {
  digitalWrite(13, HIGH);
  digitalWrite(A3, LOW);
  delay(8000);
  digitalWrite(13, LOW);
  delay(8000);
}