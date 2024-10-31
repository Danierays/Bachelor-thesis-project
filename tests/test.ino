/**********************************************************
 | Name:KEMAJOU NJAMEN DANIEL ISAAC                        |
 | Matricule:  FE14A104                                    |
 | Purpose of the program: Final year project testcase - 1 |
 | Date: JULY, 2018                                        |
 ***********************************************************/

#include <SoftwareSerial.h>

//Configure software serial port
SoftwareSerial Serial1(7,8);
int PIRinput=2;
int Buzzer=13;
String textMessage = "";
void emptyBuffer();
void sendSMS(String);

void setup() {
 Serial.begin(9600);
 Serial1.begin(9600);
 delay(2000);
 pinMode(PIRinput,INPUT);
 pinMode(Buzzer,OUTPUT);
 digitalWrite(Buzzer,LOW);
 Serial1.print("AT+CMGF=1\r"); 
  delay(1000);
 Serial1.print("AT+CNMI=2,2,0,0,0\r");
 delay(1000);
 Serial.println("emptying buffer");
 Serial1.print("AT+CMGD=1,4\r");  // Deletes all SMS saved in SIM memory
 Serial1.print("AT+CMGDA=\""); 
 Serial1.println("DEL ALL\"");
 emptyBuffer();
 Serial.println("emptied buffer");
}
void loop() {
  Serial.println("testing");
  Serial.println(digitalRead(PIRinput));
  if(digitalRead(PIRinput)==HIGH){
      Serial.println("Serial1.available is "+String(Serial1.available()));
      if(Serial1.available()>0){
        Serial.println("Something is available from the GSM module");
        textMessage=Serial1.readString();
        Serial.print(textMessage);
        delay(2000);
        if(textMessage.indexOf("ON")>=0){
          //sendSMS("ALARM IS NOW ON");
          digitalWrite(Buzzer,HIGH);
          Serial.println("ALARM ON");
          emptyBuffer();
          textMessage="";
        }
        else if(textMessage.indexOf("OFF")>=0){
          //sendSMS("ALARM IS NOW OFF");
          digitalWrite(Buzzer,LOW);
          Serial.println("ALARM OFF"); 
          emptyBuffer();
          textMessage="";
        }
      }
      else{
          Serial.println("motion detected");
          Serial.println("Sending detection message");
          sendSMS("MOTION DETECTED");
          Serial.println("Message Sent");
          delay(5000);
          //emptyBuffer();
      }
  }
 
  delay(1000);
}
     
void sendSMS(String sms){
// AT command to set SIM900 to SMS mode
  Serial1.print("AT+CMGF=1\r"); 
  delay(100);
  // REPLACE THE X's WITH THE RECIPIENT'S MOBILE NUMBER
  // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
  Serial1.println("AT + CMGS = \"+237682714771\""); 
  delay(100);
  
  // REPLACE WITH YOUR OWN SMS MESSAGE CONTENT
  Serial1.print(sms); 
  delay(100);

  Serial1.write(0x1A);
  Serial1.println();
  Serial1.println((char)26); 
  // Give module time to send SMS
  delay(4000); 
}

void emptyBuffer(){
char value;
 while(Serial1.available()>0){
  value = Serial1.read();
 delay(50);
}
value = 0;
}

