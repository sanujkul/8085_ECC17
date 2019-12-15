void setup() {
  Serial.begin(9600);
  for(int i=0; i<7; i++){
    pinMode(i, INPUT);
  }
}

void loop() {
  for(int i=0; i<8; i++){
    Serial.print(digitalRead(i));
    Serial.print(",");
  }
  Serial.println();
}
