#include <Arduino.h>
#include "Dip.h"

Dip::Dip(unsigned char pin1, unsigned char pin2, unsigned char pin3, unsigned char pin4) {
  _pin1 = pin1;
  _pin2 = pin2;
  _pin3 = pin3;
  _pin4 = pin4;
}

void Dip::init(void) {
  pinMode(_pin1, INPUT);
  pinMode(_pin2, INPUT);
  pinMode(_pin3, INPUT);
  pinMode(_pin4, INPUT);
}

byte Dip::getAddres() { // реализация метода getAddres
  _address = 0;
  if (digitalRead(_pin4)){
    _address |= 0x08;
  }
  if (digitalRead(_pin3)){
    _address |= 0x04;
  }
  if (digitalRead(_pin2)){
    _address |= 0x02;
  }
  if (digitalRead(_pin1)){
    _address |= 0x01;
  }
  return _address; //возвращаем значение адреса
}
