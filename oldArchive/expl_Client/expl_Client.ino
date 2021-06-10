#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

const char* ssid = "TP-LINK_120";
const char* pass =  "160193ya";
//const char* ssid = "Legobar";
//const char* pass = "Raspberry";
//const char* ssid = "Keenetic-8243";
//const char* pass = "vBiM8chs";
HTTPClient http; 
WiFiClient client;
const String IPLamp_0 = "http://192.168.0.80";
bool stat = false;
int f_on, f_wh = 0;
int f_off = 0;
bool f_stat_on = false;


void initlog(){
  Serial.begin(115200); 
} 

void log(String AMessage){
  Serial.println(AMessage); 
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

void setup() {
  initlog();
  WiFi.begin(ssid, pass);  
  pinMode(0, OUTPUT);
  log("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    log(".");
  }
  log("done.");
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
    get(IPLamp_0 + "/light/0?turn=off");
    digitalWrite(0, HIGH);
    log("off"); 
       
    }
  else{
    get(IPLamp_0 + "/light/0?turn=on");
    digitalWrite(0, LOW);
    log("on");
    
    }
}

void change_on(){
   int data_on = digitalRead(4);
  String resp;
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

void change_white(){
  int data_wh = digitalRead(13);  
  if (data_wh == 1){ 
    int w = getstat_white(IPLamp_0 + "/status");   
    if (w >= 100) {
      w = 0;
      }   
    w++;
    String s = get(IPLamp_0 + "/light/0?white=" + String(w));  
    log(String(w));
    delay(100); 
  }
 }

void change_bright(){
  int data_wh = digitalRead(14);
  if (data_wh == 1){   
    int w = getstat_bright(IPLamp_0 + "/status");
    if (w >= 100) {
      w = 0;
    }
    w++;
    String s = get(IPLamp_0 + "/light/0?turn=on&brightness=" + String(w));
    log(String(w));
    delay(100); 
  }
 }

void loop() { 
  change_on();
  change_white();
  change_bright();
}
