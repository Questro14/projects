#include "core.h"
#include <arduino.h>

core::core(void){
    mode = modeNorm;
    state = minStateValue;
    error = ERR_NO_ERROR;
    remote_access = false;
    flag |= mask_hardware_update | mask_lcd_update;
  }

void core::update(void){
    if(mode > modeCal3) {mode = modeCal3;}
    if(state == 0) {state = maxStateValue;}
    else {if(state > maxStateValue) {state = minStateValue;}}
  }
