int PIR=2;
int BUZZER=13;
int motion=0;
void setup() {
  Serial.begin(9600);
  pinMode(PIR,INPUT);
  pinMode(BUZZER,OUTPUT);
  digitalWrite(BUZZER,LOW);

}

void loop() {
  motion=digitalRead(PIR);
  Serial.println(motion);
  if(motion==HIGH){
    digitalWrite(BUZZER,HIGH);
  }
  else{
    digitalWrite(BUZZER,LOW);
  }
  delay(200);
}
