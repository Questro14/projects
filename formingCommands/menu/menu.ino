#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <shButton.h>
#include "core.h"
#include "lan_if.h"
#include "rom_memory.h"
#include "formingCommands.h"


unsigned char cur_mac[6] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDA, 0x02 };
unsigned char cur_ip[4] = { 192, 168, 88, 87 };
unsigned char cur_gateway[4] = { 192, 168, 88, 1 };
unsigned char cur_subnet[4] = { 255, 255, 255, 0 };
String idn_string = "SP6701C, 2006001, TESART, v1.0";

unsigned char rcvd_cmd_id;
unsigned int rcvd_parameter[17];

LiquidCrystal_I2C lcd(0x27, 16, 2);
shButton btn1(BTN1, PULL_DOWN);
shButton btn2(BTN2, PULL_DOWN);
shButton btn3(BTN3, PULL_DOWN);

/*shButton btn1(BTN1, PULL_UP);
  shButton btn2(BTN2, PULL_UP);
  shButton btn3(BTN3, PULL_UP);*/

core my_core;

uint8_t corsor_pos = 1;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(4, 0);
  lcd.backlight();
  lcd.print("FORMING");
  lcd.setCursor(4, 1);
  lcd.print("COMMANDS");
  delay(3000);
  lcd.clear();
  //Serial.begin(9600);
  config_lan();


  btn1.setVirtualClickOn(true);
  btn2.setVirtualClickOn(true);
  btn3.setVirtualClickOn(true);
  btn1.setLongClickMode(LCM_CLICKSERIES);
  btn2.setLongClickMode(LCM_CLICKSERIES);
  btn3.setLongClickMode(LCM_CLICKSERIES);

  pinMode(OPTORELAY1, OUTPUT);
  pinMode(OPTORELAY2, OUTPUT);
  pinMode(OPTORELAY3, OUTPUT);
  pinMode(OPTORELAY4, OUTPUT);

  digitalWrite(OPTORELAY1, LOW);
  digitalWrite(OPTORELAY2, LOW);
  digitalWrite(OPTORELAY3, LOW);
  digitalWrite(OPTORELAY4, LOW);

  pinMode(SW1, OUTPUT);
  pinMode(SW2, OUTPUT);
  pinMode(SW3, OUTPUT);
  pinMode(SW4, OUTPUT);

  digitalWrite(SW1, LOW);
  digitalWrite(SW2, LOW);
  digitalWrite(SW3, LOW);
  digitalWrite(SW4, LOW);
}

