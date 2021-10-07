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

String str_input = "";


String Plagin_GUID = "DFDDSDFDF454DFdfd";

const int DEVICES_MAX_COUNT = 20;
Device dictionary[DEVICES_MAX_COUNT];
int count_dictionary = 0;

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
  String response = "input data from serv: ";
  response += str_input;
  response += "\n";
  response += "{";
  response += "\"dictionary\":["; 
  for (int i=0; i<count_dictionary; i++)
  {
    response += "{";   
    response+= "\"name\": \""+String(dictionary[i].name)+"\"";
    response+= ",\"ident\": \""+dictionary[i].ident+"\"";
    response+= ",\"state\": \""+String(dictionary[i].state)+"\"";
    response+= "}"; 
    if ((i+1)<count_dictionary)
      response+= ",";  
  }
  response+= "]}"; 
  server.send(200, "text/html", response);
}

String readSerialInput()
{
  String str_data = "";
  while (Serial.available() > 0) {         // ПОКА есть что то на вход    
    str_data += (char)Serial.read();        // забиваем строку принятыми данными
    delay(2);                              // ЗАДЕРЖКА. Без неё работает некорректно!
  }
  return str_data;
}

void readAndParse()
{
  if (Serial.available() > 0)
  {  //если есть доступные данные
            
      StaticJsonDocument<capacity> doc;
      String jsstring = Serial.readStringUntil('\0');
      str_input = jsstring;
        DeserializationError error = deserializeJson(doc, jsstring);  
        if (error) {
        return;
      }
      JsonArray arr = doc["dictionary"].as<JsonArray>();
    
      count_dictionary = arr.size(); //кол-во элементов массива
     
      
      if (count_dictionary>DEVICES_MAX_COUNT)
       count_dictionary = DEVICES_MAX_COUNT;
      
       int i = 0;
      for (JsonObject repo : arr) {
        dictionary[i].name = repo["name"];
        i++;
      }

// responce
 String response = "{";
  response += "\"names\":["; 
  for (int i=0; i<count_dictionary; i++){
    response += "{";   
    response+= "\"name\": \""+String(dictionary[i].name)+"\"";
    response+= "}"; 
    if ((i+1)<count_dictionary)
      response+= ",";  
  }
  response+= "]}";
 
//    Serial.println("responce: " + response);

 
      
      for (int i=0; i<count_dictionary; i++)
      {
        Serial.println(dictionary[i].name);
     //   Serial.println(dictionary[i].ip);
       // Serial.println(dictionary[i].ident);
     //   Serial.println(dictionary[i].state);
       // Serial.println("______________");
      }
    }
}


void loop(){
  readAndParse();
  server.handleClient();
  delay(200);
}
