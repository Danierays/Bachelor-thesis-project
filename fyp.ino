/**********************************************************
 | Name:KEMAJOU NJAMEN DANIEL ISAAC                        |
 | Matricule:  FE14A104                                    |
 | Description: Final year project code         |
 | Date: JULY, 2018                                        |
 ***********************************************************/
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>

const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'},
};

byte rowPins[ROWS] = {12, 11, 10, 9};
byte colPins[COLS] = {6, 5, 4, 3};

Keypad keypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); //   

struct Access{
  char pin[17];
  char num1[10];
  char num2[10];
  char num3[10];
};

Access access;

//Configure software serial port
SoftwareSerial Serial1(7,8);
int PIRinput=2;
int Buzzer=13;
String textMessage = "";
void sendSMS(String, String);
void emptyBuffer();
String phoneNumber1, phoneNumber2, phoneNumber3 = "";

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  lcd.begin(16, 2);
  delay(2000);
  pinMode(PIRinput,INPUT);
  pinMode(Buzzer,OUTPUT);
  digitalWrite(Buzzer,LOW);
  Serial1.print("AT+CMGF=1\r"); 
  delay(200);
  Serial1.print("AT+CMGD=1,4\r");  // Deletes all SMS saved in SIM memory
  Serial1.print("AT+CMGDA=\"");
  Serial1.println("DEL ALL\"");
  Serial1.print("AT+CNMI=2,2,0,0,0\r");
  delay(1000);
//  strcpy(access.num1,"");
//  strcpy(access.num3,"");
//  strcpy(access.num2,"");
//  writeEEPROM();
  Serial.println("emptying buffer");
  emptyBuffer();
  Serial.println("emptied buffer");

for (int i=0 ; i < EEPROM.length(); i++)
{EEPROM.write(i,0);
}

  readEEPROM();
  Serial.print("Pin: ");
  Serial.println(access.pin);
  Serial.print("Number 1: ");
  Serial.println(access.num1);
  Serial.print("Number 2: ");
  Serial.println(access.num2);
  Serial.print("Number 3: ");
  Serial.print(access.num3);
  lcd.print("  HOME SECURITY ");
  delay(1500);
  switch(makeFirstChoice()){
    case '1':
      editPhoneNumber();
      break;
    case '2':
      editPin();
      break;
    default:
      break;
  }
}
void loop() {
  lcd.clear();
  delay(1000);
  lcd.print("  Tracking   ");
  Serial.println("tracking");
  Serial.println(digitalRead(PIRinput));
  while(digitalRead(PIRinput)==HIGH){
    delay(10000);
    Serial.println("motion detected");
    Serial.println("Sending detection message");

    // Send message to the numbers depending on if it is registered
    if (phoneNumber1.length()) sendSMS("MOTION DETECTED", String(phoneNumber1));
    if (phoneNumber2.length()) sendSMS("MOTION DETECTED", String(phoneNumber2));
    if (phoneNumber2.length()) sendSMS("MOTION DETECTED", String(phoneNumber2));
    Serial.println("Message Sent");
    delay(5000);
    emptyBuffer();
    break;
 }
  //emptyBuffer();
  //digitalRead(PIRinput)==LOW;
  if(Serial1.available()>0){
    Serial.println("Something is available from the GSM module");
    textMessage=Serial1.readString();
    Serial.print(textMessage);
    delay(10);
    if(textMessage.indexOf("ON")>=0){
      if (phoneNumber1.length()) sendSMS("ALARM IS NOW ON", String(phoneNumber1));
      if (phoneNumber2.length()) sendSMS("ALARM IS NOW ON", String(phoneNumber2));
      if (phoneNumber2.length()) sendSMS("ALARM IS NOW ON", String(phoneNumber2));
 
      digitalWrite(Buzzer,HIGH);
      Serial.println("ALARM ON");
      emptyBuffer();
      textMessage="";
    }
    else if(textMessage.indexOf("OFF")>=0){
      if (phoneNumber1.length()) sendSMS("ALARM IS NOW OFF", String(phoneNumber1));
      if (phoneNumber2.length()) sendSMS("ALARM IS NOW OFF", String(phoneNumber2));
      if (phoneNumber2.length()) sendSMS("ALARM IS NOW OFF", String(phoneNumber2));
      
      digitalWrite(Buzzer,LOW);
      Serial.println("ALARM OFF");
      emptyBuffer();
      textMessage="";
    }
  } 
  delay(200);
}

