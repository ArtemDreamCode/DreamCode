#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <ESP8266HTTPClient.h>

ESP8266WebServer server(80);

//WiFiClient client; 
//HTTPClient http; // for depricated Get request 

String ShellySSID = "ShellyAP108";
String ShellyPASS = "11001100";


void handle_reconnect() // подключение к точке доступа
{
  String state_buf;
  Serial.println("handle_reconnect");
  if ((server.args() == 2) && (server.argName(0) == "ssid") && (server.argName(1) == "pass"))
  {
    String ssid = server.arg(0);
    String pass = server.arg(1);
    Serial.println("ssid: " + ssid);
    Serial.println("pass: " + pass);
//    eeprom_write_state(RealCheck);
    WiFi.softAPdisconnect();
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);  
    while (WiFi.status() != WL_CONNECTED) {
      Serial.println("connecting to wifi");
      delay(450);
    }
    String ip = WiFi.localIP().toString().c_str(); 
    Serial.println("my ip in current network: " + ip);
  
    delay(100);
  }
  server.send(200, "text/html", "reconnect start");
}

void handle_connect()
{
  Serial.println("connect");
  server.send(200, "text/html", "connect");
}

void setup()
{
  Serial.begin(115200);
  Serial.println();

  Serial.print("Setting soft-AP ... ");
           //  "Настройка программной точки доступа ... "
  boolean result = WiFi.softAP(ShellySSID, ShellyPASS);
  String ip = WiFi.localIP().toString().c_str(); 
    Serial.println("my ip: " + ip); 
  if(result == true)
  {
    Serial.println("Ready");
               //  "Готово"
  }
  else
  {
    Serial.println("Failed!");
               //  "Настроить точку доступа не удалось"
  }

  server.on(("/"), HTTP_GET, handle_connect);
  server.on(("/reconnect"), HTTP_GET, handle_reconnect);
  server.begin();
  
}

void loop()
{
  server.handleClient(); 
//  Serial.printf("Stations connected = %d\n", WiFi.softAPgetStationNum());
            //  "Количество подключенных станций = "
  delay(200);
}
