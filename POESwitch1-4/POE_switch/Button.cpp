#include "Button.h"
#include <arduino.h>

Button::Button(unsigned char sw_pin, unsigned long sw_delay) {
  pin = sw_pin;
  press_time = sw_delay;
  state = BTN_NON_PRESSED;
}

void Button::Init(void) {
  pinMode(pin, INPUT);
  time_stamp = millis();
}

void Button::CheckStatus(void) {
  unsigned long cur_time = millis();
  bool but_state = digitalRead(pin);

  switch (state) {
    case BTN_NON_PRESSED:
      if (!but_state) {
        if ((cur_time - time_stamp) > press_time) {
          state = BTN_PRESSED;
        }
      }
      else {
        time_stamp = millis();
      }
      break;

    case BTN_CHECKED:
      if (but_state) {
        state = BTN_NON_PRESSED;
        time_stamp = millis();
      }
      break;
    default:
      time_stamp = millis();
      break;
  }

}
