/*
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 */

#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {  0x90, 0xA2, 0xDA, 0x00, 0x1E, 0x18 }; //MAC address shield
IPAddress ip(192,168,1,20);//145,93,90,119
EthernetClient client;
char serverName[] = "www.domain.com"; //Host domain name

const unsigned long requestInterval = 10000;  // delay between requests
boolean requested;                            // whether you've made a request since connecting

String currentLine = "";            // string to hold the text from server
String tweet = "";                  // string to hold the tweet
boolean readingTweet = false;       // if you're currently reading the tweet

//Pin setup
int analogPin = A0; //Analog pin
int ledPin1 = 9; //LED1 / Relay
int statusledPin1 = 4; //Status LED (green)
int statusledPin2 = 3; //Status LED (red)


//Variable setup
int ServerDelay = 50; //x100ms
int ServerTimeout = 0;
int ServerRequestGET = 1;
String ReqChar = ""; // string to hold character
String ReqTxt = "";  // string to hold the text from server

int statusledState = 0;
int statusledDelay = 50;//ms

int lightState = 0;
int lightStateDelay = 900; //x100ms (0.1s) 600 = 60s
int lightStateTimeout = 0;

String sinceID = "1";
String stateString = ""; //State from server in String
String sinceString = ""; //Since_id from server in String


void setup() {
  Serial.begin(9600);
  
  pinMode(ledPin1, OUTPUT);
  pinMode(statusledPin1, OUTPUT);
  pinMode(statusledPin2, OUTPUT);
  digitalWrite(statusledPin2, HIGH); //Red LED indicator for startup
  
  // attempt a DHCP connection:
  Serial.println("Attempting to get an IP address using DHCP:");
  if (!Ethernet.begin(mac)) {
    // if DHCP fails, start with a hard-coded address:
    Serial.println("failed to get an IP address using DHCP, trying manually");
    Ethernet.begin(mac, ip);
  }
  Serial.print("My address:");
  Serial.println(Ethernet.localIP());
}


void loop(){
  //Server request
  if(ServerRequestGET==1){
    ServerTimeout += 1;
    if(ServerTimeout>=ServerDelay){
      ServerTimeout = 0;
      //Execute GET requests
      connectToServerGET();
      ServerRequestGET = 1;      
    }
  }
  
  
  //Light
  /**
  lightState values:
  0 = Light are off
  1 = Turn lights on
  2 = Lights are on
  */
  
  if(lightState==1){ //Turn lights on
    for (int counter = 0; counter < 6; counter++) {//Blink status LED1
      digitalWrite(statusledPin1, LOW);
      delay(statusledDelay);
      digitalWrite(statusledPin1, HIGH);
      delay(statusledDelay);
    }  
    digitalWrite(ledPin1, HIGH);//Light ON 
    lightState = 2;
    Serial.println("Lights on");
    ServerRequestGET = 0; //Pause server requests
  }

  if(lightState==2){ //If lights are on
    lightStateTimeout += 1;
    if(lightStateTimeout>=lightStateDelay){ //Turn lights off
      digitalWrite(ledPin1, LOW);
      lightState = 0;
      lightStateTimeout = 0;
      Serial.println("Lights off");
      ServerRequestGET = 1; //Continue server requests
    }
  }
  
  if(statusledState==1){ //If startup is complete (first sinceID received)
    digitalWrite(statusledPin1, HIGH);
    digitalWrite(statusledPin2, LOW);
    statusledState=0;
  }

  delay(100); //Wait 0.1 seconds before starting new loop
}


//Get commands
void connectToServerGET() {
  // attempt to connect, and wait a millisecond:
  client.stop();
  Serial.println("connecting to server (GET commands)...");
  if (client.connect(serverName, 80)) {
    Serial.println("making HTTP request (GET commands)...");
    // make HTTP GET request:
    client.print("GET http://domain.com/PontiFlux/getfeed.php?since_id=");
    client.print(sinceID);
    client.println(" HTTP/1.1");
    client.println("HOST: www.domain.com");
    client.println();
    delay(1000);
    while (client.connected() && client.available()){
      char ReqChar = client.read();
      ReqTxt += ReqChar;
    }
    client.stop(); 
    //Serial.print(ReqTxt);
    Serial.println("");
    //State
    int stateReqTxtBegin = ReqTxt.indexOf("<state>")+7;
    int stateReqTxtEind = ReqTxt.indexOf("</state>");
    stateString = ReqTxt.substring(stateReqTxtBegin, stateReqTxtEind);
    int stateInt = stringToInt(stateString);
    if(stateInt==2){
      statusledState = 1;
    }
    if(stateInt==4){
      lightState = 1;
    }
    Serial.print("State: ");
    Serial.print(stateInt);
    Serial.println("");
    
    //Since_id
    int sinceReqTxtBegin = ReqTxt.indexOf("<sinceid>")+9;
    int sinceReqTxtEind = ReqTxt.indexOf("</sinceid>");
    sinceString = ReqTxt.substring(sinceReqTxtBegin, sinceReqTxtEind);
    int sinceInt = stringToInt(sinceString);
    //int sinceInt = stringToInt(sinceString);
    if(stateInt==2||stateInt==4){
      sinceID = sinceString;
    }
    Serial.print("Since_id: ");
    Serial.print(sinceID);
    Serial.println("");
    //Serial.print(ReqTxt);

    
    ReqTxt = "";//Empty string to prevent memory freeze
  } else {
    Serial.println("Connection failed");
  }
}  



/**
 * Converts a String to an integer
 *
 * Returns: The integer represented by the number parameter.
 * Remarks: If the conversion fails, then 0 is returned.
 */
int stringToInt(String number)
{
  int length = number.length() + 1;
  char arrayOfCharacters[length];

  number.toCharArray(arrayOfCharacters, length);
  arrayOfCharacters[length - 1] = '\0';

  return atoi(arrayOfCharacters);
}
