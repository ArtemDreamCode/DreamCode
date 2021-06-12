#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <Streaming.h>
#include <Vector.h>
#include <ArduinoJson.h>


int const lengh_max = 10;
typedef Vector<IPAddress> Elements;
IPAddress storage_array[lengh_max];
Elements vector(storage_array);

String ssid = "ESPap";
String password = "123456789";
int ACountButtons = 0;
  String l;
//массив хранения ip адресов и их состояний

String ip_base[lengh_max]; //текущие ip дареса устройств
String state_base[lengh_max];  //состояния устройств
String mac_base[lengh_max];  // мак-адреса устройств

ESP8266WebServer server(80);

HTTPClient http;
WiFiClient client;
DynamicJsonBuffer jsonBuffer; 

String SendHTML();

String get(String Arequest){
  http.begin(client, Arequest); 
//  log(Arequest);
  int httpCode = http.GET();  
  String response = http.getString(); 
  
 // log(httpCode);  
  http.end();
  //log("Free heap: " + String(ESP.getFreeHeap()));
  return response;
}

void log(String AMessage){
  Serial.println(AMessage); 
}

bool StartAPMode() {
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid, password);
  
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  return true;
}

void handle_OnConnect() 
{ 
  server.send(200, "text/html", SendHTML()); 
}

void handle_NotFound()
{
  server.send(404, "text/plain", "Not found");
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  log("");
  log("Configuring access point...");
 
  StartAPMode();
  
  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);
  
  server.begin();
  log("HTTP server started");
  vector.setStorage(storage_array);
}


bool IsElemInCollect(IPAddress AElem){
 bool f = false;
   for (IPAddress element : vector)
    {
       f = (element.toString() == AElem.toString() );   
       if (f) {break;} 
    } 
  return f;
}

void DeleteElemInCollect(IPAddress AElem){
   for(int i = vector.size() - 1; i>=0; i--){
     if (vector[i]== AElem){
       vector.remove(i);
       break;   
     }
   }
}

void AddElemInCollect(IPAddress AElem){
  vector.push_back(AElem);
}

void ControllIpToCollect(IPAddress IPAddr){ 
  bool IsValidDevice = true;
  String r = "http://" + IPAddr.toString() + "/state";
   Serial << "r: " << r << endl;
  String req = get(r);
  Serial.println(req);
  JsonObject& root = jsonBuffer.parseObject(req);
  /*if (!root.success()) {
      IsValidDevice = false;
      Serial.println("Parsing failed!");
      return;
    }*/

   String RespFromShelly = "";
   RespFromShelly = root["class"].as<char*>();
   
   Serial.println("Response:");
    Serial.println(RespFromShelly);

  if ((IsElemInCollect(IPAddr)) && (!IsValidDevice)){ // если отвалилось Shelly
      DeleteElemInCollect(IPAddr);
  }
  if ((!IsElemInCollect(IPAddr)) && (RespFromShelly == "Shelly")) {  // если Shelly добавилось новое
      AddElemInCollect(IPAddr);
      }

}

void loop() {
  server.handleClient();  
  delay(1000);
  client_status();
     Serial << "====================================================" << endl;
       Serial << "AddIpToCollect: vector.size " << vector.size() << endl;
        Serial << "====================================================" << endl;
           Serial << "===================================================="<< endl;
  delay(1000);
}

String checkMacFromIp(String ip)
{
  int i=0;
  return mac_base[i];
}

String checkIpFromMac(String mac)
{
  int i=0;
  return ip_base[i];
}

String checkStateFromMac(String mac)
{
  int i=0;
  return state_base[i];
}

