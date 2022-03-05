String GetAllStateData()
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
//   response+= ",\"index\": \""+String(DeviceIndex)+"\""; 
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
//   response+= ",\"gw\": \""+WiFi.gatewayIP().toString()+"\"";
//   response+= ",\"nm\": \""+WiFi.subnetMask().toString()+"\"";
//   response+= ",\"signalStrengh\": \""+String(WiFi.RSSI())+"\"";
//   response+= ",\"chipId\": \""+String(ESP.getChipId())+"\"";
//   response+= ",\"flashChipId\": \""+String(ESP.getFlashChipId())+"\"";
//   response+= ",\"flashChipSize\": \""+String(ESP.getFlashChipSize())+"\"";
//   response+= ",\"flashChipRealSize\": \""+String(ESP.getFlashChipRealSize())+"\"";
//   response+= ",\"freeHeap\": \""+String(ESP.getFreeHeap())+"\"";
   response+="}";
   
   return response;
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
  
  String resp = post(ServerAddr, GetAllStateData()); 
  
  MacAdr = WiFi.macAddress(); //8C:AA:B5:7B:13:73
}

bool set_AP_mode()
{
  eeprom_write_state_wifi_mode(0);
  ShellySSID = GenerateAPName();
  boolean result = WiFi.softAP(ShellySSID, ShellyPASS);
  return result;
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

String post(String addr, String Arequest){
    http.setTimeout(200);
    http.begin(client, addr+WiFi.localIP().toString()); 
    Serial.println(Arequest);
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST(Arequest);
     
    String response = http.getString();  
    Serial.println(response);   
    //http.end();
    return response;
}
