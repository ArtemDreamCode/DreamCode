bool f_process = false;
bool wifi_begin(String ssid, String pass){
  bool f = false;
  WiFi.disconnect();  //ESP has tendency to store old SSID and PASSword and tries to connect
  WiFi.mode(WIFI_STA);
 // WiFi.begin(ssid); 
  WiFi.begin(ssid, pass);  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("connecting to wifi");
    delay(450);
    f = true;
  }
  if (f) {
    String ip = WiFi.localIP().toString().c_str(); 
    Serial.println("my ip in current network: " + ip); 
    return true;
  }
  return false; 
}

void handleNotFound() {
  server.send(404, "text/plain", "not found");
}

void restServerRouting() 
{  
    server.on("/scan", handle_scan);
  //  server.send(200, "text/html", "scan done");
}


void handle_scan() 
{
  if (f_process) return;
  f_process = true; 
  /////////// сканируем точки доступа, если находим esp
  // подключаемся к ней, отправляем данные основной точки доступа
  // и говорим переподключиться в режиме станции
  //wifi_scan(0);
  
  if (wifi_scan())
    wifi_begin(ServerSSID, ServerPASS); // connect to rasp network
  f_process = false; 
}

bool wifi_scan()
{
  Serial.println("START : wifi_scan");
   // WiFi.scanNetworks will return the number of networks found
  int k = 0;
  bool isShellyFound = false;
  int n = WiFi.scanNetworks();
   
  Serial.println("Scan done");
  
  if (n == 0)
    Serial.println("No Networks Found");
  else
  {
    Serial.print(n);
    Serial.println(" Networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);  //Sr. No
      Serial.print(": ");
      Serial.print(WiFi.SSID(i)); //SSID
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i)); //Signal Strength
      Serial.print(") MAC:");
      Serial.print(WiFi.BSSIDstr(i));
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" Unsecured":" Secured");
      if (WiFi.SSID(i).indexOf(ShellySSID) > -1)
      {
        isShellyFound = true;
        if (wifi_begin(WiFi.SSID(i), ShellyPASS))
        {
            String getOut = "http://192.168.4.1/reconnect?ssid="+ServerSSID;
            Serial.println(get(getOut));
            
        }
      }
      
      delay(10);
    }
  }
  return isShellyFound;
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
