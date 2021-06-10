#include <SoftwareSerial.h>                                                              
#include "RoomInit.h"

//uno
//const uint8_t  pinRX   = 2;  //синий                                                                
//const uint8_t  pinTX   = 3;   //желтый

//node
const uint8_t  pinRX   = 13; // D7 синий                                                                   
const uint8_t  pinTX   = 15; // D8 желтый


//node
//const uint8_t  pinRX   = 3; //                                                                    
//const uint8_t  pinTX   = 1; // 

int currentRoom = -1;
String buf = "h0";

SoftwareSerial softSerial(pinRX,pinTX);    

Room room[ROOMS_COUNT];

void log_init(){
  Serial.begin(9600); 
}

void uart_init(){
  softSerial.begin(9600); 
}

void log(String AValue){
  Serial.println(AValue);
}  

// Сначала объявим кое-что, заранее:
String inStr = ""; // Это будет приемник информации от Nextion
bool serialReadFlag = false; // А это гаец-регулировщик.

void checkCommand(String ins) 
{//собственно парсим принятое сообщение
    // У нас информация от Nextion состоит из двух частей,
    String first = ins.substring(0,2);// первая буква и цифра - тип и номер обьекта
    String str_second = ins.substring(3,1); //следущие символы - значение
    String last = ins.substring(4);
    int second = str_second[1];
    Serial.print(ins);
 
    

/*
 //    Serial.print(ins);
    if (first[0] == 'r')
    { //кнопки выбора комнаты
      Serial.print("\nRoom: ");
      switch (first[1])
      {
        case '0': currentRoom = ROOM_HALLWAY;   break;
        case '1': currentRoom = ROOM_KITCHEN;   break;        
        case '2': currentRoom = ROOM_BATHROOM;  break;
        case '3': currentRoom = ROOM_TOILET;    break;
        case '4': currentRoom = ROOM_SMROOM;    break;
        case '5': currentRoom = ROOM_BIGROOM;   break;      
        default: currentRoom = -1;      break;
      }
      if( currentRoom>=0 && currentRoom < ROOMS_COUNT)
        Serial.print(room[currentRoom].name);
        
      else
        Serial.print("Unknown room");
    }
    if (first[0] == 'h')
    {//slider
       if (first[1] == '0')
       {//slider brightness;
          if (buf==first){
            Serial.print("\n\t\tbrightness ");
            buf = "h1";
          }
          room[currentRoom].brightness = second; 
          Serial.print(room[currentRoom].brightness);
          Serial.print(" ");
       }
       if (first[1] == '1')
       {//slider warmth;
          if (buf==first){
            Serial.print("\n\t\twarmth ");
            buf = "h0";
          }
          room[currentRoom].warmth = second;
          Serial.print(room[currentRoom].warmth);
          Serial.print(" ");
       }
       
    }
    if (first[0] == 'l')
    { //кнопки вкл/выкл света
      Serial.print("\n\tligtht: ");
      if( currentRoom>=0 && currentRoom<ROOMS_COUNT){ //если комната существует
        switch (first[1])
        {
          case '0': room[currentRoom].light=0;break;
          case '1': room[currentRoom].light=1; break;
        }
        if (room[currentRoom].light==0) Serial.print("Off");
        if (room[currentRoom].light==1) Serial.print("On"); 
      }
      
    }
    if (first[0] == 'i')
    {
      if (first[1] == 'b')
      { //кнопка "назад"
        Serial.print("\nback");
        currentRoom = -1;
        Serial.print(currentRoom = -1);
      }    
      if (first[1] == 'n')
      { //кнопка "информация"
        Serial.print("\ninfo");
      }
    }

    if ((first[0] == 'b') && (first[1] == 't'))
    {
      if (second == '0')
      { 
         Serial.print("\nall lamps");
      }    
      if (second == '1')
      { 
        Serial.print("\n1 lamp");
      }
      if (second == '2')
      { 
        Serial.print("\n2 lamp");
      }    
      
    }
*/
}  


// Функция принимает три параметра, первый - указатель на String с данными
// второй - наименование элемента на странице, третий - номер страницы как String,
// причем по умолчанию это нулевая страница
void printNextion(const char* data, String top, String page = "0")
{
    String spage = "page"+page+"."+top+".txt=\"";
    softSerial.print(spage);
    softSerial.print(data);
    softSerial.print("\"");
    softSerial.write(0xFF);
    softSerial.write(0xFF);
    softSerial.write(0xFF);

    /*
    
    // forNext.c_str() - возвращает указатель на стринг, точнее на массив char из наших букв
    // t2 - название элемента на странице
    // 4 - номер страницы
    printNextion(forNext.c_str(),"t2","4");
    */
}             
void setup(){                   
  uart_init(); 
  log_init();
  rooms_init();
}                

void loop()
{ 
      
// Располагаем в loop следующее
  if (softSerial.available())
  {
      uint8_t inn = softSerial.read(); // читаем один байт
      if(serialReadFlag)
      { // Если установлен флаг приема - действуем
          if(inn == 59)
          {     // ASCII : ";" // Находим конец передачи ";"
              if(inStr.length() > 0) 
              { // Проверяем длину сообщения и отправляем в "переработку"
                  checkCommand(inStr); // В этой функции будем парсить сообщение
              }
              serialReadFlag = false; // Сбрасываем флаг приема
          }
          else 
          { // А это нормальный прием
              inStr += (char)inn; // Создаем String от Nextion
          }
      }
      else 
      { // А здесь отлавливается начало передачи от Nextion
          if(inn == 35) 
          { // ASCII : "#"
              serialReadFlag = true; // После # начинаем чтение при следующем заходе
              inStr = ""; // Но до этого очистим стринг приема
          }
      }
    }
  }   
