#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DallasTemperature.h>

//network
IPAddress ip(192,168,1,2);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

const char *ssid = "TempRData";
const char *pass = "temperamentum";
const char *host = "http://192.168.1.1/";

//temperature
#define ONE_WIRE_BUS 2  // DS18B20 pin
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

float temp;

//deep sleep time (in seconds)
const int sleepTimeS = 5;

void setup() {
  //serial config
  Serial.begin(115200);
  Serial.println();

  //connect to access point
  WiFi.forceSleepWake();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  WiFi.config(ip, gateway, subnet);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  do {
    DS18B20.requestTemperatures(); 
    temp = DS18B20.getTempCByIndex(0);
    Serial.print("Temperature: ");
    Serial.println(temp);
  } while (temp == 85.0 || temp == (-127.0));

  HTTPClient http;
  http.begin(host);
  http.addHeader("Content-Type", "application/json");
  String payload = String(temp); 
  Serial.print("POST payload: "); Serial.println(payload);
  int httpCode = http.POST(payload);
  Serial.print("HTTP POST Response: "); Serial.println(httpCode); // HTTP code 200 means ok 
  http.end();

  Serial.println("ESP8266 in sleep mode");
  ESP.deepSleep(sleepTimeS * 1000000);
}

void loop() {}
