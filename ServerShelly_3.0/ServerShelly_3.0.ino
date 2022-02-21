#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <ESP8266HTTPClient.h>
#include <EEPROM.h>
#include "UnitFlash.h"


ESP8266WebServer server(80);

WiFiClient client;  
HTTPClient http; // for depricated Get request 

int Device_Position = 0;
bool RealCheck = false;
int bt_state;
String MacAdr;
String ClassDevice = "Shelly";
int DeviceIndex = 0;
String DeviceFrendlyName;
uint32_t TimerGet; //таймер отправки гет-запроса
const uint32_t ConstTimerGet = 30000; //30 cек
//const char* ssid = "R_302";
//const char* pass = "ProtProtom";
//const char* ssid = "rostelecom_104";
//const char* pass = "63030723";

String ServerAddr = "http://192.168.1.2/";

String ShellySSID = "";
String ShellyPASS = "11001100";

const char* ssid = "TP-LINK_120";
const char* pass = "160193ya";

String ServerSSID = "";
String ServerPASS = "12345678"; //MainAP //rasp
//String ServerPASS = "PvE9zyZe"; //lassard

//const char* ssid = "Keenetic-9462";
//const char* pass = "LCMN8S6X";


//const char* ssid = "iPhonexc5";
//const char* pass = "12345qAz";

//const char* ssid = "iPhonezx";
//const char* pass = "123456789";

//const char* ssid = "ESPap";
//const char* pass = "123456789";

String GenerateAPName()
{
  String addr = WiFi.macAddress(); 
  addr.remove(0, 9);
  String ssidname = "NewDev" + addr; 
  return ssidname;
}

void DoCheckButtonState()
{
  String getOut = "";
  int bt = digitalRead(5);
  if (bt != bt_state)
  { //если есть изменение состояния кнопки
     if (!RealCheck)
     {
       RealCheck = true;
       digitalWrite(4, HIGH); //включаем
       getOut = ServerAddr + "switch?turn=on";
     }
     else if (RealCheck)
     {
       RealCheck = false;
       digitalWrite(4, LOW); //выключаем
       getOut = ServerAddr + "switch?turn=off";
       
     }
     eeprom_write_state(RealCheck);
     ////todo отправить гет-запрос на сервер о изменения состояния
     String resp = get(getOut);
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
  DeviceIndex = eeprom_read_index();
  bt_state = digitalRead(5);
  Serial.println(RealCheck);
  restServerRouting();
  
  server.onNotFound(handleNotFound);
  server.begin();
  
  if (!is_station())
  {
    if (set_AP_mode())
      Serial.println("Точка доступа создана!");
    else Serial.println("Ошибка создания точки доступа!");
  }
  else //режим станции, подключаемся к serverAP
  {
    //считываем из eeprom ssid server ap
    ServerSSID = eeprom_read_server_ssid();
    wifi_begin(ServerSSID, ServerPASS);
  }

  if(!is_new_device())
  {
      handle_Reset();
  }
}

bool is_station()
{
  return (eeprom_read_state_wifi_mode() == 1);
}

bool is_new_device()
{
  return (eeprom_read_state_new_device() == 1);
}

void loop()
{
  if ((millis() - TimerGet) >= ConstTimerGet)
  {
    TimerGet = millis();
    if (is_station)
      String resp = post(ServerAddr, JsonToStr()); 
  }
  DoCheckButtonState();
  server.handleClient();  
  delay(100);
}
