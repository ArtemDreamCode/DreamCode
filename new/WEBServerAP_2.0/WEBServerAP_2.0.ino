#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

////change from 31.05.2021 14:05 work pc////

String ssid = "ESPap";
String password = "123456789";
int ACountButtons = 0;

//массив хранения уникальных ключей и их состояний
int const lengh_max = 10;

String id_base[lengh_max]; //идентификаторы устройств
String state_base[lengh_max];  //состояния устройств

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
  server.on("/butclick", handle_butclick);
  server.onNotFound(handle_NotFound);


  server.on("/new", handle_new);
  server.on("/state", handle_hardState);
  server.on("/changestate", handle_hardChangeState);
  server.on("/Data", handle_DataController);
  
  server.begin();
  log("HTTP server started");
}

void loop() {
  server.handleClient();  
}

void handle_OnConnect() 
{ 
  server.send(200, "text/html", SendHTML()); 
}

void handle_butclick() //нажатие на кнопку в форме
{
  Serial.println("handle_butclick");
  String id_buf;
  String state_buf;
  if (server.args() == 2) //если аргумента два
  {
    if (server.argName(0) == "id") //первый из них "id"
    {
      if (server.argName(1) == "state") //второй из них "state"
      {
        id_buf = server.arg(0);
        state_buf = server.arg(1); 
        change_state(id_buf, state_buf);
        for (int i=0; i<lengh_max;i++)
        {
          Serial.print(id_base[i] + " " + state_base[i] + "\n");
        }
        server.send(200, "text/html", SendHTML()); 
      }
    }
  }
}

void handle_DataController(){
 Serial.println("handle_butclick");
  String id_buf;
  String state_buf;
  if (server.args() == 1) //если аргумента два
  {
    if (server.argName(0) == "Count")
    {

        ACountButtons = server.arg(0).toInt();
        log(ACountButtons);
        server.send(200, "text/html", SendHTML()); 
   
    }
  }  
}

String generate_id()
{
  String resp;
  char cur_id[7];
  for (int i=0; i<6; i++)
  {
    cur_id[i] = random(65, 90);
  }
  cur_id[6] = '\0';
  resp = cur_id;
  return resp;
}

void handle_new()
{ 
  for (int i=0; i<lengh_max;i++)
  {
    if (id_base[i] == "") //ищем свободную ячейку массива
    {
      id_base[i] = generate_id(); //генерим айди и сохраняем в базу
      state_base[i] = "Off"; //меняем флаг соотв этому айди
      server.send(200, "text/html", id_base[i]); //отправляем айди его устройству
      break;
    }
  }
  
  for (int i=0; i<lengh_max;i++)
  {
    Serial.print(id_base[i] + " " + state_base[i] + "\n");
  }

}

String check_flag(String id)
{
  for (int i=0; i<lengh_max; i++)
  {
    if (id_base[i] == id)
    {
      return state_base[i];
      break;
    }
  }
//  return "null";
}


//String
void change_state(String id, String state)
{
  for (int i=0; i<lengh_max; i++)
  {
    if (id_base[i] == id)
    {
      state_base[i] = state;
      //return state_base[i];
    }
  }
}

void handle_hardState()
{ //принимаем гет-запрос от устройства с параметром айди
  //отвечаем на него его статусом из памяти
  if (server.args() == 1) //если аргумент один
  {
    if (server.argName(0) == "id") //и этот аргумент "id"
    {
      String id_param = server.arg(0); //смотрим значение этого аргумента - от кого пришел запрос
      Serial.print("  " + id_param + "\n");
      server.send(200, "text/html", check_flag(id_param)); //отправляем устройству его статус
    }
  }
}

void handle_hardChangeState()
{
  String id_buf;
  String state_buf;
  if (server.args() == 2) //если аргумента два
  {
    if (server.argName(0) == "id") //первый из них "id"
      id_buf = server.arg(0);
    if (server.argName(1) == "state") //второй из них "state"
    {
      state_buf = server.arg(1); 
      change_state(id_buf, state_buf);
      server.send(200, "text/html", check_flag(id_buf));
    }
  }
}

void handle_NotFound()
{
  server.send(404, "text/plain", "Not found");
}

String SendHTML()
{
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta http-equiv=\"Refresh\" content=\"5;URL=http://192.168.4.1/\" name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
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

  for (int i=0; i< ACountButtons; i++)
  {

    ptr +="<p>LED1 Status: ON</p><a class=\"button button-off\"href=http://192.168.4.100/Bton"; <button id="btnId" type="button">Hide/Show</button>
    ptr +="?id="+id_base[i]+"&state=Off\">OFF</a>\n";
  
  //   ptr +="<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/butclick";
  //   ptr +="?id="+id_base[i]+"&state=On\">ON</a>\n";
 
  }
  ptr +="</body>\n";
  
  ptr +="</html>\n";
  return ptr;
}
