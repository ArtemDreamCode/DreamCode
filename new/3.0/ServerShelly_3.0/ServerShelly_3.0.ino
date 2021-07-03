#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <ESP8266HTTPClient.h>
#include <EEPROM.h>
#include "UnitFlash.h"


ESP8266WebServer server(80);

WiFiClient client;  
String Device_GUID = "dDf5FFShellysde";
bool RealCheck = false;
int bt_state;
String MacAdr;
String ClassDevice = "Shelly";
String DeviceFrendlyName = "New Robotic Device";
//const char* ssid = "R_302";
//const char* pass = "ProtProtom";
//const char* ssid = "rostelecom_104";
//const char* pass = "63030723";


const char* ssid = "TP-LINK_120";
const char* pass = "160193ya";


//const char* ssid = "iPhonexc5";
//const char* pass = "12345qAz";

//const char* ssid = "iPhonezx";
//const char* pass = "123456789";

//const char* ssid = "ESPap";
//const char* pass = "123456789";

void DoCheckButtonState(){
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
     eeprom_write_state(RealCheck);
  }
  else
  {
    if (RealCheck) //realcheck уже начитался из памяти 
       digitalWrite(4, HIGH); //включаем
    else      
       digitalWrite(4, LOW); //выключаем
  }
 
  bt_state = bt; //готовы снова ловить выключатель  
}

void wifi_begin(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);  
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(2, HIGH);  
    Serial.println("connecting to wifi");
    delay(450);
    DoCheckButtonState();
  }
  Serial.println(WiFi.localIP());
  digitalWrite(2, LOW); 
  Serial.println("connected"); 
  MacAdr = WiFi.macAddress(); //8C:AA:B5:7B:13:73
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void restServerRouting() {
  
    server.on("/", HTTP_GET, []() {
        server.send(200, F("text/html"),
            F("Welcome to the REST Shelly Server"));
    });
    server.on(("/relay"), HTTP_GET, handle_ChangeState); // get from user
    server.on(("/state"), HTTP_GET, handle_GetState); // get from AP
    server.on(("/set"), HTTP_GET, handle_ChangeFrendlyName); // change device name
}
void setup()
{
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW); 
  //Serial.begin(115200);
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  pinMode(5, INPUT);
  DeviceFrendlyName = eeprom_read_name();
  RealCheck = eeprom_read_state();
  bt_state = digitalRead(5);
  Serial.println(RealCheck);
  wifi_begin();
  restServerRouting();
  
  server.onNotFound(handleNotFound);
  server.begin();
}

void handle_GetState(){ //запрос о состоянии от клиента
  String resp;
  String st;
   String response = "{"; 
   if (RealCheck == true){
     st = "on";
   }
   else{
     st = "off";
   }
   response+= "\"device_guid\": \""+Device_GUID+"\"";
   response+= ",\"state\": \""+st+"\"";
   response+= ",\"ip\": \""+WiFi.localIP().toString()+"\"";
   response+= ",\"class\": \""+ClassDevice+"\""; 
   response+= ",\"name\": \""+DeviceFrendlyName+"\""; 
   response+= ",\"mac\": \""+MacAdr+"\"";   
   response+= ",\"gw\": \""+WiFi.gatewayIP().toString()+"\"";
   response+= ",\"nm\": \""+WiFi.subnetMask().toString()+"\"";
   response+= ",\"signalStrengh\": \""+String(WiFi.RSSI())+"\"";
   response+= ",\"chipId\": \""+String(ESP.getChipId())+"\"";
   response+= ",\"flashChipId\": \""+String(ESP.getFlashChipId())+"\"";
   response+= ",\"flashChipSize\": \""+String(ESP.getFlashChipSize())+"\"";
   response+= ",\"flashChipRealSize\": \""+String(ESP.getFlashChipRealSize())+"\"";
   response+= ",\"freeHeap\": \""+String(ESP.getFreeHeap())+"\"";
   response+="}";
  
   server.send(200, "text/html", response);
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
    eeprom_write_state(RealCheck);
  }
  
  String response = "{"; 
  String st;
  if (RealCheck == true){
    st = "on";
   }
   else{
     st = "off";
   }
   response+= "\"state\": \""+st+"\"";
   response+="}";
   server.send(200, "text/html", response);
}

void handle_ChangeFrendlyName(){
  String nm;
  if ((server.args() == 1) && (server.argName(0) == "name")) {
    nm = server.arg(0);
    DeviceFrendlyName = nm;
    eeprom_write_name(DeviceFrendlyName);
    }
   String response = "{"; 
   response+= "\"name\": \""+DeviceFrendlyName+"\"";
   response+="}";
  
   server.send(200, "text/html", response);
}

void loop()
{
  DoCheckButtonState();
  server.handleClient();  
  Serial.println("DeviceFrendlyName: " + DeviceFrendlyName);
  delay(200);
}
