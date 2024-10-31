/**********************************************************
 | Name:KEMAJOU NJAMEN DANIEL ISAAC                        |
 | Matricule:  FE14A104                                    |
 | Purpose of the program: Final year project testcase -2  |
 | Date: JULY, 2018                                        |
 ***********************************************************/

#include <SoftwareSerial.h>

//Configure software serial port
SoftwareSerial Serial1(7,8);
int PIR=2;
int Buzzer=13;
String textMessage="";
void emptyBuffer();
void sendSMS(String);
int motion=0;

void setup() {
 Serial.begin(9600);
 Serial1.begin(9600);
 delay(2000);
 pinMode(PIR,INPUT);
 pinMode(Buzzer,OUTPUT);
 digitalWrite(Buzzer,LOW);
 Serial1.print("AT+CMGF=1\r"); 
  delay(1000);
 Serial1.print("AT+CNMI=2,2,0,0,0\r");
 delay(1000);
 Serial.println("emptying buffer");
 emptyBuffer();
 Serial.println("emptied buffer");
}
void loop() {
  Serial.println("testing");
  Serial.println(digitalRead(PIR));
  motion=digitalRead(PIR);

  if(motion==HIGH){
     Serial.println("motion detected");
     Serial.println("Sending detection message");
     sendSMS("MOTION DETECTED");
     Serial.println("Message Sent");
     emptyBuffer();
     digitalWrite(Buzzer,HIGH);
     motion==LOW;
  }
  
  if(Serial1.available()>0){
   Serial.println("Something is available from the GSM module");
   textMessage=Serial1.readString();
   Serial.print(textMessage);
   delay(2000);
   if(textMessage.indexOf("OFF")>=0){
   sendSMS("ALARM IS NOW OFF");
   digitalWrite(Buzzer,LOW);
   Serial.println("ALARM OFF");       
   emptyBuffer();
   textMessage="";
    }
   //if(textMessage.indexOf("ON")>=0){
   //sendSMS("ALARM IS NOW ON");
   //digitalWrite(Buzzer,HIGH);
  // Serial.println("ALARM ON");      
  // emptyBuffer();
   //textMessage="";
   // }
   
 }
             
      
  }
  
     
void sendSMS(String sms){
// AT command to set SIM900 to SMS mode
  Serial1.print("AT+CMGF=1\r"); 
  delay(200);

  // REPLACE THE X's WITH THE RECIPIENT'S MOBILE NUMBER
  // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
  Serial1.println("AT + CMGS = \"+237672751838\""); 
  delay(200);
  
  // REPLACE WITH YOUR OWN SMS MESSAGE CONTENT
  Serial1.print(sms); 
  delay(200);

  Serial1.write(0x0D);
  Serial1.println();
  Serial1.println((char)26); 
  // Give module time to send SMS
  delay(200); 
}

void emptyBuffer(){
 char value;
 while(Serial1.available()>0){
  value = Serial1.read();
  delay(50);
 }
 value = 0;
}

