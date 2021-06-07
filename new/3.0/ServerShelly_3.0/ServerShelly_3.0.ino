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
String local_ip;

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

void wifi_begin(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);  
  log("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(900);
    log("...");
  }
  log("done.");

  
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  local_ip = WiFi.localIP().toString();
  Serial.println(local_ip);
  
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

  
  message = "http://192.168.4.1/new?ip=" + local_ip;
  //resp = get(message); //говорим серверу, что мы новое устройство
  Serial.println(resp);


  server.on("/", handle_Connect);
  server.on("/relay", handle_ChangeState);
  server.on("/state", handle_GetState);
  
  server.begin();
  //todo
  //если епром пуст
  
  //id = get("http://192.168.4.1/new"); //говорим серверу, что мы новое устройство
  //записываем в епром
  //иначе считываем айди из епрома
}

void handle_Connect(){
  server.send(200, "text/html", "U ar connect");   
}

void handle_GetState(){
   if (RealCheck == true)
     server.send(200, "text/html", "1");      
   if (RealCheck == false)
     server.send(200, "text/html", "0");      
}


void handle_ChangeState()
{
  Serial.println("ChangeState");
  String state_buf;
  if ((server.args() == 1) && (server.argName(0) == "turn")) 
  //если аргумент один и он "turn"
  {
    state_buf = server.arg(0);
    if (state_buf == "on")
    {
      RealCheck = true;
      digitalWrite(4, HIGH); //выключаем
      Serial.println("turn on");
      //включить лампочку
    }
    else if(state_buf == "off")
    {
      RealCheck = false;
      digitalWrite(4, LOW); //выключаем
      Serial.println("turn off");
      //выключить лампочку
    }
  }
}

void loop()
{
  int bt = digitalRead(5);
  if (bt != bt_state)
  { //если есть изменение состояния кнопки
     if (!RealCheck)
     {
       RealCheck = true;
       digitalWrite(4, HIGH); //включаем
     }
     else if (RealCheck)
     {
       RealCheck = false;
       digitalWrite(4, LOW); //выключаем
     }
  }
  bt_state = bt; //готовы снова ловить выключатель
  server.handleClient();  
delay(200);
}
