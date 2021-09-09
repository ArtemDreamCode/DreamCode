#include <EEPROM.h>
/*String GetFlashName(){
  String & str = "1";
  eeprom_gets(1, str);
  return str;  
}*/

bool GetFlashState(){
  char * str;
  String r;
  EEPROM.get(0, str);
  r = str;
  Serial.println("EEPROM.get(0, str) " + r);
  return (r == "1");
}

/*void SetFlashName(String AValue){
  eeprom_puts(1, AValue);   
}*/

void SetFlashState(bool AState){
  if (AState) {
     EEPROM.write(0, 1);
  }
  else{
    EEPROM.write(0, 0); 
    } 
    EEPROM.commit();
}


/*void eeprom_puts(int Offset, String str) {
   const char *buf = str.c_str();
   EEPROM.put(Offset, strlen(buf));
   for (int i=Offset; i<16; i++) {
       EEPROM.put(Offset+1+i, buf[i]);
   }
   EEPROM.commit();
}

String readEEPROM() {
    char buf[16];
    uint16_t size = EEPROM.get(AddrName);
    for (int i=0; i<size; i++) {
      char buf[16] = EEPROM.get(AddrName+1+i);
    }
    return String(buf);
}*/
