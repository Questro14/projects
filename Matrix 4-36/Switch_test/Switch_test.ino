#include "Switch.h"

char Data[16];
unsigned char Param[2];
bool Update;
Switch my_sw;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  my_sw.Init();
  //pinMode(4, OUTPUT);
  //digitalWrite(4, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    Serial.readBytesUntil('\n', Data, 16);
    if (parseParam(Data, Param)) {
      Serial.println("Parce Ok!");
      Update = true;
      if (Param[0] > MaxSwitch) {
        Serial.println("Switch number > max!");
        Param[0] = 0;
        Update = false;
      }
      if (Param[1] > my_sw.SwBlock[Param[0]].MaxState) {
        Serial.println("Switch state > max!");
        Param[0] = 0;
        Update = false;
      }
      //Serial.println("Switch = " + String(Param[0]) + ", State = " + String(Param[1]));
      my_sw.SwState[Param[0]] = Param[1];
      for (unsigned char i = 1; i < MaxSwitch + 1; i++) {
        Serial.println("Switch = " + String(i) + ", State = " + String(my_sw.SwState[i]));
      }
      if (Update) {
        my_sw.UpdateSwitch();
        my_sw.SwitchPower(Param[0]);
      }
    }
    else {
      Serial.println("Something wrong!");
    }
    Serial.readBytesUntil('\r', Data, 16);
    buf_null(Data);
  }
}


bool parseParam(char *buf, unsigned char *rez) {
  int data;
  char i, j;
  for (i = 0; i < 2; i++) {
    data = 0;
    for (j = 0; j < 3; j++) {
      if (*buf == '.' || *buf == 0) {
        if (j != 0) {
          buf++;
          break;
        }
        else {
          return false;
        }
      }
      if (*buf >= '0' && *buf <= '9') {
        data = (data << 3) + (data << 1) + *buf - '0';
      }
      else {
        return false;
      }
      buf++;
    }
    if (data < 256) {
      *rez = data;
      rez++;
    }
    else {
      return false;
    }
  }
  return true;
}

void buf_null(char *buf) {
  for (int i = 0; i < 16; i++) {
    *buf = 0; buf++;
  }
}
