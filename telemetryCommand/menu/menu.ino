#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <shButton.h>
#include "core.h"
#include "lan_if.h"
#include "rom_memory.h"
#include "telemetry_com.h"

unsigned char cur_mac[6] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDA, 0x02 };
unsigned char cur_ip[4] = { 192, 168, 88, 87 };
unsigned char cur_gateway[4] = { 192, 168, 88, 1};
unsigned char cur_subnet[4] = { 255, 255, 255, 0 };
String idn_string = "SPE-LNA-OT1 , 2007001, TESART, v1.0";



unsigned char rcvd_cmd_id;
unsigned int rcvd_parameter[17];


LiquidCrystal_I2C lcd(0x27, 16, 2);

shButton btnSelect(BTN1, PULL_DOWN );
shButton btnDown(BTN2, PULL_DOWN );
shButton btnUp(BTN3, PULL_DOWN );

core my_core;


uint8_t corsor_pos = 1;

void setup() {

  lcd.init();
  lcd.backlight();
  lcd.setCursor(3, 0);
  lcd.print("TELEMETRY");
  lcd.setCursor(4, 1);
  lcd.print("COMMAND");
  delay(3000);
  lcd.clear();

  pinMode(Ar1, OUTPUT);
  pinMode(Ar2, OUTPUT);
  pinMode(Ar3, OUTPUT);
  pinMode(Ar4, OUTPUT);
  pinMode(Ar5, OUTPUT);

  digitalWrite(Ar1, LOW);
  digitalWrite(Ar2, LOW);
  digitalWrite(Ar3, LOW);
  digitalWrite(Ar4, LOW);
  digitalWrite(Ar5, LOW);
  config_lan();
  btnSelect.setVirtualClickOn(true);
  btnUp.setVirtualClickOn(true);
  btnDown.setVirtualClickOn(true);

}