void sendSMS(String sms, String phoneNumber){
  // AT command to set SIM900 to SMS mode
  Serial1.print("AT+CMGF=1\r");
  delay(200);

  // REPLACE THE X's WITH THE RECIPIENT'S MOBILE NUMBER
  // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
  Serial1.print("AT + CMGS = \"");
  Serial1.print("+237"+phoneNumber);
  Serial1.println("\""); 
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

void readEEPROM(){
  EEPROM.get(0, access);
  phoneNumber1 = access.num1;
  phoneNumber2 = access.num2;
  phoneNumber3 = access.num3;
}

void writeEEPROM(){
  EEPROM.put(0, access);
}

void editPin(){
  String msg = ">>>Enter Pin<<<";
  int msg_pos[2] = {0, 0};
  int input_pos[2] = {0, 1};
  for(int i = 0; i < 3; i++){
    String user_input = askUserInput(msg, msg_pos, input_pos, 16, true);     //ask for the user pin with maximum 16 characters
    if(!verifyPin(user_input)){                       // give the user 3 max trials to enter the right pin
      lcd.clear();
      lcd.print(">>>Wrong Pin<<<");
      lcd.setCursor(0, 1);
      lcd.print("   Try Again   ");
      delay(1000);
    }
    else{                                             // if the user entered the right pin give access to change his pin
      for(int i = 0; i < 3; i++){
        msg = ">>>>New Pin<<<<";
        String user_input1 = askUserInput(msg, msg_pos, input_pos, 16, true);
        msg = ">>>Re-enter Pin<<<";
        String user_input2 = askUserInput(msg, msg_pos, input_pos, 16, true);
        if(user_input1 != user_input2){               // give the user 3 trials max to ensure the two user_input
          lcd.clear();                                // give user 3 switch trials to change pins
          lcd.print("Pin Mismatch");                  
          delay(1000);
        }
        else{
          strcpy(access.pin, user_input1.c_str());
          writeEEPROM();
          lcd.clear();
          lcd.print(">>>Pin Changed<<<");
          lcd.setCursor(0, 1);
          lcd.print(">>Successfully<<");
          delay(1000);
          return ;
        }
      }
    }
  }
  
  return;
}

/* this function will present the user 
 *  the option of adding or removing
 *  numbers from systems EEPROM
 */
void editPhoneNumber(){
  String msg = ">>>Enter Pin<<<";
  int msg_pos[2] = {0, 0};
  int input_pos[2] = {0, 1};
  for(int i = 0; i < 3; i++){
    String user_input = askUserInput(msg, msg_pos, input_pos, 16, true);     //ask for the user pin with maximum 16 characters
    if(!verifyPin(user_input)){                       // give the user 3 max trials to enter the right pin
      if(i == 2){
        lcd.clear();
        lcd.print("Max Trials");
        delay(1000);
        return ;
      }
      lcd.clear();
      lcd.print(">>>Wrong Pin<<<");
      lcd.setCursor(0, 1);
      lcd.print("   Try Again   ");
      delay(1000);
    }
    else{
      break;
    }
  }
  lcd.clear();
  lcd.print("1 ->Add Phone#");
  lcd.setCursor(0, 1);
  
  lcd.print("2 ->Remove Phone#");
  unsigned long duration = millis();
  char key = 'q';
  while((key=keypad.getKey()) == NO_KEY){
    if(millis() - duration > 10000) break;
  }
  lcd.clear();
  // User enters 1 to add a phone number
  if (key == '1') {
    // Check to see if already 3 numbers in the system
    // If there are already 3 numbers then don't allow user to add another
    if (String(access.num1).length() && String(access.num2).length() && String(access.num3).length()){
      lcd.clear();
      lcd.print("System already");
      lcd.setCursor(0, 1);
      lcd.print("3 phone numbers");
    }
    // Allow user to add another number if system is not full with three numbers already
    else{
      addPhoneNumber();
    }
    
  }
  
  // User enters 2 to remove a phone number
  else if (key == '2') {
    removePhoneNumber();
  }
  else{
    return ;
  }
}

void addPhoneNumber(){
  String msg = ">>Enter Phone#<<";
  int msg_pos[2] = {0, 0};
  int input_pos[2] = {6, 1};
  String phoneNumber;
  for (int i = 0; i < 3 ; i++){
    const String user_input = askUserInput(msg, msg_pos, input_pos, 9, false);     //ask for the user pin with maximum 9 characters which is that of mobile phone number in Cameroon
    if(validatePhoneNumber(user_input)){
      phoneNumber = user_input;
      lcd.clear();
      lcd.print(phoneNumber);
      lcd.print(" added");
      lcd.setCursor(0, 1);
      lcd.print("Successfully");
      delay(3000);
      break;
    }
    else{
      lcd.clear();
      lcd.print("Phone Number");
      lcd.setCursor(0, 1);
      lcd.print("is to be 9chars");
      delay(1000);
      // This shows that the user has 3 trials in inputting the phone number correctly
      if(i == 2){
        delay(500);
        lcd.clear();
        lcd.print("Passed Max Trials");
        return ;
      }
    }
  }
  
  switch(checkFreeNumPosition()){
    case 1:
      strcpy(access.num1, phoneNumber.c_str());
      break;
    case 2:
      strcpy(access.num2, phoneNumber.c_str());
      break;
    case 3:
      strcpy(access.num3, phoneNumber.c_str());
      break;
  }
  writeEEPROM();
}

void removePhoneNumber(){
  if (checkFreeNumPosition()){
    lcd.clear();
    String msg = ">>Enter # To Del<<";
    int msg_pos[2] = {0, 0};
    int input_pos[2] = {6, 1};

    for (int i = 0; i < 3; i++){
      const String user_input = askUserInput(msg, msg_pos, input_pos, 9, false);     //ask for the user pin with maximum 9 characters which is that of mobile phone number in Cameroon
      if (user_input == phoneNumber1){
        lcd.clear();
        lcd.print(phoneNumber1);
        lcd.print(" deleted");
        lcd.setCursor(0, 1);
        lcd.print("Successfully");
        delay(3000);
        strcpy(access.num1, "");
        break;
      }
      else if (user_input == phoneNumber2){
        lcd.clear();
        lcd.print(phoneNumber2);
        lcd.print(" deleted");
        lcd.setCursor(0, 1);
        lcd.print("Successfully");
        strcpy(access.num2, "");
        delay(3000);
        break;
      }
      else if (user_input == phoneNumber3){
        lcd.clear();
        lcd.print(phoneNumber3);
        lcd.print(" deleted");
        lcd.setCursor(0, 1);
        lcd.print("Successfully");
        strcpy(access.num3, "");
        delay(3000);
        break;
      }
      else{
        lcd.clear();
        lcd.print("Number Not Found");
        lcd.setCursor(0, 1);
        lcd.print("Try Again");
        delay(2000);
        if(i == 2){
          lcd.clear();
          lcd.print("Max Trials");
          delay(1000);
          break;
        }
      }
    }
    writeEEPROM();
  }
  else {
    lcd.clear();
    lcd.print("System has no");
    lcd.setCursor(0, 1);
    lcd.print("Numbers yet");
    delay(2000);
  }
}

/*
 * This function will check that the 
 * phone number entered is nine digits (Cameroon)
 */
boolean validatePhoneNumber(String phoneNumber){
  if (phoneNumber.length() == 9){
    return true;
  }
  else return false;
}

/*
 * This funtion will help check for
 * a number position that is free in
 * the Access structure
 * 1 means num1 is free
 * 2 means num2 is free
 * 3 means num3 is free
 */
int checkFreeNumPosition(){
  if (!String(access.num1).length()) return 1;
  if (!String(access.num2).length()) return 2;
  if (!String(access.num3).length()) return 3;
  return 0;
}

/*
 * This function will verify a pin entered by the 
 * user with the one in the eeprom
 */
bool verifyPin(String & input_pin){
   if(String(access.pin) == input_pin){
      return true;
   }
   return false;
}

char makeFirstChoice(){
  lcd.clear();
  lcd.print("1-> Edit Phone #");
  lcd.setCursor(0, 1);
  lcd.print("2-> Edit pin");
  long int duration = millis();
  char key = 'q';
  while((key=keypad.getKey()) == NO_KEY){
    if(millis() - duration > 10000) break;
  }
  lcd.clear();
  return key;
}

/*
 * This function will be able to get input from the user
 * by displaying an appropriate message on a starting 
 * position of the lcd and setting the cursor to the point
 * from where will start inputting. It also takes the max
 * length of the input
 */
const String askUserInput(const String & msg, const int * msg_pos, const int * input_pos, int max_input, boolean hideInput){
  String user_input = "";
  int cur_pos = input_pos[0];
  lcd.clear();
  lcd.setCursor(msg_pos[0], msg_pos[1]);
  lcd.print(msg);
  lcd.setCursor(input_pos[0], input_pos[1]);
  float duration = millis()/1000.0;
  while(user_input.length() <= max_input){
    char key = keypad.getKey();
    // pressing * means the user made a mistake and wants to delete
    if(key == '*' && user_input.length() > 0){
      duration = millis()/1000.0;
      lcd.clear();
      lcd.setCursor(msg_pos[0], msg_pos[1]);
      lcd.print(msg);
      cur_pos =  input_pos[0];
      lcd.setCursor(cur_pos, 1);
      user_input.remove(user_input.length()-1, 1);      // remove the last character of the string
      if (hideInput){
        for(int i = 0; i < user_input.length(); i++){
        lcd.write('*');
        cur_pos++;                                      // increment the cursor position as u fill back the data in user_input
        }
      }
      lcd.print(user_input);
      cur_pos += user_input.length();
      
    }
    // pressing # means the user wants to submit the alread inputted data
    else if(key == '#'){
      break;
    }
    else{
      if(key != NO_KEY){
        lcd.write(key);         // display the key
        user_input += key;      // append key to user input
        
        if (hideInput){
          delay(100);             // delay for 100ms
          lcd.setCursor(cur_pos, 1);
          lcd.write('*');
        }
       
        cur_pos++;
        duration = millis()/1000.0;
      }
    }

    // break the loop if the user delays in entering input
    if((millis()/1000.0 - duration) > 10.0){
      lcd.clear();
      lcd.print("No valid input");
      delay(3000);
      user_input = "";
      return "";
//      break;
    }
  }
  
  return user_input;
}

