#include "Core.h"
#include "Button.h"
#include <TimerOne.h>
#include "LAN.h"
#include "Parser.h"
#include "ROM_Memory.h"


#define pin_but 9


Button my_btn(pin_but, pin_timeout);
LAN my_lan;
parser my_parser;
ROM_Memory my_rom;
CoreData Corez;
CoreDataClass MyCore(&Corez);

char *pntr;
String text;
unsigned char numb;


struct Data {
  unsigned char Led;
  unsigned char Control;
};
Data DataControl[4] = {{8, 14},
  {7, 15},
  {6, 16},
  {5, 17},
};


Errorz my_error[4] = {{0, "No Error "},
  {1, "Wr Cmd   "},
  {2, "Wr Param "},
  {3, "Out Range"},
};

char IDN[IDN_MAX_LENGHT] = "POEswitch, 123456789, TESART, v1.0";
unsigned char IDN_Length = 33;

void config_lan(void) {

  if (!digitalRead(pin_but)) {
    my_rom.Recall_LAN(my_lan.cur_mac, my_lan.cur_ip, my_lan.cur_subnet, my_lan.cur_gateway);
    my_rom.Recall_IDN(IDN, &IDN_Length);
  }
  my_lan.InitLAN();
}

void Timer_ISR() {
  my_btn.CheckStatus();
}

void setup() {
  // put your setup code here, to run once:
  my_btn.Init();
  config_lan();
  MyCore.Init();
  Timer1.initialize(100000);
  Timer1.attachInterrupt(Timer_ISR);
  for (char i = 0; i < 4; i++) {
    pinMode(DataControl[i].Led, OUTPUT);
    pinMode(DataControl[i].Control, OUTPUT);
  }
  drive(MyCore.channel);

}

