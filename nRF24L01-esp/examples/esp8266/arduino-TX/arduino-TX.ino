/*   Данный скетч делает следующее: передатчик (TX) отправляет массив
 *   данных, который генерируется согласно показаниям с кнопки и с 
 *   двух потенциомтеров. Приёмник (RX) получает массив, и записывает
 *   данные на реле, сервомашинку и генерирует ШИМ сигнал на транзистор.
    by AlexGyver 2016
*/

//передатчик
//nRF24L01 - arduino UNO
//1 GRND
//2 VCC
//3 CE   - 9
//4 CSN  - 10
//5 SCK  - 13
//6 MOSI - 11
//7 MISO - 12
//8 IRQ  - NC

#include <SPI.h>          // библиотека для работы с шиной SPI
#include "nRF24L01.h"     // библиотека радиомодуля
#include "RF24.h"         // ещё библиотека радиомодуля
//#include <avr/sleep.h>
#include <LowPower.h>

const int wakeUpPin = 2 ; // d2
int _click_on_state = 0; //состояние кнопки
int led_state = 0;  //состояние лампы

RF24 radio(9, 10); // "создать" модуль на пинах 9 и 10 Для Уно
//RF24 radio(9,53); // для Меги

byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; //возможные номера труб

byte counter1 = 100 ;
byte counter2 = 200 ;

void /*ICACHE_RAM_ATTR*/ InterruptWakeUp()
{
    detachInterrupt( digitalPinToInterrupt (wakeUpPin) );
     radio.powerUp(); //начать работу
     radio.stopListening();  //не слушаем радиоэфир, мы передатчик
     
    _click_on_state = 1;
}

void setup() {
  Serial.begin(9600); //открываем порт для связи с ПК
  pinMode(wakeUpPin, INPUT);
    

  radio.begin(); //активировать модуль
  radio.setAutoAck(0);         //режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(0, 15);    //(время между попыткой достучаться, число попыток)
  radio.enableAckPayload();    //разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(32);     //размер пакета, в байтах

  radio.openWritingPipe(address[0]);   //мы - труба 0, открываем канал для передачи данных
  radio.setChannel(0x60);  //выбираем канал (в котором нет шумов!)

  radio.setPALevel (RF24_PA_MAX); //уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_1MBPS); //скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  //должна быть одинакова на приёмнике и передатчике!
  //при самой низкой скорости имеем самую высокую чувствительность и дальность!!

     //radio.powerUp(); //начать работу
     //radio.stopListening();  //не слушаем радиоэфир, мы передатчик
  attachInterrupt(digitalPinToInterrupt(wakeUpPin)
                  , InterruptWakeUp
                  , FALLING);
Serial.print("setup\n");
delay(1000);
}

void loop() {
  
        //Serial.print("radio sh down\n");

      //  Serial.print("wake up!\n");
  
  if (_click_on_state==1)
    {
    if (led_state == 0)
      {
        Serial.print("Sent: "); Serial.println(counter1);
        radio.write(&counter1, sizeof(counter1));
        led_state = 1;
        delay(10);  
      }
      else if (led_state == 1)
      {
        Serial.print("Sent: "); Serial.println(counter2);
        radio.write(&counter2, sizeof(counter2));  
        led_state = 0;
        delay(10); 
      }    
        radio.powerDown();
        
                 
      _click_on_state=0;
      delay(100);
      attachInterrupt(digitalPinToInterrupt(wakeUpPin)
                  , InterruptWakeUp
                  , FALLING);
    }
}
