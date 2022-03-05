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
const uint32_t ConstTimerGet = 5000;
int PingIn = 0; //проверка, если сервер удалил устройство

String ServerAddr = "http://192.168.1.18:7890/";

String ShellySSID = "";
String ShellyPASS = "11001100";

const char* ssid = "TP-LINK_120";
const char* pass = "160193ya";

String ServerSSID = "";
String ServerPASS = "12345678"; //MainAP //rasp

String GenerateAPName()
{
  String addr = WiFi.macAddress(); 
  addr.remove(0, 9);
  String ssidname = "NewDev" + addr; 
  return ssidname;
}

void setup()
{
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW); 
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  pinMode(5, INPUT);


  //для прошивки нового устройства - раскомментировать
  //eeprom_default();
  //eeprom_write_name("Serv");
  //eeprom_write_server_ssid("MainAP");

    
  DeviceFrendlyName = eeprom_read_name();
  RealCheck = eeprom_read_state();
  DeviceIndex = eeprom_read_index();
  bt_state = digitalRead(5);
  Serial.println(RealCheck);
  restServerRouting();
  
  server.onNotFound(handleNotFound);
  server.begin();
  
  if (!is_station()){
    if (set_AP_mode()) Serial.println("Точка доступа создана!");
    else Serial.println("Ошибка создания точки доступа!"); }
  else { //режим станции, подключаемся к serverAP
    ServerSSID = eeprom_read_server_ssid();
    wifi_begin(ServerSSID, ServerPASS);}
  if(!is_new_device()) { handle_Reset(); }

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
    {
      if (PingIn>=5)
      {
        String AData = GetAllStateData();
        Serial.println("js Data to send : "+ AData);      
        String resp = post(ServerAddr, AData);
        PingIn = 0;
      }
      else PingIn++; 
    }
  }
  DoCheckButtonState();
  server.handleClient();  
  delay(100);
}