void loop() {
  my_core.update();
  printCorsor(corsor_pos); // отрисовка курсора
  RELAYDRIVE(my_core.channel);
  lcd.setCursor(1, 0);
  lcd.print("SELECT:");
  lcd.setCursor(8, 0);
  lcd.print(my_core.channel);

  lcd.setCursor(1, 1);
  lcd.print("PULSE :");
  lcd.setCursor(8, 1);
  lcd.print(my_core.pulse);

  lcd.setCursor(14, 0);
  lcd.print("OK");


  switch (btn3.getButtonState ()) {
    case BTN_ONECLICK:
      switch (corsor_pos) {
        case 1:
          corsor_pos = 4;
          break;
        case 2:
          corsor_pos = 5;
          break;
        case 3: // когда курсор в положении "ОК"
          //"ОК"
          if (my_core.channel == 0) {
            lcd.clear();
            lcd.setCursor(5, 0);
            lcd.print("Channel");
            lcd.setCursor(2, 1);
            lcd.print("not selected");
            delay(2000);
            lcd.clear();

          }
          else {
            lcd.clear();
            lcd.setCursor(5, 0);
            lcd.print("START");
            START(my_core.channel, my_core.pulse);
            delay(2000);
            lcd.clear();
          }
          break;
        case 4:
          corsor_pos = 1;
          break;
        case 5:
          corsor_pos = 2;
          break;
      }
  }


  switch (btn1.getButtonState()) {
    case BTN_ONECLICK:
      switch (corsor_pos) {
        case 1:
          corsor_pos = 3;
          break;
        case 2:
          corsor_pos = 1;
          break;
        case 3:
          corsor_pos = 2;
          break;
        case 4:
          ++my_core.channel;
          break;
        case 5:
          lcd.setCursor(10, 1);
          lcd.print(" ");
          ++my_core.pulse;
          break;
      }
    case BTN_DOWN:
      break;
    case BTN_LONGCLICK:
      switch (corsor_pos) {
        case 4:
          //++my_core.channel; // задаем канал
          break;
        case 5:
          lcd.setCursor(10, 1);
          lcd.print(" ");
          ++my_core.pulse;
          break;
      }
  }


  switch (btn2.getButtonState()) {
    case BTN_ONECLICK:
      switch (corsor_pos) {
        case 1:
          corsor_pos = 2;
          break;
        case 2:
          corsor_pos = 3;
          break;
        case 3:
          corsor_pos = 1;
          break;
        case 4:
          --my_core.channel;
          break;
        case 5:
          lcd.setCursor(10, 1);
          lcd.print(" ");
          --my_core.pulse;
          break;
      }
    case BTN_DOWN:
      break;
    case BTN_LONGCLICK:
      switch (corsor_pos) {
        case 4:
          //--my_core.channel; // задаем канал
          break;
        case 5:
          lcd.setCursor(10, 1);
          lcd.print(" ");
          --my_core.pulse;
          break;
      }
  }
  String text;
  char state;
  if (check_lan()) {
    // Serial.println("LAN START");
    //delay (1000);
    state = check_cmd(&rcvd_cmd_id, &rcvd_parameter[0]);
    switch (rcvd_cmd_id) {
      case REQ_OUT_PULSEWIDTH_ID:
        text = String(my_core.pulse);
        send_str_to_lan(&text);
        //Serial.println(my_core.pulse);
        //Serial.println("PULSE?");
        break;
      ////////////////
      case CMD_OUT_PULSEWIDTH_ID:
        if (rcvd_parameter[0] > 200 || rcvd_parameter[0] < 30) {
          lcd.setCursor(10, 1);
          lcd.print(" ");
          my_core.error = WRONG_PARAM;
          text = ER2;
          send_str_to_lan(&text);
          //Serial.println(rcvd_parameter[0]);
          //Serial.println("ER PULSE");
        }
        else {
          lcd.setCursor(10, 1);
          lcd.print(" ");
          my_core.pulse = rcvd_parameter[0];
          // Serial.println("PULSE SET");
        }
        break;
      /////////////////
      case CMD_SET_IP_ADDR_ID:
        if (updateIPaddr(rcvd_parameter)) {
          store_data(cur_mac, cur_ip, cur_gateway, cur_subnet);
          /*Serial.println(rcvd_parameter[0]);
            Serial.print(cur_ip[0]);
            Serial.print(cur_ip[1]);
            Serial.print(cur_ip[2]);
            Serial.println(cur_ip[3]);*/
        }
        else {
          my_core.error = WRONG_PARAM;
          text = ER2;
          send_str_to_lan(&text);
        }
        break;
      /////////////////////
      case CMD_SET_IP_MASK_ID:
        if (updateIPmask(rcvd_parameter)) {
          store_data(cur_mac, cur_ip, cur_gateway, cur_subnet);
          /*Serial.print(cur_subnet[0]);
            Serial.print(cur_subnet[1]);
            Serial.print(cur_subnet[2]);
            Serial.println(cur_subnet[3]);*/
        }
        else {
          my_core.error = WRONG_PARAM;
          text = ER2;
          send_str_to_lan(&text);
        }
        break;
      //////////////
      case CMD_SET_DEF_GATEWAY_ID:
        if (updateIPgateway(rcvd_parameter)) {
          store_data(cur_mac, cur_ip, cur_gateway, cur_subnet);
          /* Serial.print(cur_gateway[0]);
            Serial.print(cur_gateway[1]);
            Serial.print(cur_gateway[2]);
            Serial.println(cur_gateway[3]);*/
        }
        else {
          my_core.error = WRONG_PARAM;
          text = ER2;
          send_str_to_lan(&text);
        }
        break;
      ////////////////////////
      case REQ_IDN_ID:
        send_str_to_lan(&idn_string);
        //Serial.println("IDN?");
        break;
      case CMD_IN_SELECT_ID:
        if (rcvd_parameter[0] == 0 ) {
          my_core.channel = rcvd_parameter[0];
        }
        else if (rcvd_parameter[0] == 1 ) {
          my_core.channel = rcvd_parameter[0];
        }
        else if (rcvd_parameter[0] == 2) {
          my_core.channel = rcvd_parameter[0];
        }
        else if (rcvd_parameter[0] == 3) {
          my_core.channel = rcvd_parameter[0];
        }
        else if (rcvd_parameter[0] == 4) {
          my_core.channel = rcvd_parameter[0];
        }
        else {
          my_core.error = WRONG_PARAM;
          text = ER2;
          send_str_to_lan(&text);
        }
        //Serial.println(" SELECT 0");
        break;
      ////////////////////////////
      case REQ_IN_SELECT_ID:
        text = String(my_core.channel);
        send_str_to_lan(&text);
        // Serial.println("REQ_SELECT");
        break;
      ///////////////////
      case CMD_IN_START_ID:
        if (my_core.channel == 0) {
          text = ER3;
          my_core.error = CN_NOT_SELECT;
          send_str_to_lan(&text);
        }
        else {
          lcd.clear();
          lcd.setCursor(5, 0);
          lcd.print("START");
          START(my_core.channel, my_core.pulse);
          delay(2000);
          //Serial.println("START");
          lcd.clear();
        }
        break;
      ///////////////////////
      case REQ_SET_MAC_ADDR_ID:
        text = (String(cur_mac[0], HEX) + ":" +  String(cur_mac[1], HEX) + ":" +  String(cur_mac[2], HEX) + ":" +  String(cur_mac[3], HEX) + ":" +  String(cur_mac[4], HEX) + ":" +  String(cur_mac[5], HEX));
        send_str_to_lan(&text);
        //Serial.println("11");
        break;
      ////////////////////////
      case REQ_SET_IP_ADDR_ID:
        text = String(cur_ip[0]) + "." + String(cur_ip[1]) + "." + String(cur_ip[2]) + "." + String(cur_ip[3]);
        send_str_to_lan(&text);
        //Serial.println("12");
        break;
      /////////////////////////
      case REQ_SET_IP_MASK_ID:
        text = String(cur_subnet[0]) + "." + String(cur_subnet[1]) + "." + String(cur_subnet[2]) + "." + String(cur_subnet[3]);
        send_str_to_lan(&text);
        // Serial.println("13");
        break;
      /////////////////////////
      case REQ_SET_DEF_GATEWAY_ID:
        text = String(cur_gateway[0]) + "." + String(cur_gateway[1]) + "." + String(cur_gateway[2]) + "." + String(cur_gateway[3]);
        send_str_to_lan(&text);
        //Serial.println("14");
        break;
      /////////////////
      case REQ_SYSTEM_ERROR_ID:
        switch (my_core.error) {
          case ERR_NO_ERROR: text = "0, " + String(ER0); break;
          case ERR_UNKNOWN_CMD: text = "1, " + String(ER1); break;
          case WRONG_PARAM: text = "2, " + String(ER2); break;
          case CN_NOT_SELECT: text = "5, " + String(ER3); break;

        }
        my_core.error = ERR_NO_ERROR;
        send_str_to_lan(&text);
        break;
      /////////////
      default:
        text = ER1;
        my_core.error = ERR_UNKNOWN_CMD;
        send_str_to_lan(&text);
        //Serial.println("DEFAULT");
        break;
    }
    //Serial.println("LAN END");
  }
  //Serial.println("LOOP END");

}

