#include <Arduino.h>
#ifndef Dip_h
#define Dip_h

class Dip {
  
  public:
    Dip(unsigned char pin1, unsigned char pin2, unsigned char pin3, unsigned char pin4);
    unsigned char getAddres(void);
    void init(void);
    
  private:
    unsigned char _address;
    unsigned char _pin1, _pin2, _pin3, _pin4;
};
#endif
