#ifndef __Button__
#define __Button__

#include <arduino.h>

#define BTN_NON_PRESSED 0
#define BTN_PRESSED 1
#define BTN_CHECKED 2


#define pin_timeout 70

class Button{
  private:
    unsigned char pin;
    unsigned long press_time;
    unsigned long time_stamp;
  public:
    unsigned char state;
    Button(unsigned char sw_pin, unsigned long sw_delay);
    void Init(void);
    void CheckStatus(void);
};

#endif
