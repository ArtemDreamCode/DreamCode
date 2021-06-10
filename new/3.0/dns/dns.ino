#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

ESP8266WebServer server;

char* ssid = "TP-LINK_120";
char* password = "12ses3434sd333";

MDNSResponder mdns;


void setup()
{
 WiFi.begin(ssid,password);
 Serial.begin(115200);
 while(WiFi.status()!=WL_CONNECTED)
 {
 Serial.print(".");
 delay(500);
 }
 Serial.println("");
 Serial.print("IP Address: ");
 Serial.println(WiFi.localIP());

if (mdns.begin("esp8266-01", WiFi.localIP()))
Serial.println("MDNS responder started");

server.on("/",[](){server.send(200,"text/plain","Hello World!");});
server.begin();

MDNS.addService("http", "tcp", 80);}


void loop()
{
server.handleClient();
}
