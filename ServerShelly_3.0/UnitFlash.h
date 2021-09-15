#include <EEPROM.h>


int addrState = 10;
int addrName = 20;
int addrIndex = 15;
int addrStateNewDevice = 5;

char eeprom_read_state_new_device()
{
  
  EEPROM.begin(40); 
  char NewDevice = EEPROM.read(addrStateNewDevice);
  EEPROM.end();
  return NewDevice;
}


void eeprom_write_state_new_device(char i)
{
  EEPROM.begin(40);
  EEPROM.write(addrStateNewDevice, i); //записываем
  
  EEPROM.commit(); //сохраняем в епром
  EEPROM.end(); //сохраняем в епром
}

String eeprom_read_name()
{
  EEPROM.begin(40);
  String Name = "null";
  char str[20];
  int strLengh = EEPROM.read(addrName);
  for (int i=0; i<(strLengh+1); i++)
  {
    str[i] = EEPROM.read(addrName+1+i);
  }
  Name = String(str);
  EEPROM.end();
  return Name;
}

bool eeprom_read_state()
{
  EEPROM.begin(40); 
  bool state = false;
  int st = EEPROM.read(addrState);
  if (st == 1) state = true;
  EEPROM.end();
  Serial.println("name from eeprom: " + state);
  return state;
}

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
  Serial.println("name to eeprom: ");
  for (int i = 0; i<str.length() + 1; i++)
  {
    Serial.print(cstr[i]);
  }
  EEPROM.commit(); //сохраняем в епром
  EEPROM.end(); //сохраняем в епром
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

void eeprom_write_index(int index)
{
  EEPROM.begin(40);
  EEPROM.write(addrIndex, index);
  Serial.println("EEPROM write position: " + index);
  EEPROM.commit(); //сохраняем в епром
  EEPROM.end(); //сохраняем в епром
}

int eeprom_read_index()
{
  EEPROM.begin(40); 
  int index = EEPROM.read(addrIndex);
  Serial.println("EEPROM read position: " + index);
  EEPROM.end();
  return index;
}
