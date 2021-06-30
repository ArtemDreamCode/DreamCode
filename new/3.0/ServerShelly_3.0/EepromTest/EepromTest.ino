//#include <avr/eeprom.h>
#include <EEPROM.h>

String DeviceName = "NewDevice";
bool DeviceState = false;
 
int a = 0;
int value;

int addrState = 10;
int addrName = 20;

String incomingString = "";

void eeprom_write_name(String str)
{
  char *cstr = new char[str.length() + 1];
  strcpy(cstr, str.c_str()); //переводим string в char[]
  EEPROM.begin(40);
  EEPROM.write(addrName, str.length()+1); //записываем сначала длину строки-имени
  for (int i=0; i<=(str.length() + 1); i++)
  {
    EEPROM.write(addrName+1+i, cstr[i]);  //записываем последовательно каждый символ имени
  }
  EEPROM.commit(); //сохраняем в епром
  EEPROM.end(); //сохраняем в епром
}

String eeprom_read_name()
{
  EEPROM.begin(40);
  String Name = "null";
  char str[16];
  int strLengh = EEPROM.read(addrName);
  for (int i=0; i<(strLengh+1); i++)
  {
    str[i] = EEPROM.read(addrName+1+i);
  }
  Name = String(str);
  EEPROM.end();
  return Name;
}


void eeprom_write_state(bool state)
{
  EEPROM.begin(40);
  if (state)
    EEPROM.write(addrState, 1); //записываем флаг состояния
  else
    EEPROM.write(addrState, 0); //записываем сначала длину строки-имени

  EEPROM.commit(); //сохраняем в епром
  EEPROM.end();
}

bool eeprom_read_state()
{
  EEPROM.begin(40); 
  bool state = false;
  int st = EEPROM.read(addrState);
  if (st == 1) state = true;
  EEPROM.end();
  return state;
}

void setup()
{
  Serial.begin(115200);
  DeviceName = eeprom_read_name();
  Serial.println();
  Serial.print("My name is: ");
  Serial.println(DeviceName);
  
  int DSt = eeprom_read_state();
  Serial.print("My state is: ");
  if (DSt == 1) DeviceState = true;
  else DeviceState = false;
  Serial.println(DeviceState);
  
  
  

  //EEPROM.begin(512);
  
  //value = EEPROM.read(10);
  //value = eeprom_read_byte(10);
  //incomingString = Serial.readString();
  
  //Serial.print("I read from EEPROM: ");
  //Serial.println(value, DEC);
  
  //Serial.println(incomingString);
  //EEPROM.end();
}

 
void loop()
{

  if (Serial.available())
  {     //если есть доступные данные считываем строку
    DeviceName = Serial.readString(); //получили строку и сохранили имя
    Serial.print("I received: ");
    Serial.println(DeviceName);
      //EEPROM.begin(512);
      eeprom_write_name(DeviceName);
      Serial.print("I remem: ");
      Serial.print(DeviceName);
      //EEPROM.end();
    }        
  delay(500);
}