void printCorsor(uint8_t str) {  // функция отрисовки курсора
  //получаем положение курсоса и отрисовываем
  switch (str) {
    case 1:
      lcd.setCursor(0, 1);
      lcd.print(" ");
      lcd.setCursor(13, 0);
      lcd.print(" ");
      lcd.setCursor(11, 0);
      lcd.print(" ");
      lcd.setCursor(11, 1);
      lcd.print(" ");
      lcd.setCursor(0, 0);
      lcd.print(">");
      break;
    case 2:
      lcd.setCursor(0, 0);
      lcd.print(" ");
      lcd.setCursor(13, 0);
      lcd.print(" ");
      lcd.setCursor(11, 0);
      lcd.print(" ");
      lcd.setCursor(11, 1);
      lcd.print(" ");
      lcd.setCursor(0, 1);
      lcd.print(">");
      break;
    case 3:
      lcd.setCursor(0, 0);
      lcd.print(" ");
      lcd.setCursor(0, 1);
      lcd.print(" ");
      lcd.setCursor(11, 0);
      lcd.print(" ");
      lcd.setCursor(11, 1);
      lcd.print(" ");
      lcd.setCursor(13, 0);
      lcd.print(">");
      break;
    case 4:
      lcd.setCursor(0, 0);
      lcd.print(" ");
      lcd.setCursor(0, 1);
      lcd.print(" ");
      lcd.setCursor(13, 0);
      lcd.print(" ");
      lcd.setCursor(11, 1);
      lcd.print(" ");
      lcd.setCursor(11, 0);
      lcd.print("<");
      break;
    case 5:
      lcd.setCursor(0, 0);
      lcd.print(" ");
      lcd.setCursor(0, 1);
      lcd.print(" ");
      lcd.setCursor(13, 0);
      lcd.print(" ");
      lcd.setCursor(11, 0);
      lcd.print(" ");
      lcd.setCursor(11, 1);
      lcd.print("<");
      break;
  }
}


