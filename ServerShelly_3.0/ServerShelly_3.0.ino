#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <ESP8266HTTPClient.h>
#include <EEPROM.h>
#include "UnitFlash.h"


ESP8266WebServer server(80);

WiFiClient client;  
String Device_GUID = "dDf5FFShellysde";
int Device_Position = 0;
bool RealCheck = false;
int bt_state;
String MacAdr;
String ClassDevice = "Shelly";
String DeviceFrendlyName = "New Robotic Device";
int DeviceIndex = 0;
//const char* ssid = "R_302";
//const char* pass = "ProtProtom";
//const char* ssid = "rostelecom_104";
//const char* pass = "63030723";


String ShellySSID = "ShellyAP108";
String ShellyPASS = "11001100";

const char* ssid = "TP-LINK_120";
const char* pass = "160193ya";


//const char* ssid = "Keenetic-9462";
//const char* pass = "LCMN8S6X";


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
  wifi_begin();
  restServerRouting();
  
  server.onNotFound(handleNotFound);
  server.begin();

  if(!is_new_device())
  {
      handle_Reset();
  }
}


bool is_new_device()
{
  if (eeprom_read_state_new_device() == 1)
  {
    return true;
  }
  else return false;
}

void loop()
{
  DoCheckButtonState();
  server.handleClient();  
  delay(200);
}