void loop() {
  MyCore.Update();
  drive(MyCore.channel);
  if (my_lan.CheckLAN()) {
    my_parser.toUpCase(my_lan.rx_buf);
    if (my_parser.compStr(my_lan.rx_buf, REQ_IDN)) {
      my_lan.SendData(IDN, IDN_Length);
      my_lan.send_term_char();
      Corez.ErrorNum = no_error;
    }
    else if (my_parser.compStr(my_lan.rx_buf, REQ_OPC)) {
      text = "1";
      my_lan.send_str_to_lan(&text);
      Corez.ErrorNum = no_error;
    }
    else if (my_parser.compStr(my_lan.rx_buf, REQ_SYSTEM_ERROR)) {
      text = String(my_error[Corez.ErrorNum].Number) + ", " + my_error[Corez.ErrorNum].Description;
      my_lan.send_str_to_lan(&text);
      Corez.ErrorNum = no_error;
    }
    else if (my_parser.compStr(my_lan.rx_buf, CMD_RST)) {
      MyCore.Init();
      Corez.ErrorNum = no_error;
    }
    else if (my_parser.compStr(my_lan.rx_buf, CMD_SET_IDN)) {
      pntr = my_lan.rx_buf + sizeof(CMD_SET_IDN) - 1;
      my_rom.Store_IDN(pntr);
      Corez.ErrorNum = no_error;
    }
    else if (my_parser.compStr(my_lan.rx_buf, CMD_SET_MAC_ADDR)) {
      pntr = my_lan.rx_buf + sizeof(CMD_SET_MAC_ADDR) - 1;
      if (my_parser.parseMAC(pntr, my_lan.cur_mac)) {
        my_rom.Store_LAN(my_lan.cur_mac, my_lan.cur_ip, my_lan.cur_subnet , my_lan.cur_gateway);
        Corez.ErrorNum = no_error;
      }
      else {
        Corez.ErrorNum = wrong_parameter;
      }
    }
    else if (my_parser.compStr(my_lan.rx_buf, CMD_SET_IP_ADDR)) {
      pntr = my_lan.rx_buf + sizeof(CMD_SET_IP_ADDR) - 1;
      if (my_parser.parseIP(pntr, my_lan.cur_ip)) {
        my_rom.Store_LAN(my_lan.cur_mac, my_lan.cur_ip, my_lan.cur_subnet , my_lan.cur_gateway);
        Corez.ErrorNum = no_error;
      }
      else {
        Corez.ErrorNum = wrong_parameter;
      }
    }
    else if (my_parser.compStr(my_lan.rx_buf, CMD_SET_IP_MASK)) {
      pntr = my_lan.rx_buf + sizeof(CMD_SET_IP_MASK) - 1;
      if (my_parser.parseIP(pntr, my_lan.cur_subnet)) {
        my_rom.Store_LAN(my_lan.cur_mac, my_lan.cur_ip, my_lan.cur_subnet , my_lan.cur_gateway);
        Corez.ErrorNum = no_error;
      }
      else {
        Corez.ErrorNum = wrong_parameter;
      }
    }
    else if (my_parser.compStr(my_lan.rx_buf, CMD_SET_DEF_GATEWAY)) {
      pntr = my_lan.rx_buf + sizeof(CMD_SET_DEF_GATEWAY) - 1;
      if (my_parser.parseIP(pntr, my_lan.cur_gateway)) {
        my_rom.Store_LAN(my_lan.cur_mac, my_lan.cur_ip, my_lan.cur_subnet , my_lan.cur_gateway);
        Corez.ErrorNum = no_error;
      }
      else {
        Corez.ErrorNum = wrong_parameter;
      }
    }
    else if (my_parser.compStr(my_lan.rx_buf, REQ_SET_MAC_ADDR)) {
      text = String(my_lan.cur_mac[0], HEX) + ":" + String(my_lan.cur_mac[1], HEX) + ":" + String(my_lan.cur_mac[2], HEX) + ":"  + String(my_lan.cur_mac[3], HEX) + ":" + String(my_lan.cur_mac[4], HEX) + ":" + String(my_lan.cur_mac[5], HEX);
      my_lan.send_str_to_lan(&text);
      Corez.ErrorNum = no_error;
    }
    else if (my_parser.compStr(my_lan.rx_buf, REQ_SET_IP_ADDR)) {
      text = String(my_lan.cur_ip[0]) + "." + String(my_lan.cur_ip[1]) + "." + String(my_lan.cur_ip[2]) + "." + String(my_lan.cur_ip[3]);
      my_lan.send_str_to_lan(&text);
      Corez.ErrorNum = no_error;
    }
    else if (my_parser.compStr(my_lan.rx_buf, REQ_SET_IP_MASK)) {
      text = String(my_lan.cur_subnet[0]) + "." + String(my_lan.cur_subnet[1]) + "." + String(my_lan.cur_subnet[2]) + "." + String(my_lan.cur_subnet[3]);
      my_lan.send_str_to_lan(&text);
      Corez.ErrorNum = no_error;
    }
    else if (my_parser.compStr(my_lan.rx_buf, REQ_SET_DEF_GATEWAY)) {
      text = String(my_lan.cur_gateway[0]) + "." + String(my_lan.cur_gateway[1]) + "." + String(my_lan.cur_gateway[2]) + "." + String(my_lan.cur_gateway[3]);
      my_lan.send_str_to_lan(&text);
      Corez.ErrorNum = no_error;
    }
    else if (my_parser.compStr(my_lan.rx_buf, REQ_STATE_SWITCH)) {
      switch (MyCore.channel) {
        case (0):
          MyCore.chSend = 2;
          break;
        case (1):
          MyCore.chSend = 3;
          break;
        case (2):
          MyCore.chSend = 5;
          break;
        case (3):
          MyCore.chSend = 6;
          break;
        default:
          Corez.ErrorNum = wrong_parameter;
          break;

      }
      text = String(MyCore.chSend);
      my_lan.send_str_to_lan(&text);
      Corez.ErrorNum = no_error;
    }
    else if (my_parser.compStr(my_lan.rx_buf, CMD_STATE_SWITCH1)) {
      MyCore.channel = 0;
      Corez.ErrorNum = no_error;
    }
    else if (my_parser.compStr(my_lan.rx_buf, CMD_STATE_SWITCH2)) {
      MyCore.channel = 1;
      Corez.ErrorNum = no_error;
    }
    else if (my_parser.compStr(my_lan.rx_buf, CMD_STATE_SWITCH3)) {
      MyCore.channel = 2;
      Corez.ErrorNum = no_error;
    }
    else if (my_parser.compStr(my_lan.rx_buf, CMD_STATE_SWITCH4)) {
      MyCore.channel = 3;
      Corez.ErrorNum = no_error;
    }


    else {
      Corez.ErrorNum = wrong_comand;
      text = String(my_error[Corez.ErrorNum].Number) + ", " + my_error[Corez.ErrorNum].Description;
      my_lan.send_str_to_lan(&text);

    }
  }

  if (my_btn.state == BTN_PRESSED) {
    my_btn.state = BTN_CHECKED;
    ++MyCore.channel;

  }

}

void drive(int stat) {
  for (int i = 0; i < 4; ++i) {
    if (i == MyCore.channel) {
      continue;
    }
    digitalWrite(DataControl[i].Led, LOW);
    digitalWrite(DataControl[i].Control, LOW);
  }
  digitalWrite(DataControl[stat].Led, HIGH);
  digitalWrite(DataControl[stat].Control, HIGH);
}
