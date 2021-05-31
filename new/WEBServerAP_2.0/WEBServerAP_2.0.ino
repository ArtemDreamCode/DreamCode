#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

////change from 31.05.2021 14:05 work pc////

String ssid = "ESPap";
String password = "123456789";

bool LED1status = false;


//массив хранения уникальных ключей и их состояний
int const lengh_max = 10;

String id_base[lengh_max] = {"shally"}; //идентификаторы устройств
String state_base[lengh_max] = {"Off"};  //состояния устройств

String cur_id = "shally";

ESP8266WebServer server(80);

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
   connected to this access point to see it.
*/
void handleRoot() {
  server.send(200, "text/html", "<h1>You are connected</h1>");
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
  //log("AP IP address: " + myIP);
  
  return true;
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  log("");
  log("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
 
  StartAPMode();
  server.on("/", handle_OnConnect);
  server.on("/led1on", handle_led1on);
  server.on("/led1off", handle_led1off);
  server.onNotFound(handle_NotFound);


  server.on("/new", handle_new);
  server.on("/hard", handle_hard);
  server.on("/hardstate", handle_hardState);
    
  server.begin();
  log("HTTP server started");
}

void loop() {
  server.handleClient();  
}

void handle_OnConnect() 
{ 
  if(LED1status)
   log("Lamp Status: ON");
  else
   log("Lamp Status: OFF");


  log("");
  server.send(200, "text/html", SendHTML(LED1status)); 
}

void handle_led1on() 
{
  LED1status = true;
  log("LED1 Status: ON ");
  server.send(200, "text/html", SendHTML(LED1status)); 
}

void handle_led1off() 
{
  LED1status = false;
  log("LED1 Status: OFF ");
  server.send(200, "text/html", SendHTML(LED1status)); 
}

String generate_id()
{
  String resp = "shally";
  char cur_id[6];
  for (int i=0; i<6; i++)
  {
    cur_id[i] = random(65, 90);
    resp = cur_id;
  }
  return resp;
}

void handle_new()
{
  for (int i=0; i<lengh_max;i++)
  {
    if (id_base[i] == "shally") //ищем свободную ячейку массива
    {
      String abcd = generate_id; //генерим айди и сохраняем в базу
      id_base[i] = abcd;
      state_base[i] = "Off"; //меняем флаг соотв этому айди
      server.send(200, "text/html", id_base[i]); //отправляем айди его устройству
      break;
    }
  }
}

String check_flag(String id)
{
  for (int i=0; i<lengh_max; i++)
  {
    if (id_base[i] == id)
    {
      return state_base[i];
    }
  }
}

void handle_hard()
{ //принимаем гет-запрос от устройства с параметром айди
  //отвечаем на него его статусом из памяти
  String message = "";
  if (server.args() == 1) //если аргумент один
  {
    if (server.argName(0) == "id") //и этот аргумент "id"
    {
      String id_param = server.arg(0); //смотрим значение этого аргумента - от кого пришел запрос
      server.send(200, "text/html", check_flag(id_param)); //отправляем устройству его статус
    }
  }
}

void handle_hardState()
{
  String message = "";

  if (server.arg("id")=="")
  { //параметр не найден
    message = "id Arg not found";
  }
  else
  { //параметр найден
    message = "id = ";
    message += server.arg("id");
  }
  if (LED1status) {
    log("LED1 Status: ON");
    server.send(200, "text/html", "On");
  }
  else{
    log("LED1 Status: Off");
    server.send(200, "text/html", "Off"); 
    } 
}


void handle_NotFound()
{
  server.send(404, "text/plain", "Not found");
}

String SendHTML(bool led1stat)
{
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta http-equiv=\"Refresh\" content=\"7;URL=http://192.168.4.1/\" name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
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
  ptr +="<h3>Using Access Point(AP) Mode</h3>\n";
  
  if(led1stat)
    ptr +="<p>LED1 Status: ON</p><a class=\"button button-off\" href=\"/led1off\">OFF</a>\n";
  else
    ptr +="<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/led1on\">ON</a>\n";
/*
  if(led2stat)
    ptr +="<p>LED2 Status: ON</p><a class=\"button button-off\" href=\"/led2off\">OFF</a>\n";
  else
    ptr +="<p>LED2 Status: OFF</p><a class=\"button button-on\" href=\"/led2on\">ON</a>\n";
*/
  ptr +="</body>\n";
  
  ptr +="</html>\n";
  return ptr;
}
