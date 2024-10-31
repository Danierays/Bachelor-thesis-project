/*************
 * Name:KEMAJOU NJAMEN DANIEL ISAAC                        |
 * Matricule:  FE14A104 
 * Purpose of program: To sending and receiving SMS using SIM900 GSM GPRS SHIELD and ARDUINO
 * 
 *****************/

#include <SoftwareSerial.h>

SoftwareSerial Serial1(7, 8);

void setup()
{
  Serial1.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  delay(100);
  /*
  The first task is to set baud rates of SoftwareSerial library to communicate with GSM module.
  We achieve this by invoking Serial1.begin function.

  Both should be set at the same baud rate and we use 9600 bits/second here in our tutorial.
  Configuration part is over with setting baud rates and its good to give a small delay of 100 milli seconds.
  */
}


void loop()
{
  if (Serial.available()>0)
  /*
  checks for any data coming through serial port of arduino.
  The function returns the number of bytes available to read from serial buffer.
  If there is no data available, it returns a -1
  */
   switch(Serial.read())
   /*
    Reads all the data available on serial buffer (or incoming serial data if put otherwise). 
    Returns the first byte of incoming serial data.
   */
  {
    case 's':
      SendMessage();
      break;
    case 'r':
      RecieveMessage();
      break;
      /*
      The program seeks user input via serial monitor of Arduino. 
      If the input is ‘s’ the program will invoke function to send an sms from GSM module.
      If the user input is ‘r’, the program will invoke the function to receive a live SMS from GSM module and display it on serial monitor of Arduino.
      */
  }

 if (Serial1.available()>0)
 /*
 checks for any data coming from GSM module through the SoftwareSerial pins 7 and 8.
 Returns the number of bytes available to read from software serial port. 
 Returns a -1 if no data is available to read.
 */
   Serial.write(Serial1.read());
   /*
    
   Serial1.read() Reads the incoming data through software serial port.
   */
}


 void SendMessage()

 /*
 SendMessage() – is the function we created in our arduino sketch to send an SMS. 
 To send an SMS, we should set our GSM module to Text mode first. This is achieved by sending an AT Command “AT+CMGF=1” 
 We send this command by writing this to SoftwareSerial port.
 To achieve this we use the Serial1.println() function.
 Serial1.println writes data to software serial port (the Tx pin of our Software Serial – that is pin 8) 
 this will be captured by GSM module (through its Rx pin).
 After setting the GSM module to Text mode, we should the the mobile number to which we shall send the SMS. 
 This is achieved with AT command “AT+CMGS=\”+237xxxxxxxxxx\”\r”
 */
{
  Serial1.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  Serial1.println("AT+CMGS=\"+237651256271\"\r"); // Replace x with mobile number
  delay(1000);
  Serial1.println("THIS IS THE REPETITION OF TELECOM LAB 3");// The SMS text you want to send
  delay(100);
   Serial1.println((char)26);
   /*
   In next step, we should send the actual content of SMS.
   The end of SMS content is identified with CTRL+Z symbol.
   The ASCII value of this CTRL+Z is 26. So we send a char(26) to GSM module using the line Serial1.println((char)26); 
   Each and every AT command may be followed by 1 second delay.
   We must give some time for GSM module to respond properly.
   Once these commands are send to GSM module, you shall receive an SMS in the set mobile number.
   */
   
  delay(1000);
}


 void RecieveMessage()
{
  Serial1.println("AT+CNMI=2,2,0,0,0"); // AT Command to receive a live SMS
  delay(1000);

  /*is the function to receive an SMS (a live SMS). 
   * The AT command to receive a live SMS is “AT+CNMI=2,2,0,0,0” – we just need to send this command to GSM module and apply a 1 second delay. 
   * Once you send this command, try sending an SMS to the SIM card number put inside GSM module. 
   * You will see the SMS you had sent displayed on your Arduino serial monitor.

There are different AT commands for different tasks.
If you want to read all SMS’s stored in your SIM card, send the following AT Command to gsm module – “AT+CMGL=\”ALL\”\r”*/
 }
 

