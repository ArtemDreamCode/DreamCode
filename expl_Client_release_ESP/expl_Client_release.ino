#include "service.h"
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

void setup() {
  initlog();
  wifi_begin();
  pinMode(wakeUpPin, CHANGE);//
  pinMode(ledPin, OUTPUT);//
  attachInterrupt(digitalPinToInterrupt(wakeUpPin)
                  , InterruptWakeUp
                  , FALLING);
}

void loop() { 
  if ( _click_on_state == 1){
    _click_on();
    _click_on_state=0;
  }

unsigned long currentMillis = millis(); 
  if (currentMillis - previousMillis >= period) { 
    previousMillis = currentMillis; 
    ChangeLight();
  }
}
