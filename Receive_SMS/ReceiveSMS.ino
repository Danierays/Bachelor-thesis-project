/**********************************************************
 | Name:KEMAJOU NJAMEN DANIEL ISAAC                        |
 | Matricule:  FE14A104                                    |
 | purpose of the program: receiving SMS through GSM module|
 | Date: JULY, 2018                                     |
 ***********************************************************/

#include <SoftwareSerial.h>

//Configure software serial port
SoftwareSerial Serial1 (7, 8);
//variable to save incoming SMS characters
char incoming_char = 0;

void setup() {
  // Arduino communicates with SIM900 GSM shield at a baud rate of 9600
  //Make sure that corresponds to the baud rate of your module
 Serial1.begin(9600);
  //For serial monitor
  Serial.begin(9600);
  //Give time to your GSM shield log on to network
  delay(100);
  //AT command to set SIM900 to SMS mode
  Serial1.println("AT+CMGF=35"); 
  delay(100);
  //Set module to send SMS data to serial out upon receipt
  Serial1.println("AT+CNMI=2,2,0,0,0");
  delay(100);
}
void loop() {
  // Display any text that the GSM shield sends out on the serial monitor
  if(Serial1.available() > 0){
    //Get the character from the cellular serial port
    incoming_char = Serial1.read();
    //Print the incoming character to the terminal
    Serial.println(incoming_char);
  }

}
