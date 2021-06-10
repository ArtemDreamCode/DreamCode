#include "ESP8266WiFi.h"
#include "ESP8266HTTPClient.h"
//#include <ArduinoJson.h>

const String IPLamp_0 = "http://192.168.0.80"; 
const String IPRel_0 = "http://192.168.0.100"; 


const char* ssid = "TP-LINK_120";
const char* pass =  "160193ya";
//const char* ssid = "Legobar";
//const char* pass = "Raspberry";
//const char* ssid = "Keenetic-8243";
//const char* pass = "vBiM8chs";
bool stat = false;
int f_on = 0;
int f_off = 0;
bool f_stat_on = false;
HTTPClient http; 

void InitLog() {
  Serial.begin(115200);  
}

void Log(String ALogInfo){
  Serial.println(ALogInfo); 
}

void setup() {
  InitLog();
  DoGet(IPLamp_0 + "/light/0?turn=off"); 
 // pinMode(5, OUTPUT); 
}

/*void DoGetRelay(String cUrl){
  HTTPClient http;     
  http.begin(cUrl);
  http.GET();
  String json = http.getString(); 
   const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& root = jsonBuffer.parseObject(json);
  JsonObject& Body_Data = root["Body"]["Data"];
                  if (!root.success()) {
                      Serial.println("parseObject() failed");
                     }
  http.end();  
} 
*/

void DoGet(String cUrl){    
  http.begin(cUrl);
  int httpcode = http.GET();
  String httpbody= http.getString();
  Log(httpbody);
} 

void loop() {   
  int data = digitalRead(4);
  
  if (data == 1){ 
    f_on = 1; 
  }
  else {
    f_off = 2;
  }

  if (f_on + f_off == 3){
    stat = not stat; 
    if (stat){

      f_stat_on = not f_stat_on; 
      if (f_stat_on){
  //      DoGet(IPRel_0 + "/relay/0?turn=on");
       DoGet(IPLamp_0 + "/light/0?turn=on");
   //            digitalWrite(5, LOW);
        Log("on");
       // delay(3000);
      }
      else{
        DoGet(IPLamp_0 + "/light/0?turn=off"); 
      //  DoGet(IPRel_0 + "/relay/0?turn=off"); 
     //   digitalWrite(5, HIGH);
        Log("off");
      //  delay(3000);        
          
      }
    }
    else {

    }
  f_on = 0;
  f_off = 0;    
  }   
    http.end(); 
} // void loop
