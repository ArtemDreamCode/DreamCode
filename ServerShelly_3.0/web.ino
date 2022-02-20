String JsonToStr()
{
  String dev_guid = GenerateAPName();
  String st;
   String response = "{"; 
   if (RealCheck){
     st = "on";
   }  
   else{
     st = "off";
   }
   response+= "\"device_guid\": \"" + dev_guid + "\"";
   response+= ",\"index\": \""+String(DeviceIndex)+"\""; 
   response+= ",\"state\": \""+st+"\"";
   response+= ",\"ip\": \""+WiFi.localIP().toString()+"\"";
   response+= ",\"class\": \""+ClassDevice+"\""; 
   response+= ",\"name\": \""+DeviceFrendlyName+"\""; 
   char newdev = eeprom_read_state_new_device();
   //Serial.println(newdev);
   if (newdev == 1)
      response+= ",\"isnewdevice\": \"old\"";
   else 
      response+= ",\"isnewdevice\": \"new\"";
   response+= ",\"mac\": \""+WiFi.macAddress()+"\"";   
   response+= ",\"gw\": \""+WiFi.gatewayIP().toString()+"\"";
   response+= ",\"nm\": \""+WiFi.subnetMask().toString()+"\"";
   response+= ",\"signalStrengh\": \""+String(WiFi.RSSI())+"\"";
   response+= ",\"chipId\": \""+String(ESP.getChipId())+"\"";
   response+= ",\"flashChipId\": \""+String(ESP.getFlashChipId())+"\"";
   response+= ",\"flashChipSize\": \""+String(ESP.getFlashChipSize())+"\"";
   response+= ",\"flashChipRealSize\": \""+String(ESP.getFlashChipRealSize())+"\"";
   response+= ",\"freeHeap\": \""+String(ESP.getFreeHeap())+"\"";
   response+="}";
   
   return response;
}

void handle_GetState(){ //запрос о состоянии от клиента
   server.send(200, "text/html", JsonToStr());
}
void handle_ChangeState()
{
  String state_buf;
  if ((server.args() == 1) && (server.argName(0) == "turn"))
  //если аргумент один и он "turn"
  {
    state_buf = server.arg(0);
    if (state_buf == "on")
    {
      RealCheck = true;
      digitalWrite(4, HIGH); //выключаем
      //включить лампочку
    }
    else if(state_buf == "off")
    {
      RealCheck = false;
      digitalWrite(4, LOW); //выключаем
      //выключить лампочку
    }
    eeprom_write_state(RealCheck);
  }
  
  String response = "{"; 
  String st;
  if (RealCheck){
    st = "on";
   }
   else{
     st = "off";
   }
   response+= "\"state\": \""+st+"\"";
   response+="}";
   server.send(200, "text/html", response);
}

void handle_Reset()
{  
   server.send(200, "text/html", "ok");
   DeviceFrendlyName = GenerateAPName();
   eeprom_write_state_new_device(0);   
   eeprom_write_name(DeviceFrendlyName);
   String response = "{"; 
   response+= "\"name\": \""+DeviceFrendlyName+"\"";
    response+="}";
}

void handle_FullReset()
{
   server.send(200, "text/html", "ap mode");
   delay(200);
   DeviceFrendlyName = GenerateAPName();
   eeprom_write_state_new_device(0);   
   eeprom_write_name(DeviceFrendlyName);
   String response = "{"; 
   response+= "\"name\": \""+DeviceFrendlyName+"\"";
    response+="}";
    WiFi.disconnect();
   if (set_AP_mode())
     Serial.println("Точка доступа создана!");
   else Serial.println("Ошибка создания точки доступа!");
   
}

void handle_ChangeFrendlyName(){
  if ((server.args() == 1) && (server.argName(0) == "name")) {
    String nm;
    nm = server.arg(0);
    DeviceFrendlyName = nm;
    eeprom_write_name(DeviceFrendlyName);
    if (!is_new_device())
    {
      eeprom_write_state_new_device(1);
    }
    String response = "{"; 
    response+= "\"name\": \""+DeviceFrendlyName+"\"";
    response+="}";
    server.send(200, "text/html", response);
  }
  
  if ((server.args() == 1) && (server.argName(0) == "index")) {
    int index = 0;
    index = server.arg(0).toInt();
    DeviceIndex = index;
    eeprom_write_index(DeviceIndex);
    String response = "{"; 
    response+= "\"index\": \""+String(DeviceIndex)+"\"";
    response+="}";
  
    server.send(200, "text/html", response);
  }
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

void restServerRouting() {
  
    server.on("/", HTTP_GET, []() {
        server.send(200, F("text/html"),
            F("Welcome to the REST Shelly Server"));
    });
    server.on(("/relay"), HTTP_GET, handle_ChangeState); // get from user
    server.on(("/reset"), HTTP_GET, handle_Reset); // get from user
    server.on(("/state"), HTTP_GET, handle_GetState); // get from AP
    server.on(("/set"), HTTP_GET, handle_ChangeFrendlyName); // change device name
    server.on(("/reconnect"), HTTP_GET, handle_reconnect);
    server.on(("/fullreset"), HTTP_GET, handle_FullReset);
} 
void wifi_begin(String ssid, String pass){
  WiFi.mode(WIFI_STA);
  WiFi.hostname(GenerateAPName());
  WiFi.begin(ssid, pass);  
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(2, HIGH);  
    Serial.println("connecting to wifi: " + ssid);
    delay(450);
    DoCheckButtonState();
  }
  Serial.println(WiFi.localIP());
  digitalWrite(2, LOW); 
  Serial.println("connected to " + ssid); 
  
  String getOut = "";
  
  if (RealCheck)
    getOut = "http://192.168.1.2/switch?turn=on";
  else getOut = "http://192.168.1.2/switch?turn=off";
  String resp = get(getOut); 
  
  MacAdr = WiFi.macAddress(); //8C:AA:B5:7B:13:73
}

bool set_AP_mode()
{
  eeprom_write_state_wifi_mode(0);
  ShellySSID = GenerateAPName();
  boolean result = WiFi.softAP(ShellySSID, ShellyPASS);
  return result;
}

void handle_reconnect() // подключение к точке доступа
{
  Serial.println("handle_reconnect ok");
  server.send(200, "text/html", "reconnect start");
  //if (!is_station) //только если в режиме точки доступа
  //{
    String state_buf;
    Serial.println("handle_reconnect");
    if ((server.args() == 1) && (server.argName(0) == "ssid"))
    {
      String ssid = server.arg(0);
      Serial.println("ssid: " + eeprom_read_server_ssid());
      Serial.println("pass: " + ServerPASS);
  //    eeprom_write_state(RealCheck);
      WiFi.softAPdisconnect();
      WiFi.disconnect();
      WiFi.mode(WIFI_STA);
      WiFi.begin(ssid, ServerPASS);  
      while (WiFi.status() != WL_CONNECTED) {
        Serial.println("connecting to wifi");
        delay(450);
      }
      eeprom_write_state_wifi_mode(1);
      eeprom_write_server_ssid(ssid);
      String ip = WiFi.localIP().toString().c_str(); 
      Serial.println("my ip in current network: " + ip);
    
      delay(100);
    }
  //}
}

String get(String Arequest){
    http.setTimeout(200);
    http.begin(client, Arequest); 
    Serial.println(Arequest);
    int httpCode = http.GET();
   // log(String(httpCode)); 
     
    String response = http.getString();  
    Serial.println(response);   
    http.end();
    return response;
}
