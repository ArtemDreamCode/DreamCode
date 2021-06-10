#include "CTBot.h"
#include "ESP8266WiFi.h"
#include "ESP8266HTTPClient.h"

struct TUsers 
{                  
  int CurrDevice;
  int id;
  bool DeviceBrightWhiteState;
}; 

const int cUsersCount = 1;

TUsers Users[cUsersCount];
//int UserId[cUsersCount] = {677359579, 666390205, 1128794723};
  int UserId[cUsersCount] = {677359579};

CTBot myBot;
const String IPLamp_0 = "http://192.168.0.90"; 
const String IPLamp_1 = "http://192.168.0.20"; 
const String IPRel_0 = "http://192.168.0.100"; 
//const String IPRel_1 = "http://192.168.0.20"; 


const char* ssid = "TP-LINK_120";
const char* pass =  "160193ya";
//const char* ssid = "Legobar";
//const char* pass = "Raspberry";
//const char* ssid = "Keenetic-8243";
//const char* pass = "vBiM8chs";

//const String token = "830921018:AAF03MKEpAXTmeZ6ywbVqw7gydBmf8Ea1Qw";
const String token = "1510508001:AAE-xdOyObmwviBEuOHfqGvD8ojd1TSgtLk";
void InitLog() {
  Serial.begin(115200);  
}

void Log(String ALogInfo){
  Serial.println(ALogInfo); 
}

void setup() {
  InitLog();
  Log("Starting TelegramBot...");
  myBot.wifiConnect(ssid, pass);
  myBot.setTelegramToken(token);

  if (myBot.testConnection())
    Log("\ntestConnection [OK]");
  else
    Log("\ntestConnection [Not OK]");
  
  InitUsers();
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
  myBot.sendMessage(AMessageId, "~", myKbd);
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
  myBot.sendMessage(AMessageId, "~", myKbd); 
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
  myBot.sendMessage(AMessageId, "~", myKbd); 
}

void DoGet(String cUrl){
  HTTPClient http;     
  http.begin(cUrl);
  http.GET();
  http.end();  
} 

int GetCurrUser(int AMessId){
  int _id = -1;
  for (int i = 0; i < cUsersCount; i++){
    if (Users[i].id == AMessId){
       _id = i; // в идеале возвращать элемент 
       break;
      }
    }  
    return _id;
}

void loop() {
  TBMessage msg;
  
  if (myBot.getNewMessage(msg)) {
    if (msg.messageType == CTBotMessageText) {          
      
      int UserIndex = GetCurrUser(msg.sender.id);
      Log("UserIndex " + String(UserIndex));
    
      if (UserIndex == -1) {
        if (msg.text.equalsIgnoreCase("/start")) {
          myBot.sendMessage(msg.sender.id, "Привет " + msg.sender.firstName + "; Ваш id: " + msg.sender.id);
          Log("New User");
        }
      }
      else {
        if (msg.text.equalsIgnoreCase("/start")) {
          myBot.sendMessage(msg.sender.id, "Привет " + msg.sender.firstName);
          Log("Old User");
          StartKB(msg.sender.id);          
        }
    
        if (msg.text.equalsIgnoreCase("Назад")){
          if (!Users[UserIndex].DeviceBrightWhiteState){
            StartKB(msg.sender.id);             
          } 
          else {
            DetailKB(msg.sender.id);   
          }
        }
    
        if (msg.text.equalsIgnoreCase("/MyUI")){
          myBot.sendMessage(msg.sender.id, "UId: " + String(msg.sender.id)); 
          myBot.sendMessage(msg.sender.id, "UName: " + msg.sender.firstName);                  
        }
    
        if (msg.text.equalsIgnoreCase("Лампа 1")){
          Users[UserIndex].CurrDevice = 0;
          DetailKB(msg.sender.id);
        }
    
        if (msg.text.equalsIgnoreCase("Лампа 2")) {
          Users[UserIndex].CurrDevice = 1;
          DetailKB(msg.sender.id);   
        }
            
        if (msg.text.equalsIgnoreCase("Включить")) {
          if(Users[UserIndex].CurrDevice == 0){
          //  DoGet(IPLamp_0 + "/light/0?turn=on"); 
            DoGet(IPRel_0 + "/relay/0?turn=on");      
           }
           if(Users[UserIndex].CurrDevice == 1){
             DoGet(IPLamp_1 + "/light/0?turn=on"); 
           }             
         }
            
        if (msg.text.equalsIgnoreCase("Выключить")) {
           if(Users[UserIndex].CurrDevice == 0){
           //  DoGet(IPLamp_0 + "/light/0?turn=off");
             DoGet(IPRel_0 + "/relay/0?turn=off");  
           }
           if(Users[UserIndex].CurrDevice == 1){
             DoGet(IPLamp_1 + "/light/0?turn=off"); 
           }             
         }    
     
         if (msg.text.equalsIgnoreCase("Прочее")) {
           if(Users[UserIndex].CurrDevice > -1){ 
             Users[UserIndex].DeviceBrightWhiteState = true;            
             BrightWhiteKB(msg.sender.id);
           }            
         }

         if (Users[UserIndex].DeviceBrightWhiteState){
           int len = msg.text.length();
           String value;        
           String prefix = msg.text.substring(0,1);
           if (len = 6){ 
             value = msg.text.substring(2,4);
             if (value == "10") { 
               value += "0";
             }
           }    
           if (Users[UserIndex].CurrDevice == 0){
             if (prefix == "w"){
               DoGet(IPLamp_0 + "/light/0?white=" + value);   
             }
             if (prefix == "b"){
               DoGet(IPLamp_0 + "/light/0?turn=on&brightness=" + value); 
             }
           }
           if (Users[UserIndex].CurrDevice == 1){
             if (prefix == "w"){
               DoGet(IPLamp_1 + "/light/0?white=" + value); 
             }
             if (prefix == "b"){
               DoGet(IPLamp_1 + "/light/0?turn=on&brightness=" + value);  
                // myBot.sendMessage(msg.sender.id, "value " + String(value));
                // myBot.sendMessage(msg.sender.id, "CurrDevice " + String(Users[UserIndex].CurrDevice)); 
             }
           }
 
         } // if (Users[UserIndex].DeviceBrightWhiteState)   
      }  //UserIndex > 0 (else)
   }  //  if (msg.messageType == CTBotMessageText)
 } // if (myBot.getNewMessage(msg))
} // void loop
