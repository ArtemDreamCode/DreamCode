#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266HTTPClient.h>
#include <PubSubClient.h>

ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;
HTTPClient http;

const char* mqtt_server = "farmer.cloudmqtt.com";
WiFiClient espClient;
PubSubClient client(espClient);

String dev_id = "MegaESP";
String wifi_ssid = "gofk_phone";
String wifi_pass = "12345678";
String use_mqtt = "1";
String mqtt_brocker = "farmer.cloudmqtt.com";
String mqtt_port = "10212";
String mqtt_user = "gofk";
String mqtt_pass = "12345678";
String http_on_request = "http://yandex.ru/on11";
String http_off_request = "http://yandex.ru/off22";
String boot_state = "0";
String use_http = "1";
byte server_mode; // 1 = client, 2 = AP

#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
const int LOG_LEVEL = 2;
const String APSSID = "Gofk_AP";
const String APPassword = "12345678";
int value = 0;
unsigned long lastMsg = 0;
String outTopic = dev_id + "/out";
String stateTopic = dev_id + "/state";
String inTopic = dev_id + "/in";
String cmdTopic = dev_id + "/cmd";
      
const char* wifi_ssid_c = "gofk_phone";
const char* wifi_pass_c = "12345678";

void MQTTcallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("MQTT получение <<< топик: ");
  Serial.print(topic);
  Serial.print(", сообщение: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  if ((char)payload[0] == '1') {
  } else {
    digitalWrite(BUILTIN_LED, HIGH);
  }
}

void MQTTreconnect() {
  while (!client.connected()) {
    Serial.print("MQTT, подключение к брокеру...");
    if (client.connect(dev_id.c_str(),"gofk","ca3huvnm4")) {
      Serial.println("успешно");
      String s = "MQTT отправка >>> топик: " + outTopic + ", сообщение: hello world";
      Serial.println(s);
      client.publish(outTopic.c_str(), "hello world");
      s = "MQTT отправка >>> топик: " + stateTopic + ", сообщение: State";
      Serial.println(s);
      client.publish(stateTopic.c_str(), "State");
      client.subscribe(inTopic.c_str());
      client.subscribe(cmdTopic.c_str());
    } else {
      Serial.print("ошибка, rc=");
      Serial.print(client.state());
      Serial.println(", повтор через 5 секунд");
      delay(5000);
    }
  }
}

void saveParam(String param, String value) {
  String filePath = "/conf/" + param + ".txt";
  String msg; 
  File fw = SPIFFS.open(filePath, "w");
  if (!fw) {
    msg = filePath + ": ошибка открытия файла";
  } else {
    fw.print(value);
    msg = filePath + ": изменения сохранены <-- " + value; 
    fw.close();
  }
  Serial.println(msg);
}

String getParam(String param) {
  String filePath = "/conf/" + param + ".txt";
  String msg; 
  File fr = SPIFFS.open(filePath, "r");
  if (!fr) {
    msg = filePath + ": ошибка открытия файла";
    Serial.println(msg);
    saveParam(param, "");
    msg = filePath + ": создан новый файл без содержимого";
    Serial.println(msg);
    return "";
  } else {
    String value = "";
    for(int i=0;i<fr.size();i++) { value += (char)fr.read(); }
    fr.close();
    msg = filePath + ": данные прочитаны --> " + value;  
    Serial.println(msg);
    return value;
  }
}

void WIFIinit() {
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  byte tries = 15;
  Serial.println("Подключаемся к сети " + wifi_ssid + " с паролем " + wifi_pass);
  WiFi.begin(wifi_ssid.c_str(), wifi_pass.c_str());
  //WiFi.begin(wifi_ssid_c, wifi_pass_c);
  while (--tries && WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Создаем точку доступа (AP)");
    server_mode = 2;
    StartAPMode();
  } else {
    Serial.println("Подключение к сети WiFi установлено");
    server_mode = 1;
  }
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("SoftAP IP: ");
  Serial.println(WiFi.softAPIP());
  Serial.print("MAC address: ");
  Serial.println(WiFi.softAPmacAddress());
  createWebServer();
  server.begin();
  Serial.println("HTTP-сервер запущен"); 
}

bool StartAPMode() {
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0));
  WiFi.softAP(APSSID.c_str(), APPassword.c_str());
  return true;
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println("Startup");
  if(SPIFFS.begin()) {
    Serial.println("Инициализация файловой системы (SPIFFS)...успешно");
  } else {
    Serial.println("Инициализация файловой системы (SPIFFS)...ошибка");
  }
  String flist = FileList("/");
  Serial.println("Список файлов: ");
  Serial.println(flist);
  dev_id = getParam("dev_id");
  wifi_ssid = getParam("wifi_ssid");
  wifi_pass = getParam("wifi_pass");
  use_mqtt = getParam("use_mqtt");
  mqtt_brocker = getParam("mqtt_brocker");
  mqtt_port = getParam("mqtt_port");
  mqtt_user = getParam("mqtt_user");
  mqtt_pass = getParam("mqtt_pass");
  http_on_request = getParam("http_on_request");
  http_off_request = getParam("http_off_request");
  boot_state = getParam("boot_state");
  use_http = getParam("use_http");
  client.setServer(mqtt_server, 10212);
  client.setCallback(MQTTcallback);
  WIFIinit();
  MDNS.begin(dev_id);
  httpUpdater.setup(&server);
  MDNS.addService("http", "tcp", 80);
}

