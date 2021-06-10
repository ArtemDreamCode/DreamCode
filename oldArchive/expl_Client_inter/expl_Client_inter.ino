#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

//#include <ESP8266HttpClient.h>
//#include <ESP8266HTTPClient.h>
//#include <ESP8266WiFi.h>

#define DEBUG

const char* ssid = "143";
const char* pass =  "12345678";
//const char* ssid = "Legobar";
//const char* pass = "Raspberry";
//const char* ssid = "Keenetic-8243";
//const char* pass = "vBiM8chs";
HTTPClient http; 
WiFiClient client;
unsigned long previousMillis = 0;
const long period = 4000; 

//const String IPLamp_0 = "http://192.168.0.40";
const String IPLamp_0 = "http://192.168.0.101";
bool stat = false;
bool wstat = false;
bool bstat = false;
int f_on, fw_on, fb_on = 0;
int f_off, fw_off, fb_off = 0;
bool f_stat_on, fw_stat_on, fb_stat_on = false;
const int wakeUpPin = 4;

int WakeUpState = 0;
int lightState = 0;
int _click_on_state = 0;

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

void ICACHE_RAM_ATTR InterruptWakeUp()
{
      detachInterrupt( digitalPinToInterrupt (wakeUpPin) );
      _click_on_state = 1;
}

void wifi_begin()
{
  WiFi.begin(ssid, pass);  
  log("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    log("...");
  }
  log("done.");
}

void setup() {
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);
  pinMode(wakeUpPin, INPUT_PULLUP);
  
  initlog();
  
  //WiFi.disconnect(); 
  log("wifi is down\n");
  WiFi.forceSleepBegin();
  delay(1);
  
  attachInterrupt( digitalPinToInterrupt (wakeUpPin), InterruptWakeUp, FALLING);
}

String get(String Arequest){
  http.begin(client, Arequest); 
  log(Arequest);
  int httpCode = http.GET();  
  String response = http.getString(); 
  
 // log(httpCode);  
//  log(response);   
  http.end();
  log("Free heap: " + String(ESP.getFreeHeap()));
  return response;
}

void changeturn(){
  if (getstat_light(IPLamp_0 + "/status")){
    get(IPLamp_0 + "/relay/0?turn=off");
    digitalWrite(0, HIGH);
    log("off");        
    }
  else{
    get(IPLamp_0 + "/relay/0?turn=on");
    digitalWrite(0, LOW);
    log("on");    
    }
}
/*
void changeCustomturn(){
  if (getstat_light(IPLamp_0 + "/status")){
    digitalWrite(0, LOW);
    log("Custom status: on"); 
       
    }
  else{
    digitalWrite(0, HIGH);
    log("Custom status: off");    
    }
}

void changewhite(){
  int w = getstat_white(IPLamp_0 + "/status");
  int k = (w / 10) * 10;
  if (k == 100){
    k = 0;
  }
  k += 10;
  String s = get(IPLamp_0 + "/light/0?white=" + String(k));
  
  //log(String(w));
 // log(String(k));
 // delay(3000);
}

void changebright(){
  int b = getstat_bright(IPLamp_0 + "/status");
  int k = (b / 10) * 10;
  if (k == 100){
    k = 0;
  }
  k += 10;
  String s = get(IPLamp_0 + "/light/0?turn=on&brightness=" + String(k));
  
 // log(String(b));
 // log(String(k));
//  delay(3000);
}

*/
void _click_on(){
  log("кнопка нажата");
  WiFi.forceSleepWake();
  wifi_begin();
  log("connect to wifi is done\n");
  delay(1);
  changeturn();
  //WiFi.disconnect(); 
  log("wifi is down\n");
  WiFi.forceSleepBegin();
  delay(1);
  WakeUpState = 0;
  attachInterrupt( digitalPinToInterrupt (wakeUpPin), InterruptWakeUp, FALLING);
}
/*
void click_on(){
  int data_on = digitalRead(4);
  if (data_on == 1){ 
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
        changeturn();
      }
      else{
        changeturn();              
      }
    }
    else {

    }
  f_on = 0;
  f_off = 0;    
  }  
  
 }
 
bool getstat_light(String Arequest){
 String s = get(Arequest); 
 bool f = false;
 int n = s.indexOf("ison");
 if (n > -1) {
   String rr =  s.substring(n + 6, n + 11);
   int k = rr.indexOf("true");
   if (k > -1){
    f = true;
   }
   else {
    f = false;
   }
 }
 return f;
}

int getstat_white(String Arequest){
 String s = get(Arequest);
 int n = s.indexOf("white");
 if (n > -1) {
   String rr = s.substring(n + 6, n + 11);
   int k = rr.indexOf(":");
   int g = rr.indexOf(",");
   
   String r = rr.substring(k + 1, g);
   const char* c_inp = r.c_str();
   int res = atoi(c_inp);
   return res;   
 }
}

int getstat_bright(String Arequest){
 String s = get(Arequest);
 int n = s.indexOf("brightness");
 if (n > -1) {
   String rr = s.substring(n + 12, n + 17);
   int k = rr.indexOf(":");
   int g = rr.indexOf(",");
 //  log(rr);
   String r = rr.substring(k + 1, g);
 //  log(r);
   const char* c_inp = r.c_str();
   int res = atoi(c_inp);
   return res;   
 }
}

void click_white(){
  int data_wh = digitalRead(13);  
  if (data_wh == 1){ 
    fw_on = 1; 
  }
  else {
    fw_off = 2;
  }
  
  if (fw_on + fw_off == 3){
    wstat = not wstat; 
    if (wstat){

      fw_stat_on = not fw_stat_on; 
      if (fw_stat_on){
        changewhite();
      }
      else{
        changewhite();              
      }
    }
    else {

    }
  fw_on = 0;
  fw_off = 0;    
  }   
 }

void click_bright(){
  int data_br = digitalRead(14);
  if (data_br == 1){ 
    fb_on = 1; 
  }
  else {
    fb_off = 2;
  }
  
  if (fb_on + fb_off == 3){
    bstat = not bstat; 
    if (bstat){

      fb_stat_on = not fb_stat_on; 
      if (fb_stat_on){
        changebright();
      }
      else{
        changebright();              
      }
    }
    else {

    }
  fb_on = 0;
  fb_off = 0;    
  }   
 }

void check_custom_state(){
  unsigned long currentMillis = millis(); // сохраняем текущее время
  if (currentMillis - previousMillis >= period) { 
    previousMillis = currentMillis; 
    changeCustomturn();
  }
}
*/

void loop(){
  if ( _click_on_state == 1){ _click_on(); _click_on_state=0;}
  delay(1000);
  //_click_on();
  // get(IPLamp_0 + "/relay/0?turn=off");
  //int data_on = digitalRead(4);
  //if (data_on == 0) log("кнопка не нажата");
  //if (data_on == 1) log("кнопка нажата");
  //delay(500);
 // click_white();
 // click_bright();
//  check_custom_state(); 
}