void config_lan(void) {
  if (digitalRead(BTN2) == HIGH) { // default settings
    store_data(cur_mac, cur_ip, cur_gateway, cur_subnet);
  }
  recall_data(cur_mac, cur_ip, cur_gateway, cur_subnet);
  server_start(cur_mac, cur_ip, cur_gateway, cur_subnet);

  String text1;
  text1 = String(cur_ip[0]) + "." + String(cur_ip[1]) + "." + String(cur_ip[2]) + "." + String(cur_ip[3]);
  lcd.setCursor(0, 0);
  lcd.print(text1);
  text1 = String(cur_subnet[0]) + "." + String(cur_subnet[1]) + "." + String(cur_subnet[2]) + "." + String(cur_subnet[3]);
  lcd.setCursor(0, 1);
  lcd.print(text1);
  delay(2000);
  lcd.clear();
}

void START(int ch, unsigned int pl) {
  switch (ch) {
    case 0:
      break;
    case 1:
      PULSEDELAY(pl, OPTORELAY1);
      break;
    case 2:
      PULSEDELAY(pl, OPTORELAY2);
      break;
    case 3:
      PULSEDELAY(pl, OPTORELAY3);
      break;
    case 4:
      PULSEDELAY(pl, OPTORELAY4);
      break;
  }
}

void RELAYDRIVE(int channel) {
  switch (channel) {
    case 0:
      digitalWrite(SW2, LOW);
      digitalWrite(SW3, LOW);
      digitalWrite(SW4, LOW);
      digitalWrite(SW1, LOW);
      break;
    case 1:
      digitalWrite(SW2, LOW);
      digitalWrite(SW3, LOW);
      digitalWrite(SW4, LOW);
      digitalWrite(SW1, HIGH);
      break;
    case 2:
      digitalWrite(SW1, LOW);
      digitalWrite(SW3, LOW);
      digitalWrite(SW4, LOW);
      digitalWrite(SW2, HIGH);
      break;
    case 3:
      digitalWrite(SW1, LOW);
      digitalWrite(SW2, LOW);
      digitalWrite(SW4, LOW);
      digitalWrite(SW3, HIGH);
      break;
    case 4:
      digitalWrite(SW1, LOW);
      digitalWrite(SW2, LOW);
      digitalWrite(SW3, LOW);
      digitalWrite(SW4, HIGH);
      break;
  }
}

void PULSEDELAY(unsigned int p, int opt) {
  digitalWrite(opt, HIGH);
  delay(p);
  digitalWrite(opt, LOW);
}

/*bool updateMAC(char *buf) {
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
           rez = data;
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
             rez = data;
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
  }*/

bool updateIPaddr(unsigned int *buf) {
  return parseTCPIP(buf, cur_ip);
}

bool updateIPmask(unsigned int *buf) {
  store_data(cur_mac, cur_ip, cur_gateway, cur_subnet);
  return parseTCPIP(buf, cur_subnet);
}

bool updateIPgateway(unsigned int *buf) {
  return parseTCPIP(buf, cur_gateway);
}

bool parseTCPIP(unsigned int *buf, unsigned char *rez) {
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

/*void updateIDN(char *buf) {
  String *idn_pntr = &idn_string;
  for (char i; i < IDN_MAX_LENGHT; i++) {
    if (buf == 0) {
       idn_pntr = *buf;
      return;
    }
    else {
       idn_pntr = *buf;
      idn_pntr++;
      buf++;
    }
  }
  }*/
