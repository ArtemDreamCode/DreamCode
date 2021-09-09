#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <ESP8266HTTPClient.h>

struct Device
{
   const char* name;
   const char* ip;
   String ident;
   const char* state;
};

String Plagin_GUID = "DFDDSDFDF454DFdfd";

const int DEVICES_MAX_COUNT = 20;
Device dictionary_new[DEVICES_MAX_COUNT];
int count_dictionary_new = 0;

ESP8266WebServer server(80);

WiFiClient client; 
HTTPClient http; // for depricated Get request 

//const char* ssid = "Keenetic-9462";
//const char* pass = "LCMN8S6X";


const char* ssid = "iPhonexc5";
const char* pass = "12345qAz";

//const char* ssid = "143";
//const char* pass = "12345678";

const int capacity = 10000;
String ip;

String ipToIdent(String ip)
{
  int index = ip.lastIndexOf('.');
  String ident = "ident_" + ip.substring(index+1, ip.length());
  return ident;
}

void wifi_begin(){
  bool f = false;
  WiFi.mode(WIFI_STA);
 // WiFi.begin(ssid); 
  WiFi.begin(ssid, pass);  
  while (WiFi.status() != WL_CONNECTED) {
 //   Serial.println("connecting to wifi");
    delay(450);
    f = true;
  }
  if (f) {
  ip = WiFi.localIP().toString().c_str(); 
  Serial.print("ip: " + ip);
  }
//  Serial.println("connected"); 
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

void setup(){
  Serial.begin(115200);
  wifi_begin();
  restServerRouting();  
  server.onNotFound(handleNotFound);
  server.begin();
}


void restServerRouting() {
  
    server.on(("/"), HTTP_GET, handle_GetInfo);
    server.on(("/getInfo"), HTTP_GET, handle_GetState); // get from AP
    server.on(("/turn"), HTTP_GET, handle_turn); // get from AP
}

void handle_GetInfo() 
{
  String response = "{\"getInfo\":\""+ip+"\"}";
  
  Serial.print(response);
  delay(200);
  readAndParse();
  handle_GetState();
}

void handle_turn(){
  String ident_buf;
  String state_buf;
  String message;
  if ((server.args() == 2) && (server.argName(0) == "ident") && (server.argName(1) == "state"))
  //если аргумента два, один из них "ident", второй "state"
  {
    ident_buf = server.arg(0);
    state_buf = server.arg(1);
    //////формируем сообщение серверу по serialport
    int index = ident_buf.indexOf('_');
    message =  "{\"request\":\"http://";
    message += "192.168.0.";
    message += ident_buf.substring(index+1, ident_buf.length());
    message += "/relay?turn=";
    message += state_buf;
    message += "\"}";
    Serial.print(message);
    server.send(200, "text/html", "ok");
  }
}

void handle_GetState(){ //запрос о состоянии от клиента
  String response = "{";
  response += "\"dictionary_new\":["; 
  for (int i=0; i<count_dictionary_new; i++)
  {
    response += "{";   
    response+= "\"name\": \""+String(dictionary_new[i].name)+"\"";
    response+= ",\"ident\": \""+dictionary_new[i].ident+"\"";
    response+= ",\"state\": \""+String(dictionary_new[i].state)+"\"";
    response+= "}"; 
    if ((i+1)<count_dictionary_new)
      response+= ",";  
  }
  response+= "],";
  response+="\"dictionary_old\":[]}"; 
  server.send(200, "text/html", response);
}

void readAndParse()
{
  if (Serial.available() > 0)
  {  //если есть доступные данные
            
      StaticJsonDocument<capacity> doc;
      String jsstring = Serial.readString();
    
        DeserializationError error = deserializeJson(doc, jsstring);  
        if (error) {
        Serial.println("err parse: " + jsstring);
//        Serial.println(error.f_str());
        return;
      }
      
      JsonArray arr = doc["dictionary_new"].as<JsonArray>();
    
      count_dictionary_new = arr.size(); //кол-во элементов массива
     
        //Serial.print("ok arr.count = " + count_dictionary_new);
      
      if (count_dictionary_new>DEVICES_MAX_COUNT)
       count_dictionary_new = DEVICES_MAX_COUNT;
      

       // Walk the JsonArray efficiently
       int i = 0;
      for (JsonObject repo : arr) {
        dictionary_new[i].name = repo["name"];
     //   dictionary_new[i].ip = repo["ip"];
   //     dictionary_new[i].ident = ipToIdent(String(dictionary_new[i].ip));
 //       dictionary_new[i].state = repo["state"];
        i++;
      }

// responce
 String response = "{";
  response += "\"names\":["; 
  for (int i=0; i<count_dictionary_new; i++){
    response += "{";   
    response+= "\"name\": \""+String(dictionary_new[i].name)+"\"";
    response+= "}"; 
    if ((i+1)<count_dictionary_new)
      response+= ",";  
  }
  response+= "]}";
 
//    Serial.println("responce: " + response);

 
      
      for (int i=0; i<count_dictionary_new; i++)
      {
        Serial.println(dictionary_new[i].name);
     //   Serial.println(dictionary_new[i].ip);
       // Serial.println(dictionary_new[i].ident);
     //   Serial.println(dictionary_new[i].state);
       // Serial.println("______________");
      }
    }
}


void loop(){
//  readAndParse();
  server.handleClient();
  delay(200);
}
