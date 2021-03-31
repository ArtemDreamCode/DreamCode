//приемник
//nRF24L01 - ESP8266 12
//1 GRND
//2 VCC
//3 CE   - GPIO4   5 d2  
//4 CSN  - GPIO15  d8
//5 SCK  - GPIO14  d5
//6 MOSI - GPIO13  d7
//7 MISO - GPIO12  d6
//8 IRQ  - NC
 
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
RF24 radio(4, 15); 

byte address[][6] = {"1Node","2Node","3Node","4Node","5Node","6Node"};  //возможные номера труб

int lamp_pin = 0; //d3 пин управления реле
int ti=0;

void setup(){     
  SPI.setHwCs(true);
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);   
    Serial.begin(9600);  
    radio.begin();
    radio.setAutoAck(0);
    radio.setRetries(0,15);
    radio.enableAckPayload();
    radio.setPayloadSize(32);
    radio.setChannel(0x60);
    radio.setPALevel      (RF24_PA_MAX);
    radio.setDataRate     (RF24_1MBPS);
    
    radio.powerUp(); //начать работу
    delay(10);
    radio.startListening();
    delay(10);
    radio.openReadingPipe (1, address[0]);
//  radio.stopListening   (); 
    Serial.println("Radio configured!");
    delay(100);
    pinMode(lamp_pin, OUTPUT);
    digitalWrite(lamp_pin, LOW);
}

void loop(){
  byte pipeNo, gotByte;     
  while(radio.available()){
    radio.read(&gotByte, sizeof(gotByte));
    Serial.print("Recieved: ");
    Serial.println(gotByte);
    if (gotByte==100)
    {
      digitalWrite(lamp_pin, HIGH);
    }
    else if (gotByte==200)
    {
      digitalWrite(lamp_pin, LOW);
    }
  }
}
