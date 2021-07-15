#include "Switch.h"
#include <arduino.h>

void Switch::Init(void) {
  pinMode(data_pin, OUTPUT);
  digitalWrite(data_pin, LOW);
  pinMode(clk_pin, OUTPUT);
  digitalWrite(clk_pin, LOW);
  pinMode(le_pin, OUTPUT);
  digitalWrite(le_pin, LOW);
  
  for (char i = 1; i < MaxSwitch + 1; i++) {
    pinMode(SwBlock136[i].power_pin, OUTPUT);
  }

  
  //UpdateSwitchSPDT();
  //UpdateSwitch436();
  
 /* for (char i = 1; i < MaxSwitch + 1; i++) {
    SwitchPower(i);
  }*/
}

void Switch::UpdateSwitch136(void) {
  unsigned char mark;
  digitalWrite(le_pin, LOW);
  for (char a = MaxSwitch; a > 0; a--) {
    mark = SwBlock136[a].TxMark;
    for (char b = 0; b < SwBlock136[a].TxBit; b++) {
      if (SwBlock136[a].CtrlData[SwState[a]]&mark) {
        digitalWrite(clk_pin, LOW);
        digitalWrite(data_pin, HIGH);
        digitalWrite(clk_pin, HIGH);
      }
      else {
        digitalWrite(clk_pin, LOW);
        digitalWrite(data_pin, LOW);
        digitalWrite(clk_pin, HIGH);
      }
      mark = mark >> 1;
    }
  }
  digitalWrite(le_pin, HIGH);
}


void Switch::SwitchPower(unsigned char SwNum) {
  if (SwNum <= MaxSwitch) {
    digitalWrite(SwBlock136[SwNum].power_pin, HIGH);
    delay(PowerSwitchTime);
    digitalWrite(SwBlock136[SwNum].power_pin, LOW);
  }
}
