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
int bt_state = 0;
String state;
String id; 
String message;

const char* ssid = "ESPap"; //гараж
const char* pass = "123456789";
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
  WiFi.mode(WIFI_STA);
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
  http.end();
  //log("Free heap: " + String(ESP.getFreeHeap()));
  return response;
}

void setup() {
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  pinMode(5, INPUT);
  initlog(); 
  wifi_begin();

  //todo
  //если епром пуст
  id = get("http://192.168.4.1/new"); //говорим серверу, что мы новое устройство
  //записываем в епром
  //иначе считываем айди из епрома
}


void loop(){
  //int bt = 0;
  int bt = digitalRead(5);
  if (bt != bt_state){ //если есть изменение состояния кнопки
     if (!RealCheck){
       RealCheck = true;
       digitalWrite(4, HIGH); //включаем
       message = "http://192.168.4.1/changestate?id=" + id + "&state=" + state;
       resp = get(message);  //говорим серверу, что лампа включилась
     }
     else if (RealCheck){
      RealCheck = false;
       digitalWrite(4, LOW); //выключаем
       message = "http://192.168.4.1/changestate?id=" + id + "&state=" + state;
       resp = get(message); //говорим серверу, что лампа выключилась
     }
  }
  bt_state = bt; //готовы снова ловить выключатель

  
  message = "http://192.168.4.1/state?id=" + id;
  log(message);
  resp = get(message); //спрашиваем у сервера состояние
  log(resp);
  if (resp == "On")
    if (!RealCheck){
      RealCheck = true;
      digitalWrite(4, HIGH);
      Serial.println("Lamp ON");
    }
  if (resp == "Off")
    if (RealCheck){
      RealCheck = false;
      digitalWrite(4, LOW);
      Serial.println("Lamp OFF");
    }
  delay(500);

  log(id);
}
