#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "SSID";
const char* password = "PASSWORD";
const int relayPin = 12;  // active HIGH
const int ledPin   = 13;  // active LOW

ESP8266WebServer server(80);

void handleRoot() {
  digitalWrite(ledPin, LOW);
  server.send(200, "text/plain", "Hello from Sonoff!");
  digitalWrite(ledPin, HIGH);
}

void turnOnRelay() {
  digitalWrite(ledPin, LOW);
  digitalWrite(relayPin, HIGH);
  server.send(204);
}
void turnOffRelay() {
  digitalWrite(ledPin, HIGH);
  digitalWrite(relayPin, LOW);
  server.send(204);
}

void handleNotFound(){
  digitalWrite(ledPin, LOW);
  String message = "File Not Found\n\n";
  server.send(404, "text/plain", message);
  digitalWrite(ledPin, HIGH);
}

void setup(void){
  pinMode(ledPin, OUTPUT);     
  pinMode(relayPin, OUTPUT);        
  digitalWrite(relayPin, LOW);
  digitalWrite(ledPin, LOW);   
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/sonoff1_ON", turnOnRelay);
  server.on("/sonoff1_OFF", turnOffRelay);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
  digitalWrite(ledPin, HIGH);  // led off
}

void loop(void){
  server.handleClient();
}
