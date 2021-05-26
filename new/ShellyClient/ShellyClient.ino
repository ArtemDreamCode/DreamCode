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
bool RealCheck = false;
String resp;
const char* ssid = "iPhonexc5"; //гараж
const char* pass = "12345qAz";
int bt_state = 0;
unsigned long previousMillis = 0;

#define DEBUG;

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
    delay(900);
    log("...");
  }
  log("done.");
}

String get(String Arequest){
  http.begin(client, Arequest); 
  log(Arequest);
  int httpCode = http.GET();  
  String response = http.getString(); 
  
 // log(httpCode);  
//  log(response);   
  http.end();
  //log("Free heap: " + String(ESP.getFreeHeap()));
  return response;
}

void setup() {
  pinMode(4, OUTPUT);
  pinMode(5, INPUT);
  initlog(); 
  wifi_begin();

}
void loop(){
  int bt = digitalRead(5);
  if (bt != bt_state){
     if (RealCheck)
      {
      RealCheck = false;
      digitalWrite(4, LOW);
      }
     else
      {
      RealCheck = true;
      digitalWrite(4, HIGH);
      }
      }

  if (RealCheck){
    resp = get("http://172.20.10.11/MyStateOn");}
  else{
    resp = get("http://172.20.10.11/MyStateOff");}
    
  if (resp == "On"){
      log("Get on");
      RealCheck = true;
      digitalWrite(4, HIGH);
      }
    if (resp == "Off"){
      log("Get off");
      RealCheck = false;
      digitalWrite(4, LOW);
    }

  bt_state = bt;
  delay(300);
  
}