bool FileRead(String path) {
  if (path.endsWith("/")) path += "index.htm";
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) {
    if (SPIFFS.exists(pathWithGz))
      path += ".gz";
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}

String FileList(String path) {
  String output="";
  Dir dir = SPIFFS.openDir(path);
  while (dir.next()) {
    File entry = dir.openFile("r");
    output += "- " + String(entry.name()).substring(1) + "\n\r";
    entry.close();
  }
  return output; 
}

String getContentType(String filename) {
  if (server.hasArg("download")) return "application/octet-stream";
  else if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".json")) return "application/json";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

String scanWiFi() {
  Serial.println("Сканируем WiFi-сети");
  String networks = "";
  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("Сканирование завершено");
  if (n == 0) {
    Serial.println("Сети не обнаружены");
    networks = "Сети не обнаружены";
  } else {
    Serial.print("Обнаружено сетей: ");
    Serial.println(n);    
    for (int i = 0; i < n; ++i) {
      networks += String(i + 1) + ": " + String(WiFi.SSID(i)) + " (" + String(WiFi.RSSI(i)) + ")" + ((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*") + "<br>";
      Serial.print(i + 1);      
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      delay(10);
    }
  }
  return networks;
}

void createWebServer() {
  /* server.on("/", []() {
    
  } */
  server.on("/", HTTP_GET, []() {
    if (!FileRead("/index.html")) server.send(404, "text/plain", "FileNotFound");
  });
  server.on("/a.js", HTTP_GET, []() {
    if (!FileRead("/a.js")) server.send(404, "text/plain", "FileNotFound");
  });
  server.on("/favicon.ico", HTTP_GET, []() {
    if (!FileRead("/favicon.ico")) server.send(404, "text/plain", "FileNotFound");
  });
  server.on("/style.css", HTTP_GET, []() {
    if (!FileRead("/style.css")) server.send(404, "text/plain", "FileNotFound");
  });
  server.on("/index.html", HTTP_GET, []() {
    if (!FileRead("/index.html")) server.send(404, "text/plain", "FileNotFound");
  });
  server.on("/get", HTTP_GET, []() {
    String param = server.arg("param");
    server.send(200, "text/plain", getParam(param));
  });
  server.on("/set", HTTP_GET, []() {
    String param = server.arg("param");
    String value = server.arg("value");
    saveParam(param,value);
    server.send(200, "text/plain", getParam(param));
  });
  server.on("/state", HTTP_GET, []() {
    int r = random(100);
    server.send(200, "text/plain", String(r));
  });
  server.on("/set", []() {
    server.send(200, "text/plain", "Set page, webtype = 1");
  }); 
  server.on("/info", HTTP_GET, []() {    
    String param = server.arg("param");
    Serial.println(">>> запрос к /info, param=" + param);
    String n;
    if(param == "local_ip"){
      n = String(WiFi.localIP()[0]) + "." + String(WiFi.localIP()[1]) + "." + String(WiFi.localIP()[2]) + "." + String(WiFi.localIP()[3]);
    } else if (param == "softap_ip") {
      n = String(WiFi.softAPIP()[0]) + "." + String(WiFi.softAPIP()[1]) + "." + String(WiFi.softAPIP()[2]) + "." + String(WiFi.softAPIP()[3]);
    } else if (param == "mac") {
      n =  WiFi.softAPmacAddress();
    } else if (param == "dev_id") {
      n = dev_id;
    } else if (param == "wifi_networks") {
      n = scanWiFi();
    }    
    Serial.println("<<< " + n);
    server.send(200, "text/plain", n);
  });
  server.on("/restart", []() {
    Serial.println(">>> запрос к /restart");
    Serial.println("Перезагружаем модуль");
    ESP.restart();
  }); 
}

void SendHTTPRequest(bool off){
  String req = (off ? http_off_request : http_on_request);
  Serial.println("=> Отправка HTTP-запроса [" + req + "]");
  http.begin(req);
  http.addHeader("Content-Type", "text/plain");
  int httpCode = http.GET();
  String payload = http.getString();
  Serial.println(httpCode);
  Serial.println(payload);
  http.end();
}

void loop() {
  server.handleClient();
  MDNS.update();
  if (!client.connected()) {
    MQTTreconnect();
  }
  client.loop();
  unsigned long now = millis();
  if (now - lastMsg > 10000) {
    lastMsg = now;
    ++value;
    snprintf (msg, MSG_BUFFER_SIZE, "hello world #%ld", value);
    String s = "MQTT отправка >>> топик: " + outTopic + ", сообщение: " + msg;
    Serial.println(s);
    client.publish(outTopic.c_str(), msg);
    delay(1000);
  }
  }
