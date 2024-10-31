/**********************************************************
 | Name:KEMAJOU NJAMEN DANIEL ISAAC                        |
 | Matricule:  FE14A104                                    |
 | purpose of the program: Send SMS through GSM module|
 | Date: JULY, 2018                                     |
 ***********************************************************/

#include <SoftwareSerial.h>
// Configure software serial port
SoftwareSerial Serial1(7, 8); 

void setup() {
  // Arduino communicates with SIM900 GSM shield at a baud rate of 19200
  // Make sure that corresponds to the baud rate of your module
  Serial.begin(9600);
  Serial1.begin(9600);
  // Give time to your GSM shield log on to network
  delay(5000);   
  sendSMS();

}
void loop() { 
 
}
  
void sendSMS(){
// AT command to set SIM900 to SMS mode
  Serial1.print("AT+CMGF=1\r\n"); 
  delay(1000);

  // REPLACE THE X's WITH THE RECIPIENT'S MOBILE NUMBER
  // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
  Serial1.println("AT + CMGS = \"+237672751838\""); 
  delay(1000);
  
  // REPLACE WITH YOUR OWN SMS MESSAGE CONTENT
  Serial1.print("HAHAHAHA..."); 
  delay(100);

//gsm900.write(0x0D);
Serial1.println();
Serial1.println((char)26); 
  // Give module time to send SMS
  delay(2000); 
}

