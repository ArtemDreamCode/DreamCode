#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266HTTPClient.h>

ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;
HTTPClient http;
WiFiClient client;

const char* mqtt_server = "farmer.cloudmqtt.com";
WiFiClient espClient;


const char* ssid = "iPhonexc5"; //гараж
const char* pass = "12345qAz";

unsigned long previousMillis = 0;

void initlog(){
#ifdef DEBUG
  Serial.begin(115200); 
#endif
} 

void log(String AMessage){
#ifdef DEBUG
  Serial.println(AMessage); 
#endif
}

void wifi_begin()
{
  WiFi.begin(ssid, pass);  
  log("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    log("...");
  }
  log("done.");
}

void setup() {
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);
  initlog(); 
  wifi_begin();

}
void loop(){
  delay(1000);
}
