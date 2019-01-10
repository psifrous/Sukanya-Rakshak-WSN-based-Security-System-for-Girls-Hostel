#include <ESP8266WiFi.h>
#include <WiFiClient.h>

// defines pins numbers
const int trigPin = 5;  //D4
const int echoPin = 4;  //D3
int ledStatus = 12; //for LED

// defines variables
double duration;
float distance;
float constDistance=0;
double inches, cm;

//WiFi Credentials
const char* ssid = "Abc";
const char* pass = "12345678";

//Raspberry Pi's IP Address
const char* host = "172.20.10.12";  

//Message that will be sent to the Server when it detects obstricle.
String msg="Warning!!!";

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(ledStatus, OUTPUT);
  delay(500);
  Serial.begin(38400);
  Serial.println();
  Serial.print("conecting to: ");
  Serial.println(ssid);
  Serial.println("Try to connect to server: ");
  Serial.println(host);
  
  WiFi.mode(WIFI_STA); 
  delay(1000);
  WiFi.begin(ssid, pass);
  IPAddress subnet(255,255,255,240);
  WiFi.config(IPAddress(172,20,10,13),IPAddress(172,20,10,15),subnet);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("My IP: ");
  Serial.println(WiFi.localIP());
  long rssi = WiFi.RSSI();
  Serial.print("RSSI: ");
  Serial.print(rssi);
  Serial.println(" dBm");
}

//GLOBAL INITIALIZATION
//Connectint to server after void Setup so that connaction remain open for ever
WiFiClient client;

void loop() {

  if (!client.connect(host,8000)) { //The problem start here
    Serial.println(".");
    return;
  }
  
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);  

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);  
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  digitalWrite(ledStatus, LOW);

  //printing the Host IP if it gets connected
  Serial.println();
  Serial.print("Conected to IP: ");
  Serial.println(host);

  //Formula for calculating distance in 'CM'
  distance= ((duration/2) / 29.1);

  //check if constant distance
  if(constDistance==0)
  {
    constDistance=distance-5;
  }

  //Led Status Glow when value floctuates bello a certain value.
  if(distance<constDistance)
  {
    Serial.println("Sending Alert to Server: ");
    client.println(msg);
    Serial.println("Message Sent.");
    Serial.print("Distance: ");
    Serial.println(distance);
    digitalWrite(ledStatus, HIGH);
    //delay(50);
    //digitalWrite(ledStatus, LOW);
  }
  delay(150);

  /*msg="this is nodemcu";
  Serial.println("Sending string to server: ");
  client.println(msg);
  Serial.println("Message Sent.");
  delay(100);*/

  Serial.println();
  /*Serial.println("Closing connection");
  client.flush();
  client.stop();
  Serial.println("Connection Closed");*/  
}
