
void DoCheckButtonState()
{
  String getOut = "";
  int bt = digitalRead(5);
  if (bt != bt_state)
  { //если есть изменение состояния кнопки
     if (!RealCheck)
     {
       RealCheck = true;
       digitalWrite(4, HIGH); //включаем
       getOut = ServerAddr + "switch?turn=on";
     }
     else if (RealCheck)
     {
       RealCheck = false;
       digitalWrite(4, LOW); //выключаем
       getOut = ServerAddr + "switch?turn=off";
       
     }
     eeprom_write_state(RealCheck);
     ////todo отправить гет-запрос на сервер о изменения состояния
     String resp = get(getOut);
  }
  else
  {
    if (RealCheck) //realcheck уже начитался из памяти 
       digitalWrite(4, HIGH); //включаем
    else      
       digitalWrite(4, LOW); //выключаем
  }
 
  bt_state = bt; //готовы снова ловить выключатель  
}
