#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

////change from 31.05.2021 14:05 work pc////

String ssid = "ESPap";
String password = "123456789";
int ACountButtons = 0;

//массив хранения ip адресов и их состояний
int const lengh_max = 10;

String ip_base[lengh_max]; //текущие ip дареса устройств
String state_base[lengh_max];  //состояния устройств
String mac_base[lengh_max];  // мак-адреса устройств

ESP8266WebServer server(80);


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


void handle_new()
{ 
  String ip_param;
  if ((server.args() == 1) && (server.argName(0) == "ip"))//если аргумент 1 и он "ip"
  {
    ip_param = server.arg(0); //смотрим значение этого аргумента - от кого пришел запрос
    for (int i=0; i<lengh_max;i++)
    {
      if (ip_base[i] == "") //ищем свободную ячейку массива
      {
        ip_base[i] = ip_param; //сохраняем в базу айпи сервера
        state_base[i] = "Off"; //меняем флаг соотв этому айпи
        server.send(200, "text/html", "ok"); //подтверждаем получение айпиадреса
        break;
      }
    }
  }
  for (int i=0; i<lengh_max;i++)//выводим список всех айпиадресов и их флагов
  {
    Serial.print(ip_base[i] + " " + state_base[i] + "\n");
  }

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


  server.on("/new", handle_new);
  
  server.begin();
  log("HTTP server started");
}

void loop() {
  server.handleClient();  
  delay(1000);
  client_status();
  delay(1000);
}

void client_status() {
    unsigned char number_client;
    struct station_info *stat_info;
    struct ip_addr *IPaddress;
    
    String cur_mac;
    String cur_ip;
    char cur_id[7];
    
    IPAddress address;
    int i=1;
    number_client= wifi_softap_get_station_num();
    stat_info = wifi_softap_get_station_info();
    Serial.print(" Total connected_client are = ");
    Serial.println(number_client);
    while (stat_info != NULL) {
        
        ipv4_addr *IPaddress = &stat_info->ip;
        address = IPaddress->addr;
        
        cur_ip = address.toString();
        id_base[i-1] = cur_ip;
        
        Serial.print("client= ");
        Serial.print(i);
        Serial.print(" ip adress is = ");
        Serial.print((address));
        Serial.print(" with mac adress is = ");
        Serial.print(stat_info->bssid[0],HEX);
        cur_id[0] = stat_info->bssid[0]
        Serial.print(stat_info->bssid[1],HEX);
        Serial.print(stat_info->bssid[2],HEX);
        Serial.print(stat_info->bssid[3],HEX);
        Serial.print(stat_info->bssid[4],HEX);
        Serial.print(stat_info->bssid[5],HEX);
        stat_info = STAILQ_NEXT(stat_info, next);
        i++;
        Serial.println();
    }
    wifi_softap_free_station_info();
}

String SendHTML()
{
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +=  "<head>\n";
  ptr +=  "</head>\n";
  
  ptr +=  "<body>\n";
  ptr +=  "  <input type='button' value='On' onClick=\"change_on()\" id='id_on'>\n";
  ptr +=  "  <input type='button' value='Off' onClick=\"change_off()\" id='id_off'>\n";
  ptr +=  "</body>\n";
  
  ptr +=  "<script>\n";
  
  ptr +=  "function change_on(){\n";
  ptr +=  "   document.getElementById('id_on').onclick=function(){\n";
  ptr +=  "     var x = new XMLHttpRequest();\n";
  ptr +=  "    x.open(\"GET\", \"http://" + ip_base[0] +"/relay?turn=on\", true);\n";
  ptr +=  "    x.onload = function (){\n";
  ptr +=  "      alert( x.responseText);\n";
  ptr +=  "    }\n";
  ptr +=  "    x.send(null);\n";
  ptr +=  "   }\n";
  ptr +=  "     return false;\n";
  ptr +=  "  }\n";
  ptr +=  "\n";  
  ptr +=  "function change_off(){\n";
  ptr +=  "   document.getElementById('id_off').onclick=function(){\n";
  ptr +=  "     var x = new XMLHttpRequest();\n";
  ptr +=  "    x.open(\"GET\", \"http://" + ip_base[0] +"/relay?turn=off\", true);\n";
  ptr +=  "    x.onload = function (){\n";
  ptr +=  "      alert( x.responseText);\n";
  ptr +=  "    }\n";
  ptr +=  "    x.send(null);\n";
  ptr +=  "   }\n";
  ptr +=  "     return false;\n";
  ptr +=  "  }\n";
  ptr +=  "</script>\n";
  
  ptr +=  "</script>\n";
  
  ptr +=  "</html>\n";
  return ptr;
}
