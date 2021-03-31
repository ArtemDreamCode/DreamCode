#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
 
#define DEBUG

HTTPClient http; 
WiFiClient client;

//const String IPLamp_0 = "http://192.168.0.40"; // L1
const String IPLamp_0 = "http://192.168.0.50"; // dimmer
//const String IPLamp_0 = "http://192.168.0.100";

const int wakeUpPin = 4; // d2
const int ledPin = 5; // d1
int _click_on_state = 0;
const char* ssid = "TP-LINK_120";
const char* pass =  "160193ya";
unsigned long previousMillis = 0;
const long period = 4000; 

void ICACHE_RAM_ATTR InterruptWakeUp()
{
      detachInterrupt( digitalPinToInterrupt (wakeUpPin) );
      _click_on_state = 1;
}

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
  WiFi.begin(ssid, pass);
  log("wifi do begin ..");
  log("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(ledPin, LOW);
    delay(200);
    digitalWrite(ledPin, HIGH);
    log("...");
  }
  log("wifi is begin");
}

int AHandle(int valg){
  int a = 0;
 return a;
 }

String get(String Arequest){
    http.setTimeout(200);
    http.begin(client, Arequest); 
    log(Arequest);
    int httpCode = http.GET();
   // log(String(httpCode)); 
     
    String response = http.getString();  
  //  log(response);   
    http.end();
    log("Free heap: " + String(ESP.getFreeHeap()));
  

  return response;
}


int getstat_light(String Arequest){ 
 String s = get(Arequest); 
 int f = -1;
 int n = s.indexOf("ison");
 if (n > -1) {
   String rr =  s.substring(n + 6, n + 11);
   int k = rr.indexOf("true");
   if (k > -1){
    f = 1;
   }
   else {
    f = 0;
   }
 }
 else{ // у-во не доступно code -1
   f = -1;
 }
 return f;
}

void changeturn(){
  int stat = getstat_light(IPLamp_0 + "/status");
  log(String(stat));
  if (stat == 1){
    get(IPLamp_0 + "/light/0?turn=off");
    digitalWrite(ledPin, HIGH); // что бы не ждать, сделаем сразу
    log("off");        
    }
  if (stat == 0){
    get(IPLamp_0 + "/light/0?turn=on");
    digitalWrite(ledPin, LOW); // что бы не ждать, сделаем сразу
    log("on");    
    }
}

void ChangeLight(){
  int stat = getstat_light(IPLamp_0 + "/status");
  log(String(stat));
  if (stat == 1){
    digitalWrite(ledPin, LOW); // что бы не ждать, сделаем сразу
    log("on");        
    }
 
  if (stat == 0){
    digitalWrite(ledPin, HIGH); // что бы не ждать, сделаем сразу
    log("off");    
    }

  /*if ((WiFi.status() != WL_CONNECTED) || (stat == -1)) {
     log("WiFi.status() != WL_CONNECTED) || (stat == -1)");
      while (true) {
        stat = getstat_light(IPLamp_0 + "/status");     
        digitalWrite(ledPin, HIGH);
        delay(100);
        digitalWrite(ledPin, LOW);
     }
  }*/
}

 /* 
0: WL_IDLE_STATUS, когда Wi-Fi находится в процессе переключения между статусами

1: WL_NO_SSID_AVAIL в случае, если настроенный SSID не может быть достигнут

3: WL_CONNECTED после успешного установления соединения

4: WL_CONNECT_FAILED, если соединение не удалось

6: WL_CONNECT_WRONG_PASSWORD, если пароль неверен
7: WL_DISCONNECTED, если модуль не настроен в режиме станции*/


void _click_on(){
  log("кнопка нажата");
  delay(1);
  changeturn();
  delay(1);
  attachInterrupt( digitalPinToInterrupt (wakeUpPin), InterruptWakeUp, FALLING);
}
 
