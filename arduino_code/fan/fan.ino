void setup() {
  // put your setup code here, to run once:
  pinMode(3,OUTPUT);

  Serial.begin(115200);
  digitalWrite(3,LOW);

  String status;
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(3,HIGH);
  delay(300000);
  digitalWrite(3,LOW);
  delay(50000);
}
