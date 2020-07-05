#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
 
const char* ssid = "TP-LINK_D5D323";
const char* password = "23081957";
//int datet = []; 
int ledPin = D7; // GPIO13
WiFiServer server(80);
const long utcOffsetInSeconds = 14400;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
 
void setup() {
  Serial.begin(9600);
  delay(10);
 
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
 
  // Connect to WiFi network
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
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
   timeClient.begin();

}
 
void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // Match the request
 
  int value = LOW;
  if (request.indexOf("/LED=ON") != -1)  {
    digitalWrite(ledPin, HIGH);
    value = HIGH;
  }
  if (request.indexOf("/LED=OFF") != -1)  {
    digitalWrite(ledPin, LOW);
    value = LOW;
  }
 
// Set ledPin according to the request
//digitalWrite(ledPin, value);
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
 
  client.print("Led pin is now: ");
 
  if(value == HIGH) {
    client.print("On");
    //time call
    timeClient.update();
  
  client.println("<br><br>");
  client.println(daysOfTheWeek[timeClient.getDay()]);
  client.print(", ");
  client.print(timeClient.getHours());
  client.print(":");
  client.print(timeClient.getMinutes());
  client.print(":");
  client.println(timeClient.getSeconds());
  
  } else {
    client.print("Off");
    //time call
    timeClient.update();
  
  client.println("<br><br>");
  client.println("<a>");
  client.print(daysOfTheWeek[timeClient.getDay()]);
  client.println("</a>");
  client.print(", ");
  client.print(timeClient.getHours());
  client.print(":");
  client.print(timeClient.getMinutes());
  client.print(":");
  client.println(timeClient.getSeconds());
  }
  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"\"><button>Turn On </button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button>Turn Off </button></a><br />");  
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
 
}
 
