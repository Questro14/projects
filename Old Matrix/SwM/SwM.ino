#include "switch_matrix.h"
#include "lcd_display.h"
#include "switch_matrix.h"
#include "buttonz.h"
#include "hw_interface.h"
#include "core.h"
#include "lan_if.h"
#include <TimerOne.h>
#include "rom_memory.h"
#include "IP_change.cpp"


unsigned char cur_mac[6] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDA, 0x02 };
unsigned char cur_ip[4] = { 192, 168, 5, 16 };
unsigned char cur_gateway[4] = { 192, 168, 5, 1 };
unsigned char cur_subnet[4] = { 255, 255, 255, 0 };
String idn_string = "Tes-art,ATE-AESA,20190001,1.0";

unsigned char rcvd_cmd_id;
unsigned char rcvd_parameter[6];

lcd_display my_lcd;
core my_core;
Buttonz bntModeUp(modeUp_pin, pin_timeout);
Buttonz bntModeDown(modeDown_pin, pin_timeout);
Buttonz bntStateUp(stateUp_pin, pin_timeout);
Buttonz bntStateDown(stateDown_pin, pin_timeout);
hw_interface my_SPI(sw_data_pin, sw_sclk_pin, sw_nle_pin);


void setup() {
  Serial.begin(9600);
  //while(!Serial){;}
  my_SPI.init();
  my_lcd.init();
  bntModeUp.init();
  bntModeDown.init();
  bntStateUp.init();
  bntStateDown.init();
  config_lan();
  Timer1.initialize(100000);
  Timer1.attachInterrupt(Timer_ISR);
  interrupts();
}

void loop() {

  String text;
  bool state;
  if (!check_lan()) {
    my_core.remote_access = true;
    state = check_cmd(&rcvd_cmd_id, &rcvd_parameter[0]);
    if (rcvd_cmd_id != REQ_SYSTEM_ERROR_ID) {
      my_core.error = state;
    }
    switch (rcvd_cmd_id) {
      case REQ_IDN_ID:
        send_str_to_lan(&idn_string);
        break;
      case CMD_OUT_STATE_ID:
        my_core.state = rcvd_parameter[0];
        my_core.flag |= mask_core_update;
        break;
      case REQ_OUT_STATE_ID:
        text = String(my_core.state);
        send_str_to_lan(&text);
        break;
      case CMD_OUT_MODE_NORM_ID:
        my_core.mode = modeNorm;
        my_core.flag |= mask_core_update;
        break;
      case CMD_OUT_MODE_CAL1_ID:
        my_core.mode = modeCal1;
        my_core.flag |= mask_core_update;
        break;
      case CMD_OUT_MODE_CAL2_ID:
        my_core.mode = modeCal2;
        my_core.flag |= mask_core_update;
        break;
      case CMD_OUT_MODE_CAL3_ID:
        my_core.mode = modeCal3;
        my_core.flag |= mask_core_update;
        break;
      case REQ_OUT_MODE_ID:
        switch (my_core.mode) {
          case modeNorm: text = "NORMAL"; break;
          case modeCal1: text = "CAL1"; break;
          case modeCal2: text = "CAL2"; break;
          case modeCal3: text = "CAL3"; break;
        }
        send_str_to_lan(&text);
        break;
      case REQ_SYSTEM_ERROR_ID:
        switch (my_core.error) {
          case ERR_NO_ERROR: text = "0, No error"; break;
          case ERR_UNKNOWN_CMD: text = "1, Unknown parameter"; break;
        }
        my_core.error = ERR_NO_ERROR;
        send_str_to_lan(&text);
        break;
      case REQ_SET_MAC_ADDR_ID:
        text = String(cur_mac[0], HEX) + ":" +  String(cur_mac[1], HEX) + ":" +  String(cur_mac[2], HEX) + ":" +  String(cur_mac[3], HEX) + ":" +  String(cur_mac[4], HEX) + ":" +  String(cur_mac[5], HEX);
        send_str_to_lan(&text);
        break;
      case REQ_SET_IP_ADDR_ID:
        text = String(cur_ip[0]) + "." + String(cur_ip[1]) + "." + String(cur_ip[2]) + "." + String(cur_ip[3]);
        send_str_to_lan(&text);
        break;
      case REQ_SET_IP_MASK_ID:
        text = String(cur_subnet[0]) + "." + String(cur_subnet[1]) + "." + String(cur_subnet[2]) + "." + String(cur_subnet[3]);
        send_str_to_lan(&text);
        break;
      case REQ_SET_DEF_GATEWAY_ID:
        text = String(cur_gateway[0]) + "." + String(cur_gateway[1]) + "." + String(cur_gateway[2]) + "." + String(cur_gateway[3]);
        send_str_to_lan(&text);
        break;
      //case CMD_SET_IDN_ID:
        
    }
    my_core.flag |= mask_lcd_update;
  }
  // Check the state of the buttons
  if (my_core.remote_access) {
    if (bntModeDown.state == BTN_PRESSED && bntStateUp.state == BTN_PRESSED) {
      my_core.remote_access = false;
      server_stop();
      my_core.flag |= mask_lcd_update;
    }
    if (bntModeUp.state == BTN_PRESSED)    {
      bntModeUp.state = BTN_CHECKED;
    }
    if (bntModeDown.state == BTN_PRESSED)  {
      bntModeDown.state = BTN_CHECKED;
    }
    if (bntStateUp.state == BTN_PRESSED)   {
      bntStateUp.state = BTN_CHECKED;
    }
    if (bntStateDown.state == BTN_PRESSED) {
      bntStateDown.state = BTN_CHECKED;
    }
  }
  else {
    if (bntModeUp.state == BTN_PRESSED) {
      my_core.mode = (my_core.mode + 1)&mode_cntr_mask;
      my_core.flag |= mask_core_update;
      bntModeUp.state = BTN_CHECKED;
    }
    if (bntModeDown.state == BTN_PRESSED) {
      my_core.mode = (my_core.mode - 1)&mode_cntr_mask;
      my_core.flag |= mask_core_update;
      bntModeDown.state = BTN_CHECKED;
    }
    if (bntStateUp.state == BTN_PRESSED) {
      my_core.state++;
      my_core.flag |= mask_core_update;
      bntStateUp.state = BTN_CHECKED;
    }
    if (bntStateDown.state == BTN_PRESSED) {
      my_core.state--;
      my_core.flag |= mask_core_update;
      bntStateDown.state = BTN_CHECKED;
    }
  }

  if (my_core.flag & mask_core_update) {
    my_core.update();
    my_core.flag |= mask_hardware_update | mask_lcd_update;
    my_core.flag ^= mask_core_update;
  }
  if (my_core.flag & mask_hardware_update) {
    if (my_core.mode == modeNorm) {
      my_SPI.tx_data(&regSwitch[my_core.state][0]);
    }
    else {
      my_SPI.tx_data(&regState[my_core.mode][0]);
    }
    my_core.flag ^= mask_hardware_update;
  }
  if (my_core.flag & mask_lcd_update) {
    my_lcd.update(&my_core.mode, &my_core.state, &my_core.error, &my_core.remote_access);
    my_core.flag ^= mask_lcd_update;
  }
}

