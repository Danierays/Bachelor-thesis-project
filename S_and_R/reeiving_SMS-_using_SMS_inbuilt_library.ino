#include <GSM.h> 
// PIN Number for the SIM 
#define PINNUMBER "" //sim pin incase the sim is kept locked 
// initialize the library instances 
GSM gsmAccess; 
GSM_SMS sms; 
// Array to hold the number a SMS is retreived from 
char senderNumber[20]; 
void setup() { 
  // initialize serial communications and wait for port to open: 
  Serial.begin(9600); 
  while (!Serial) { 
    ; // wait for serial port to connect. Needed for native USB port only } 
  Serial.println("SMS Messages Receiver"); 
  // connection state 
  boolean notConnected = true; 
  // Start GSM connection 
  while (notConnected) { 
    if (gsmAccess.begin(PINNUMBER) == GSM_READY) { 
      notConnected = false; 
      } 
      else { 
        Serial.println("Not connected"); 
        delay(1000); 
        } 
        } 
      Serial.println("GSM initialized"); 
      Serial.println("Waiting for messages"); 
  }
}     

 void loop(){ 
   char c; 
    // If there are any SMSs available() 
     if (sms.available()) 
        { 
          Serial.println("Message received from:"); 
          // Get remote number 
          sms.remoteNumber(senderNumber, 20); 
          Serial.println(senderNumber); 
          // An example of message disposal 
          // Any messages starting with # should be discarded 
          if (sms.peek() == '#') 
          { 
            Serial.println("Discarded SMS"); 
            sms.flush(); 
            } 
            // Read message bytes and print them 
            while (c = sms.read()) 
            { 
              Serial.print(c); 
              } 
              Serial.println("\nEND OF MESSAGE"); 
              // Delete message from modem memory 
              sms.flush(); 
              Serial.println("MESSAGE DELETED"); 
              } 
              delay(1000); 
              }
        

