void setup() {
    Serial.begin(9600);
    pinMode(A5, OUTPUT);
    digitalWrite(A5, HIGH);
}

void loop() {
    int value = analogRead(A2);

    Serial.println("Analog  Value: ");
    Serial.println(value);
    
}