void Timer_ISR() {
  bntModeUp.CheckStatus();
  bntModeDown.CheckStatus();
  bntStateUp.CheckStatus();
  bntStateDown.CheckStatus();
}

void config_lan(void) {
  if (digitalRead(modeDown_pin) == HIGH || digitalRead(stateUp_pin) == HIGH) {
    // default settings
    recall_data(cur_mac, cur_ip, cur_gateway, cur_subnet);
  }
  server_start(cur_mac, cur_ip, cur_gateway, cur_subnet);

  String text;
  text = String(cur_ip[0]) + "." + String(cur_ip[1]) + "." + String(cur_ip[2]) + "." + String(cur_ip[3]);
  my_lcd.show_info(&text, 0);
  text = String(cur_subnet[0]) + "." + String(cur_subnet[1]) + "." + String(cur_subnet[2]) + "." + String(cur_subnet[3]);
  my_lcd.show_info(&text, 1);
  delay(2000);
  my_lcd.clear();
}

bool updateMAC(char *buf) {
  char *rez = cur_mac;
  unsigned char numb;
  bool next_char, end_of_string;
  int data;
  for (char i = 0; i < 6; i++) {
    data = 0;
    next_char = false;
    end_of_string = false;
    for (char j = 0; j < 3; j++) {
      switch (*buf) {
        case '0': numb = 0; break;
        case '1': numb = 1; break;
        case '2': numb = 2; break;
        case '3': numb = 3; break;
        case '4': numb = 4; break;
        case '5': numb = 5; break;
        case '6': numb = 6; break;
        case '7': numb = 7; break;
        case '8': numb = 8; break;
        case '9': numb = 9; break;
        case 'a': numb = 10; break;
        case 'b': numb = 11; break;
        case 'c': numb = 12; break;
        case 'd': numb = 13; break;
        case 'e': numb = 14; break;
        case 'f': numb = 15; break;
        case ':': next_char = true; break;
        case 0: end_of_string = true; break;
        default: return false; break;
      }
      if (end_of_string) {  // Символ конча строки
        if (i == 5 && j != 0) { // Если считаны 6 символов
          *rez = data;
          return true;
        }
        else {
          return false;
        }
      }
      buf++;
      if (next_char) {
        if (j != 0) {
          if (data < 256) {
            *rez = data;
            rez++;
            break;
          }
          else {
            return false;
          }
        }
        else {
          return false;
        }
      }
      else {
        data = (data << 4) + numb;
      }
    }

  }
  return true;
}

bool updateIPaddr(char *buf){
  return parseTCPIP(buf, cur_ip);
}

bool updateIPmask(char *buf){
  return parseTCPIP(buf, cur_subnet);
}

bool updateIPgateway(char *buf){
  return parseTCPIP(buf, cur_gateway);
}

bool parseTCPIP(char *buf, unsigned char *rez) {
  int data;
  char i, j;
  for (i = 0; i < 4; i++) {
    data = 0;
    for (j = 0; j < 4; j++) {
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

void updateIDN(char *buf){
  String *idn_pntr = &idn_string;
  for(char i;i<IDN_MAX_LENGHT;i++){
    if(buf ==0){
      *idn_pntr = *buf;
      return;
    }
    else{
      *idn_pntr = *buf;
      idn_pntr++;
      buf++;
    }
  }
}
