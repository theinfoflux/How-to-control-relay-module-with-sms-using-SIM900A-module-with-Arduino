// Include Software Serial library to communicate with GSM
#include <SoftwareSerial.h>
SoftwareSerial SIM900(7, 8);

// Variable to store text message
String textMessage;

// Create a variable to store Lamp state
String Relay1State = "HIGH";
String message;

const int relay1 = 3;  // relay1 connected to pin 3

void setup() {
    pinMode(relay1, OUTPUT);
  digitalWrite(relay1, LOW);
    // Initializing serial commmunication
  Serial.begin(19200); 
  SIM900.begin(19200);

  // Give time to your GSM shield log on to network
  delay(20000);
  Serial.print("SIM900 ready...");
  // AT command to set SIM900 to SMS mode
  SIM900.print("AT+CMGF=1\r"); 
  delay(100);
  // Set module to send SMS data to serial out upon receipt 
  SIM900.print("AT+CNMI=2,2,0,0,0\r");
  delay(100);
}

void loop(){
  if(SIM900.available()>0){
    textMessage = SIM900.readString();
    Serial.print(textMessage); 
    textMessage.toUpperCase();    
    delay(10);
  } 
  
  if(textMessage.indexOf("RELAY1ON")>=0){
    // Turn on relay1 and save current state
    digitalWrite(relay1, HIGH);
    Relay1State = "on"; 
    message = "Relay1 is " + Relay1State;
    sendSMS(message);
    Serial.println("Relay1 set to ON");
    textMessage = ""; 
  }
  if(textMessage.indexOf("RELAY1OFF")>=0){
    // Turn off relay1 and save current state
    digitalWrite(relay1, LOW);
    Relay1State = "off";
    message = "Relay1 is " + Relay1State;
    sendSMS(message);
    Serial.println("relay1 set to OFF");  
    textMessage = "";   
  }
  
  
  if(textMessage.indexOf("RELAY1STATE")>=0){
   message = "Relay1 is " + Relay1State;
    sendSMS(message);
    Serial.println("Relay1 state resquest");
    textMessage = "";
  }
  

}  

// Function that sends SMS
void sendSMS(String message){
  // AT command to set SIM900 to SMS mode
  SIM900.print("AT+CMGF=1\r"); 
  delay(100);

  // REPLACE THE X's WITH THE RECIPIENT'S MOBILE NUMBER
  // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
  SIM900.println("AT + CMGS = \"+92XXXXXXXX\""); 
  delay(100);
  // Send the SMS
  SIM900.println(message); 
  delay(100);
  // End AT command with a ^Z, ASCII code 26
  SIM900.println((char)26); 
  delay(100);
  SIM900.println();
  // Give module time to send SMS
  delay(5000);  
}
