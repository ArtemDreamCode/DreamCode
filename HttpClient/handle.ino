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
    server.on(("/areyouhere"), HTTP_GET, handle_ping);
} 

void handle_ping(){ //запрос о состоянии от клиента
   PingIn = 0;
   server.send(200, "text/html", "iamhere");
}

void handle_GetState(){ //запрос о состоянии от клиента
   server.send(200, "text/html", GetAllStateData());
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
