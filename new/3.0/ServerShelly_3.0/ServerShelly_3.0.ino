#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266HTTPClient.h>

ESP8266WebServer server(80);

WiFiClient client;

bool RealCheck = false;
int bt_state = 0;
String MacAdr;


const char* ssid = "R_302";
const char* pass = "ProtProtom";

//const char* ssid = "ESPap";
//const char* pass = "123456789";


void wifi_begin(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);  
  while (WiFi.status() != WL_CONNECTED) {
    delay(900);
  }
  MacAdr = WiFi.macAddress(); //8C:AA:B5:7B:13:73
  MacAdr = deleteColon(MacAdr); //убираем из мак-адреса символы двоеточия
}

String deleteColon(String str){ //убираем из мак-адреса символы двоеточия
  char buf[13];
  int j=0;
  for (int i=0; i<str.length(); i++){
    if (str[i] != ':'){
      buf[j] = str[i];
      j++;
    }
  }
  buf[12] = '\0';
  str = buf;
  return str;
}

void setup()
{
  //Serial.begin(115200);
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  pinMode(5, INPUT);
  wifi_begin();

  server.on("/relay", handle_ChangeState);
  server.on("/state", handle_GetState);
  
  server.begin();
}

void handle_GetState(){ //запрос о состоянии от клиента
  String resp;
  if (RealCheck == true)
  {
    resp = "1"+ MacAdr;
    server.send(200, "text/html", resp);
  }
  if (RealCheck == false)
  {
    resp = "0"+ MacAdr;
    server.send(200, "text/html", resp);
  }
}

void handle_ChangeState()
{
  String state_buf;
  if ((server.args() == 1) && (server.argName(0) == "turn"))
  //если аргумент один и он "turn"
  {
    state_buf = server.arg(0);
    if (state_buf == "on")
    {
      RealCheck = true;
      digitalWrite(4, HIGH); //выключаем
      //включить лампочку
    }
    else if(state_buf == "off")
    {
      RealCheck = false;
      digitalWrite(4, LOW); //выключаем
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
