#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// defines pins numbers
const int trigPin = 2;//D4 5;  //D2
const int echoPin = 0;//D3 4;  //D1

const int ledPin = 5; //D1

// defines variables
double duration;
float distance;
float constDistance=0;
double inches, cm;

//device ID should be different for different devices
const int devID=102;
const char* ssid = "YouAreNotSkynet";
const char* password = "thisislaptop";
 
const char* host = "192.168.137.90";
const char* streamId = "collectdata.php";
 
void sendRequest(int dist){
  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;

  HTTPClient http;
  
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  Serial.println("Connection Established!!");
  // We now create a URI for the request
  String url = "http://192.168.137.90/php/";
  url += streamId;
  url += "?DID=";
  url += devID;
  url += "&Distance=";
  url += dist;
 
 Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  http.begin(url);
  int httpCode = http.GET();            //Send the request
  String payload = http.getString();    //Get the response payload
 
  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload
 
  http.end();
  
  
  delay(10);
  Serial.println();
  Serial.println("closing connection");
 
  delay(2500);
  }
 
void setup() {

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
 
// We start by connecting to a WiFi network
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  //Setting up IP address for this NodeMCU
  IPAddress subnet(255,255,255,240);
  WiFi.config(IPAddress(172,20,10,22),IPAddress(172,20,10,15),subnet);  //Change these values for additional devices.
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
}
 
void loop() {
//LED Glows if power is supplied
  digitalWrite(ledPin, HIGH);
  
// Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

// Calculating the distance
  distance= ((duration/2) / 29.1);

// Setting up constant distance for the first time 
  if(constDistance==0)
  {
    constDistance=distance-3;
  }

//Compairing Calculated distance from the Constant Distance for aor Threats
  if(distance<constDistance)
  {
    Serial.print("Distance: ");
    Serial.println(distance);
    sendRequest(distance);
  }
  delay(200);
}
