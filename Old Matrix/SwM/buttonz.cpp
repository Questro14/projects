#include "buttonz.h"
#include <arduino.h>
#include "lcd_display.h"

Buttonz::Buttonz(unsigned char sw_pin, unsigned long sw_delay) {
  pin = sw_pin;
  press_time = sw_delay;
  state = BTN_NON_PRESSED;
}


void Buttonz::init(void) {
  pinMode(pin, INPUT_PULLUP);
  time_stamp = millis();
}

void Buttonz::CheckStatus(void) {
  unsigned long cur_time = millis();
  switch (state) {
    case BTN_NON_PRESSED:
      if ((digitalRead(pin) == LOW)) {
        if ((cur_time - time_stamp) > press_time) {
          state = BTN_PRESSED;
        }
      }
      else {
        time_stamp = millis();
      }
      break;

    case BTN_CHECKED:
      if (digitalRead(pin) == HIGH) {
        state = BTN_NON_PRESSED;
        time_stamp = millis();
      }
      break;
    default:
    time_stamp = millis();
      break;

      
    case BTN_LONG_PRESSED:
      if (digitalRead(pin) == HIGH) {
       if (time_stamp > millis()) {
          state = BTN_LONG_PRESSED;
          Serial.println (548);
          }
        } 
  break;
  }
}
