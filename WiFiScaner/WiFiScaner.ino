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

const int DEVICES_MAX_COUNT = 20;
Device dictionary_new[DEVICES_MAX_COUNT];
int count_dictionary_new = 0;


ESP8266WebServer server(80);

WiFiClient client; 
HTTPClient http; // for depricated Get request 

String ShellySSID = "ShellyAP1";
String ShellyPASS = "11001100";

//String HomeSSID = "LASSARD";
//String HomePASS = "PvE9zyZe";

//String MobSSID = "Samsung";
//String MobPASS = "11001100";

String ServerSSID = "MainAP";
String ServerPASS = "12345678";

//=======================================================================
//                     SETUP
//=======================================================================
void setup() {
  Serial.begin(115200);
  Serial.println(""); //Remove garbage

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  delay(100);

  Serial.println("WiFi Netwoek Scan Started");

  wifi_begin(ServerSSID, ServerPASS);
  restServerRouting();  
  server.onNotFound(handleNotFound);
  server.begin();

}

//=======================================================================
//                        LOOP
//=======================================================================
void loop() {
   Serial.println("START : wifi_scan");
  server.handleClient(); 
  // Wait a bit before starting New scanning again
  delay(5000);
}
//=======================================================================
