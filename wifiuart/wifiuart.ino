#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

IPAddress ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

const char *ssid = "TempRData";
const char *pass = "temperamentum";

ESP8266WebServer server(80);

void handleRoot() {
  for ( uint8_t i = 0; i < server.args(); i++ ) {
    Serial.println(server.argName(i) + ": " + server.arg(i));
  }
}

void setup() {
  //serial config
  Serial.begin(115200);
  Serial.println();

  //access point config
  Serial.print("Setting soft-AP configuration... ");
  Serial.println(WiFi.softAPConfig(ip, gateway, subnet) ? "Ready" : "Failed!");
  
  Serial.print("Setting soft-AP... ");
  Serial.println(WiFi.softAP(ssid, pass, 10, false) ? "Ready" : "Failed!");

  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());

  server.on("/", HTTP_POST, handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
}
