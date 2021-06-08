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
  server.begin();

  server.on("/Bton", handle_Bton);
  server.on("/Btoff", handle_Btoff);
  server.on("/ChangeState", handle_ChangeState);
  
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  server.begin();
  //todo
  //если епром пуст
  
  //id = get("http://192.168.4.1/new"); //говорим серверу, что мы новое устройство
  //записываем в епром
  //иначе считываем айди из епрома
}

void handle_Bton()
{
   RealCheck = true;
   digitalWrite(4, HIGH); //включаем
   Serial.println("on");
   server.send(200, "text/html", SendHTML());   
}

void handle_Btoff()
{
   RealCheck = false;
   digitalWrite(4, LOW); //выключаем
   Serial.println("off");
   server.send(200, "text/html", SendHTML()); 
}

void handle_ChangeState(){
   Serial.println("ChangeState");
   if (RealCheck) {
      server.send(200, "text/html", "On"); }
   else {
      server.send(200, "text/html", "Off"); }      
}

String SendHTML()
{
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head>";
  ptr +="<meta http-equiv=\"refresh\" content=\"1;http://192.168.4.1\" />";
  //<meta http-equiv="refresh" content="0;https://yutex.ru/index.html">
  ptr +="</head>\n";  
  ptr +="</html>\n";
  return ptr;
}

void loop(){
  int bt = digitalRead(5);
  if (bt != bt_state){ //если есть изменение состояния кнопки
     if (!RealCheck){
       RealCheck = true;
       digitalWrite(4, HIGH); //включаем
     }
     else if (RealCheck){
      RealCheck = false;
       digitalWrite(4, LOW); //выключаем
     }
  }
  bt_state = bt; //готовы снова ловить выключатель

 server.handleClient();  
delay(500);
}
