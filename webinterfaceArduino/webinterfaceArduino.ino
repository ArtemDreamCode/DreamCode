#include <ArduinoJson.h>

struct Device
{
   const char* name;
   const char* ip;
   String ident;
   const char* state;
};

String str_input = "";


String Plagin_GUID = "DFDDSDFDF454DFdfd";

const int DEVICES_MAX_COUNT = 20;

Device dictionary_new[DEVICES_MAX_COUNT]; //массив структур новых устройств

//Device dictionary_old[DEVICES_MAX_COUNT]; //массив структур старых устройств

int count_dictionary_new = 0;
int count_dictionary_old = 0;


const int capacity = 10000;
String ip;

String ipToIdent(String ip)
{
  int index = ip.lastIndexOf('.');
  String ident = "ident_" + ip.substring(index+1, ip.length());
  return ident;
}

void setup(){
  Serial.begin(115200);
//  wifi_begin();
//  restServerRouting();  
//  server.onNotFound(handleNotFound);
//  server.begin();

}



/*String readSerialInput()
{
  String str_data = "";
  while (Serial.available() > 0) {         // ПОКА есть что то на вход    
    str_data += (char)Serial.read();        // забиваем строку принятыми данными
    delay(2);                              // ЗАДЕРЖКА. Без неё работает некорректно!
  }
  return str_data;
}*/

void readAndParse()
{
  if (Serial.available() > 0)
  {  //если есть доступные данные
            
      String jsstring = Serial.readStringUntil('\0'); //читаем строку из серийника
      str_input = jsstring; //сохраняем в глобальную перем
      StaticJsonDocument<capacity> doc; //обьявляем json document
      
      DeserializationError error = deserializeJson(doc, jsstring);  //десериализуем, проверяем на ошибки
      if (error) { //если ошибка, прерываем
        return;
      }
      JsonArray dictionary_new = doc["dictionary_new"].as<JsonArray>(); //массив обьектов json новые устройства
      JsonArray dictionary_old = doc["dictionary_old"].as<JsonArray>(); //массив обьектов json старые устройства 
    
      count_dictionary_new = dictionary_new.size(); //кол-во элементов массива
      count_dictionary_old = dictionary_old.size(); //кол-во элементов массива
           
      if (count_dictionary_new>DEVICES_MAX_COUNT) //если элементов в jsone больше, чем массив структур в памяти
       count_dictionary_new = DEVICES_MAX_COUNT;  //уменьшаем кол-во элементов, обрезая оставшиеся :))
      if (count_dictionary_old>DEVICES_MAX_COUNT)
       count_dictionary_old = DEVICES_MAX_COUNT;

       // идем по массиву обьектов и заполняем соответств структуры
       int i = 0;
      for (JsonObject repo : arr) {
        dictionary_new[i].name = repo["name"];
     //   dictionary_new[i].ip = repo["ip"];
   //     dictionary_new[i].ident = ipToIdent(String(dictionary_new[i].ip));
 //       dictionary_new[i].state = repo["state"];
        i++;
      }
   }
}


void loop(){
  readAndParse();
  //server.handleClient();
  delay(200);
//  Serial.println(  "{\"name\":\"kitchen\",\"ip\":\"192.168.0.22\",\"ident\":\"ident_22\",\"state\":\"on\"}");
}
