#include "CTBot.h"
#include "ESP8266WiFi.h"
#include "ESP8266HTTPClient.h"
#include <ESP8266WebServer.h>

struct TUsers 
{                  
  int CurrDevice;
  int id;
  bool DeviceBrightWhiteState;
}; 

const int cUsersCount = 2;

TUsers Users[cUsersCount];
int UserId[cUsersCount] = {677359579, 666390205};

CTBot myBot;
const String IP_0 = "http://192.168.1.90"; //192.168.0.100 - reley 
const String IP_1 = "http://192.168.0.20"; 

//const char* ssid = "TP-LINK_120";
//const char* pass =  "160193ya";
//const char* ssid = "Legobar";
//const char* pass = "Raspberry";
const char* ssid = "Keenetic-8243";
const char* pass = "vBiM8chs";

const String token = "830921018:AAF03MKEpAXTmeZ6ywbVqw7gydBmf8Ea1Qw";
ESP8266WebServer server(80);
uint8_t LED1pin = D7;
bool LED1status = LOW;
uint8_t LED2pin = D6;
bool LED2status = LOW;

void setup() {
  // initialize the Serial
  Serial.begin(115200);
  Serial.println("Starting TelegramBot...");

  // connect the ESP8266 to the desired access point
  myBot.wifiConnect(ssid, pass);

  // set the telegram bot token
  myBot.setTelegramToken(token);

  // check if all things are ok
  if (myBot.testConnection())
    Serial.println("\ntestConnection [OK]");
  else
    Serial.println("\ntestConnection [Not OK]");
  
  InitUsers();
  
  WiFi.begin(ssid, pass);
  // проверка подключения Wi-Fi к сети
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: "); 
  Serial.println(WiFi.localIP());
  server.on("/", handle_OnConnect);
  server.on("/led1on", handle_led1on);
  server.on("/led1off", handle_led1off);
  server.on("/led2on", handle_led2on);
  server.on("/led2off", handle_led2off);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP server started");
}
void InitUsers(){
  for (int i = 0; i < cUsersCount; i++){
   Users[i].CurrDevice = -1;
   Users[i].id = UserId[i];
   Users[i].DeviceBrightWhiteState = false;
   StartKB(UserId[i]);
  }  
}

void StartKB(int AMessageId){
  int UserIndex = GetCurrUser(AMessageId);  
  CTBotReplyKeyboard myKbd;
 // myKbd.addRow();
  myKbd.addButton("Лампа 1");
 // myKbd.addRow();
  myKbd.addButton("Лампа 2");  
  myKbd.enableResize();  
  myBot.sendMessage(AMessageId, "Ok", myKbd);
  Users[UserIndex].CurrDevice = -1;
}

void DetailKB(int AMessageId){
  CTBotReplyKeyboard myKbd;
  int UserIndex = GetCurrUser(AMessageId);
 // myKbd.addRow();
  myKbd.addButton("Включить");
  myKbd.addButton("Выключить");  
  myKbd.addButton("Прочее");  
  myKbd.addButton("Назад");  
  myKbd.enableResize();   
  myBot.sendMessage(AMessageId, "Ok", myKbd); 
  Users[UserIndex].DeviceBrightWhiteState = false;
}

void BrightWhiteKB(int AMessageId){
  CTBotReplyKeyboard myKbd;
 for (int i = 20; i <= 100; i+=20){
  myKbd.addButton("w " + String(i) + "%");
 } 
 myKbd.addRow();
 for (int i = 20; i <= 100; i+=20){
  myKbd.addButton("b " + String(i) + "%");
 } 
  myKbd.addRow();
  myKbd.addButton("Назад");
  myKbd.enableResize();   
  myBot.sendMessage(AMessageId, "Ok", myKbd); 
}

void DoGet(String cUrl){
  HTTPClient http;     
  http.begin(cUrl);
  http.GET();
  http.end();  
} 

int GetCurrUser(int AMessId){

  for (int i = 0; i < cUsersCount; i++){
    if (Users[i].id == AMessId){
       return i; // в идеале возвращать элемент 
       break;
      }
    }  
}



