const int wakeUpPin = 2 ; // d2
int _click_on_state = 0; //состояние кнопки


void /*ICACHE_RAM_ATTR*/ InterruptWakeUp()
{
    detachInterrupt( digitalPinToInterrupt (wakeUpPin) );
    _click_on_state = 1;
}


void setup() {
  Serial.begin(9600); //открываем порт для связи с ПК
  pinMode(wakeUpPin, INPUT);
 
  // put your setup code here, to run once:
   attachInterrupt(digitalPinToInterrupt(wakeUpPin)
                  , InterruptWakeUp
                  , FALLING);
}

void loop() {
  if (_click_on_state==1)
    {
        Serial.println("button");
        delay(10);  
      }    
         
                 
      _click_on_state=0;
      delay(100);
      attachInterrupt(digitalPinToInterrupt(wakeUpPin)
                  , InterruptWakeUp
                  , FALLING);
}