void client_status() {
    unsigned char number_client;
    struct station_info *stat_info;
    struct ip_addr *IPaddress;

    String mac;
    String ip;
    char cur_mac[7];
    
    IPAddress address;
    int i=0;
    number_client= wifi_softap_get_station_num();
    stat_info = wifi_softap_get_station_info();
 //   Serial.print(" Total connected_client are = ");
 //   Serial.println(number_client);
    while (stat_info != NULL) {
        ipv4_addr *IPaddress = &stat_info->ip;
        address = IPaddress->addr;
        ControllIpToCollect(address);
                
      /*  Serial.print("client= ");
        Serial.print(i + 1);
        Serial.print(" ip adress is = ");
        Serial.print(address);
        Serial.print(" with mac adress is = ");
        Serial.print(stat_info->bssid[0],HEX);
        Serial.print(stat_info->bssid[1],HEX);
        Serial.print(stat_info->bssid[2],HEX);
        Serial.print(stat_info->bssid[3],HEX);
        Serial.print(stat_info->bssid[4],HEX);
        Serial.print(stat_info->bssid[5],HEX);*/
        
        cur_mac[0] = stat_info->bssid[0];
        cur_mac[1] = stat_info->bssid[1];
        cur_mac[2] = stat_info->bssid[2];
        cur_mac[3] = stat_info->bssid[3];
        cur_mac[4] = stat_info->bssid[4];
        cur_mac[5] = stat_info->bssid[5];
        cur_mac[6] = '\0';

        ip = address.toString();
        mac = cur_mac;
        
       // Serial.print(mac);
        stat_info = STAILQ_NEXT(stat_info, next);
        i++;
      //  Serial.println();
    }
    wifi_softap_free_station_info();
}
String SendHTML()
{
  int Index = 0;
  String s;
  String ptr =  "<!DOCTYPE html https://html5css.ru/css/css3_buttons.php>\n";
  ptr +=  "<html>\n";
  ptr +=  "<head>\n";
  ptr +=  "<style>\n";
  ptr +=  ".button {\n";
  ptr +=  "  background-color: #4CAF50;\n";
  ptr +=  "  border: none;\n";
  ptr +=  "  color: white;\n";
  ptr +=  "  padding: 15px 32px;\n";
  ptr +=  "  text-align: center;\n";
  ptr +=  "  text-decoration: none;\n";
  ptr +=  "  display: inline-block;\n";
  ptr +=  "  font-size: 16px;\n";
  ptr +=  "  margin: 4px 2px;\n";
  ptr +=  "  cursor: pointer;\n";
  ptr +=  "}\n";
  ptr +=  ".buttonScroll {border-radius: 4px;}\n";
  ptr +=  ".buttonWidth {width: 250px;}\n";
  ptr +=  "</style>\n";
  ptr +=  "</head>\n";
  ptr +=  "<body>\n";
  
  ptr +=  "<h2>Home controll</h2>\n";
   for (IPAddress element : vector)
  {
    Index++; 
      
    ptr +=  "<input type='button' class=\"button buttonScroll buttonWidth\" value='On' onClick=\"change_on_"+String(Index)+"()\" id='id_on'>\n";
    ptr +=  "<input type='button' class=\"button buttonScroll buttonWidth\" value='Off' onClick=\"change_off_"+String(Index)+"()\" id='id_off'>\n";
    
    ptr +=  "</body>\n";
    ptr +=  "<script>\n";
    ptr +=  "function change_on_"+String(Index)+"(){\n";
    ptr +=  "     var x = new XMLHttpRequest();\n";
    ptr +=  "    x.open(\"GET\", \"http://" + element.toString() +"/relay?turn=on\", true);\n";
    ptr +=  "    x.onload = function (){\n";
    ptr +=  "      alert( x.responseText);\n";
    ptr +=  "    }\n";
    ptr +=  "    x.send(null);\n";
    ptr +=  "     return false;\n";
    ptr +=  "  }\n";
    ptr +=  "\n";  
    ptr +=  "function change_off_"+String(Index)+"(){\n";
    ptr +=  "     var x = new XMLHttpRequest();\n";
    ptr +=  "    x.open(\"GET\", \"http://" + element.toString() +"/relay?turn=off\", true);\n";
    ptr +=  "    x.onload = function (){\n";
    ptr +=  "      alert( x.responseText);\n";
    ptr +=  "    }\n";
    ptr +=  "    x.send(null);\n";
    ptr +=  "     return false;\n";
    ptr +=  "  }\n";
    ptr +=  "</script>\n";
      }
  ptr +=  "</html>\n";

  return ptr;
}