void loop() {
  my_core.update();
  driveRelay(my_core.channel, my_core.polarity);
  printCorsor(corsor_pos); // отрисовка курсора
  lcd.setCursor(1, 0);
  lcd.print("CHANNEL :");
  lcd.setCursor(10, 0);
  lcd.print(my_core.channel);

  lcd.setCursor(1, 1);
  lcd.print("POLARITY:");
  lcd.setCursor(10, 1);
  switch (my_core.polarity) {
    case 1:
      lcd.print("POS");
      break;
    case 2:
      lcd.print("NEG");
      break;
  }

  //////////select////////
  switch (btnSelect.getButtonState ()) {
    case BTN_ONECLICK:
      switch (corsor_pos) {
        case 1:
          corsor_pos = 3;
          break;
        case 2:
          corsor_pos = 4;
          break;
        case 3:
          corsor_pos = 1;
          break;
        case 4:
          corsor_pos = 2;
          break;
      }
  }
  /////////вверх/////////
  switch (btnUp.getButtonState()) {
    case BTN_ONECLICK:
      switch (corsor_pos) {
        case 1:
          corsor_pos = 2;
          break;
        case 2:
          corsor_pos = 1;
          break;
        case 3:
          ++my_core.channel;
          break;
        case 4:
          ++my_core.polarity;
          break;

      }
  }
  //////////////низ////////////////
  switch (btnDown.getButtonState()) {
    case BTN_ONECLICK:
      switch (corsor_pos) {
        case 1:
          corsor_pos = 2;
          break;
        case 2:
          corsor_pos = 1;
          break;
        case 3:
          --my_core.channel;
          break;
        case 4:
          --my_core.polarity;
          break;
      }
  }

  unsigned char res[20];
  String text;
  char state;
  if (check_lan()) {
    //Serial.println("LAN START");
    //delay (1000);
    state = check_cmd(&rcvd_cmd_id, &rcvd_parameter[0]);
    switch (rcvd_cmd_id) {
      /////////////////////////
      case REQ_IDN_ID:
        send_str_to_lan(&idn_string);
        //Serial.println("1");
        break;
      /////////////////////
      case CMD_IN_SELECT_ID:
        //my_core.channel = rcvd_parameter[0];
        if (rcvd_parameter[0] == 0 ) {
          my_core.channel = rcvd_parameter[0];
        }
        else if (rcvd_parameter[0] == 1 ) {
          my_core.channel = rcvd_parameter[0];
        }
        else if (rcvd_parameter[0] == 2) {
          my_core.channel = rcvd_parameter[0];
        }
        else {
          my_core.error = WRONG_PARAM;
          text = ER2;
          send_str_to_lan(&text);
        }
        break;
      /////////////////////////
      case REQ_IN_SELECT_ID:
        text = String(my_core.channel);
        send_str_to_lan(&text);
        break;
      //////////////////////
      case CMD_IN_POLARITY_POS_ID:
        my_core.polarity = 1;
        break;
      ////////////////////////
      case CMD_IN_POLARITY_NEG_ID:
        my_core.polarity = 2;
        break;

      case REQ_IN_POLARITY_ID:
        switch (my_core.polarity) {
          case 1:
            text = "POS";
            send_str_to_lan(&text);
            break;
          case 2:
            text = "NEG";
            send_str_to_lan(&text);

            break;
        }
        break;

      //////////////////////////
      case CMD_SET_IP_ADDR_ID:
        if (updateIPaddr(rcvd_parameter)) {
          store_data(cur_mac, cur_ip, cur_gateway, cur_subnet);
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
        }
        else {
          my_core.error = WRONG_PARAM;
          text = ER2;
          send_str_to_lan(&text);
        }
        break;
      ///////////////////////
      case CMD_SET_DEF_GATEWAY_ID:
        if (updateIPgateway(rcvd_parameter)) {
          store_data(cur_mac, cur_ip, cur_gateway, cur_subnet);
        }
        else {
          my_core.error = WRONG_PARAM;
          text = ER2;
          send_str_to_lan(&text);
        }
        break;
      /////////////////////
      case REQ_SET_MAC_ADDR_ID:
        text = String(cur_mac[0], HEX) + ":" +  String(cur_mac[1], HEX) + ":" +  String(cur_mac[2], HEX) + ":" +  String(cur_mac[3], HEX) + ":" +  String(cur_mac[4], HEX) + ":" +  String(cur_mac[5], HEX);
        send_str_to_lan(&text);
        // Serial.println("8");
        break;
      ////////////////////////
      case REQ_SET_IP_ADDR_ID:
        text = String(cur_ip[0]) + "." + String(cur_ip[1]) + "." + String(cur_ip[2]) + "." + String(cur_ip[3]);
        send_str_to_lan(&text);
        // Serial.println("9");
        break;
      ////////////////////////
      case REQ_SET_IP_MASK_ID:
        text = String(cur_subnet[0]) + "." + String(cur_subnet[1]) + "." + String(cur_subnet[2]) + "." + String(cur_subnet[3]);
        send_str_to_lan(&text);
        //Serial.println("10");
        break;
      /////////////////////
      case REQ_SET_DEF_GATEWAY_ID:
        text = String(cur_gateway[0]) + "." + String(cur_gateway[1]) + "." + String(cur_gateway[2]) + "." + String(cur_gateway[3]);
        send_str_to_lan(&text);
        //Serial.println("11");
        break;
  
      //////////////////
      case REQ_SYSTEM_ERROR_ID:
        switch (my_core.error) {
          case ERR_NO_ERROR: text = "0, " + String(ER0); break;
          case ERR_UNKNOWN_CMD: text = "1, " + String(ER1); break;
          case WRONG_PARAM: text = "2, " + String(ER2); break;
        }
        my_core.error = ERR_NO_ERROR;
        send_str_to_lan(&text);
        break;
      ////////////////////////////
      default:
        text = ER1;
        my_core.error = ERR_UNKNOWN_CMD;
        send_str_to_lan(&text);

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
      lcd.setCursor(13, 1);
      lcd.print(" ");
      lcd.setCursor(0, 0);
      lcd.print(">");
      break;
    case 2:
      lcd.setCursor(0, 0);
      lcd.print(" ");
      lcd.setCursor(13, 1);
      lcd.print(" ");
      lcd.setCursor(13, 0);
      lcd.print(" ");
      lcd.setCursor(0, 1);
      lcd.print(">");
      break;
    case 3:
      lcd.setCursor(0, 0);
      lcd.print(" ");
      lcd.setCursor(13, 1);
      lcd.print(" ");
      lcd.setCursor(0, 1);
      lcd.print(" ");
      lcd.setCursor(13, 0);
      lcd.print("<");
      break;
    case 4:
      lcd.setCursor(0, 0);
      lcd.print(" ");
      lcd.setCursor(13, 0);
      lcd.print(" ");
      lcd.setCursor(0, 1);
      lcd.print(" ");
      lcd.setCursor(13, 1);
      lcd.print("<");
      break;

  }
}



void config_lan(void) {
  if (digitalRead(BTN1) == HIGH) {             // default settings
    store_data(cur_mac, cur_ip, cur_gateway, cur_subnet);
  }
  recall_data(cur_mac, cur_ip, cur_gateway, cur_subnet);
  server_start(cur_mac, cur_ip, cur_gateway, cur_subnet);

  String text;
  text = String(cur_ip[0]) + "." + String(cur_ip[1]) + "." + String(cur_ip[2]) + "." + String(cur_ip[3]);
  lcd.setCursor(0, 0);
  lcd.print(text);
  text = String(cur_subnet[0]) + "." + String(cur_subnet[1]) + "." + String(cur_subnet[2]) + "." + String(cur_subnet[3]);
  lcd.setCursor(0, 1);
  lcd.print(text);
  delay(2000);
  lcd.clear();
}

void driveRelay(int ch, int pol) {
  switch (ch) {
    case 0:
      digitalWrite(Ar1, LOW);
      digitalWrite(Ar2, LOW);
      digitalWrite(Ar3, LOW);
      digitalWrite(Ar4, LOW);
      digitalWrite(Ar5, LOW);
      break;
    case 1:
      digitalWrite(Ar2, LOW);
      digitalWrite(Ar3, LOW);
      digitalWrite(Ar4, LOW);
      digitalWrite(Ar5, LOW);
      digitalWrite(Ar1, HIGH);
      if (pol == 1) {
        digitalWrite(Ar3, LOW);
      }
      else if (pol == 2) {
        digitalWrite(Ar3, HIGH);
      }
      break;
    case 2:
      digitalWrite(Ar3, LOW);
      digitalWrite(Ar4, LOW);
      digitalWrite(Ar5, LOW);
      digitalWrite(Ar1, LOW);
      digitalWrite(Ar5, HIGH);
      digitalWrite(Ar2, HIGH);
      if (pol == 1) {
        digitalWrite(Ar4, LOW);
      }
      else if (pol == 2) {
        digitalWrite(Ar4, HIGH);
      }
      break;
  }
}

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