void loop() {
  TBMessage msg;
 
  if (myBot.getNewMessage(msg)) {
    if (msg.messageType == CTBotMessageText) {
          
          int UserIndex = GetCurrUser(msg.sender.id);
           if (msg.text.equalsIgnoreCase("/start")) {
             StartKB(msg.sender.id);
             myBot.sendMessage(msg.sender.id, "Привет " + msg.sender.firstName);
           //  myBot.sendMessage(msg.sender.id, "sender.id " + String(Users[UserIndex].id));
            }
            if (msg.text.equalsIgnoreCase("Назад")){

              if (!Users[UserIndex].DeviceBrightWhiteState){
                StartKB(msg.sender.id);
              //  myBot.sendMessage(msg.sender.id, "sender.id " + String(Users[UserIndex].id));               
              } else{
                DetailKB(msg.sender.id);   
              //  myBot.sendMessage(msg.sender.id, "sender.id " + String(Users[UserIndex].id));
              }
            }
            if (msg.text.equalsIgnoreCase("/MyUI")){
              myBot.sendMessage(msg.sender.id, "UId: " + String(msg.sender.id)); 
              myBot.sendMessage(msg.sender.id, "UName: " + msg.sender.firstName); 
                 
            }
              if (msg.text.equalsIgnoreCase("Лампа 1")){
                Users[UserIndex].CurrDevice = 0;
                DetailKB(msg.sender.id);
               // myBot.sendMessage(msg.sender.id, "sender.id " + String(Users[UserIndex].id));
              }
             if (msg.text.equalsIgnoreCase("Лампа 2")) {
                Users[UserIndex].CurrDevice = 1;
                DetailKB(msg.sender.id);   
               // myBot.sendMessage(msg.sender.id, "sender.id " + String(Users[UserIndex].id));
            }
            if (msg.text.equalsIgnoreCase("Включить")) {
               if(Users[UserIndex].CurrDevice == 0){
                 DoGet(IP_0 + "/light/0?turn=on"); 
                 DoGet("http://192.168.0.100/relay/0?turn=on");      
               }
               if(Users[UserIndex].CurrDevice == 1){
                 DoGet(IP_1 + "/light/0?turn=on"); 
               }             
            }
            if (msg.text.equalsIgnoreCase("Выключить")) {
               if(Users[UserIndex].CurrDevice == 0){
                 DoGet(IP_0 + "/light/0?turn=off");
                 DoGet("http://192.168.0.100/relay/0?turn=off");  
               }
               if(Users[UserIndex].CurrDevice == 1){
                 DoGet(IP_1 + "/light/0?turn=off"); 
               }             
            }            
            if (msg.text.equalsIgnoreCase("Прочее")) {
            //  myBot.sendMessage(msg.sender.id, "CurrDevice " + String(Users[UserIndex].CurrDevice)); 
              if(Users[UserIndex].CurrDevice > -1){ 
                Users[UserIndex].DeviceBrightWhiteState = true;            
              //  myBot.sendMessage(msg.sender.id, "sender.id " + String(Users[UserIndex].id));
                BrightWhiteKB(msg.sender.id);
              }            
            }

            // to do
           // "w " + String(i) + "%"
           // "b " + String(i) + "%"
           //http://192.168.0.20/light/0?turn=on&brightness=60
          // http://192.168.0.20/light/0?white=38
             if (Users[UserIndex].DeviceBrightWhiteState){
               int len = msg.text.length();
               String value;
        
               String prefix = msg.text.substring(0,1);
               if (len = 6){ 
                 value = msg.text.substring(2,4);
                 if (value == "10") { value += "0";}
               }    
              if (Users[UserIndex].CurrDevice == 0){
               if (prefix == "w"){
                  DoGet(IP_0 + "/light/0?white=" + value);   
                 // myBot.sendMessage(msg.sender.id, "value " + String(value));
                 // myBot.sendMessage(msg.sender.id, "CurrDevice " + String(Users[UserIndex].CurrDevice)); 
                }
                if (prefix == "b"){
                  DoGet(IP_0 + "/light/0?turn=on&brightness=" + value); 
                 // myBot.sendMessage(msg.sender.id, "value " + String(value));
                 //   myBot.sendMessage(msg.sender.id, "CurrDevice " + String(Users[UserIndex].CurrDevice)); 
                }
               }
               if (Users[UserIndex].CurrDevice == 1){
                if (prefix == "w"){
                  DoGet(IP_1 + "/light/0?white=" + value); 
                 // myBot.sendMessage(msg.sender.id, "value " + String(value)); 
                 // myBot.sendMessage(msg.sender.id, "CurrDevice " + String(Users[UserIndex].CurrDevice));  
                }
                if (prefix == "b"){
                 DoGet(IP_1 + "/light/0?turn=on&brightness=" + value);  
                // myBot.sendMessage(msg.sender.id, "value " + String(value));
                // myBot.sendMessage(msg.sender.id, "CurrDevice " + String(Users[UserIndex].CurrDevice)); 
                }
               }
 
   }    
       } //CTBotMessageText
  }

  server.handleClient();
  if(LED1status)
  {DoGet(IP_0 + "/light/0?turn=on");}
  else
  {DoGet(IP_0 + "/light/0?turn=off");}
  
  if(LED2status)
  {digitalWrite(LED2pin, HIGH);}
  else
  {digitalWrite(LED2pin, LOW);}
 
}

void handle_OnConnect() {
  LED1status = LOW;
  LED2status = LOW;
  Serial.println("GPIO7 Status: OFF | GPIO6 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,LED2status)); 
}
void handle_led1on() {
  LED1status = HIGH;
  Serial.println("GPIO7 Status: ON");
  server.send(200, "text/html", SendHTML(true,LED2status)); 
}
void handle_led1off() {
  LED1status = LOW;
  Serial.println("GPIO7 Status: OFF");
  server.send(200, "text/html", SendHTML(false,LED2status)); 
}
void handle_led2on() {
  LED2status = HIGH;
  Serial.println("GPIO6 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status,true)); 
}
void handle_led2off() {
  LED2status = LOW;
  Serial.println("GPIO6 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,false)); 
}
void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}
String SendHTML(uint8_t led1stat,uint8_t led2stat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #1abc9c;}\n";
  ptr +=".button-on:active {background-color: #16a085;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP8266 Web Server</h1>\n";
    ptr +="<h3>Using Station(STA) Mode</h3>\n";
  
   if(led1stat)
  {ptr +="<p>LED1 Status: ON</p><a class=\"button button-off\" href=\"/led1off\">OFF</a>\n";}
  else
  {ptr +="<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/led1on\">ON</a>\n";}
  if(led2stat)
  {ptr +="<p>LED2 Status: ON</p><a class=\"button button-off\" href=\"/led2off\">OFF</a>\n";}
  else
  {ptr +="<p>LED2 Status: OFF</p><a class=\"button button-on\" href=\"/led2on\">ON</a>\n";}
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
