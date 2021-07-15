#include "lan_if.h"
#include <arduino.h>
#include "telemetry_com.h"
#include <SPI.h>
#include <Ethernet.h>


int i;

EthernetServer server(def_tcp_port);
EthernetClient client;

char rx_buf[255];
char prx_buf = 0;
char last_rx_byte, rx_byte;

bool check_lan(void) {

  client = server.available();
  if (client) {
    //Serial.println("Client enable");
    if (client.available() > 0) {
      //Serial.println("Data evailabe");
      last_rx_byte = rx_byte;
      rx_byte = client.read();
      if (rx_byte == 8) {
        prx_buf--;
        return false;
      }
      if (rx_byte == 10) {
        if (last_rx_byte == 13) {
          prx_buf--;

        }
        rx_buf[prx_buf] = 0;
        prx_buf = 0;
        return true;
      }
      else {
        rx_buf[prx_buf] = rx_byte;
        prx_buf++;
        return false;
      }
    }
  }
  return false;
}


void server_stop(void) {
  client.stop();
}


char check_cmd(unsigned char *cmd_id, unsigned int *parameter) {
  prx_buf = 0;
  toUpCase(rx_buf);

  if (compStr(rx_buf, REQ_IDN)) {
    *cmd_id = REQ_IDN_ID;
    return ERR_NO_ERROR;
  }
  ///////////////////
  if (compStr(rx_buf, CMD_IN_POLARITY_POS)) {
    *cmd_id = CMD_IN_POLARITY_POS_ID;
    *parameter = parStr(&rx_buf[13]);
    return ERR_NO_ERROR;
  }
  ///////////////////////////
  if (compStr(rx_buf, CMD_IN_POLARITY_NEG)) {
    *cmd_id = CMD_IN_POLARITY_NEG_ID;
    *parameter = parStr(&rx_buf[13]);
    return ERR_NO_ERROR;
  }
  ///////////////////////////
  if (compStr(rx_buf, REQ_IN_POLARITY)) {
    *cmd_id = REQ_IN_POLARITY_ID;
    return ERR_NO_ERROR;
  }
  /////////////////////////////////
  if (compStr(rx_buf, CMD_IN_SELECT)) {
    *cmd_id = CMD_IN_SELECT_ID;
    *parameter = parStrPulse(&rx_buf[14]);
    return ERR_NO_ERROR;
  }
  //////////////////////////////
  if (compStr(rx_buf, REQ_IN_SELECT)) {
    *cmd_id = REQ_IN_SELECT_ID;
    return ERR_NO_ERROR;
  }
  //////////////////////////
  if (compStr(rx_buf, REQ_SYSTEM_ERROR )) {
    *cmd_id = REQ_SYSTEM_ERROR_ID;
    return ERR_NO_ERROR;
  }

  if (compStr(rx_buf, REQ_SET_MAC_ADDR )) {
    *cmd_id = REQ_SET_MAC_ADDR_ID;
    return ERR_NO_ERROR;
  }
  if (compStr(rx_buf, CMD_SET_IP_ADDR )) {
    *cmd_id = CMD_SET_IP_ADDR_ID;
    for (int w = 0; w < 15; w++) {
      parameter[w] = rx_buf[w + 18];
    }
    return ERR_NO_ERROR;
  }
  ////////////////////////////
  if (compStr(rx_buf, REQ_SET_IP_ADDR )) {
    *cmd_id = REQ_SET_IP_ADDR_ID;
    return ERR_NO_ERROR;
  }
  //////////////////////
  if (compStr(rx_buf, CMD_SET_IP_MASK )) {
    *cmd_id = CMD_SET_IP_MASK_ID;
    for (int w = 0; w < 15; w++) {
      parameter[w] = rx_buf[w + 15];
    }
    return ERR_NO_ERROR;
  }
  /////////////////////////////////
  if (compStr(rx_buf, REQ_SET_IP_MASK )) {
    *cmd_id = REQ_SET_IP_MASK_ID;
    return ERR_NO_ERROR;
  }
  ///////////////////////////////////
  if (compStr(rx_buf, CMD_SET_DEF_GATEWAY )) {
    *cmd_id = CMD_SET_DEF_GATEWAY_ID;
    for (int w = 0; w < 15; w++) {
      parameter[w] = rx_buf[w + 21];
    }
    return ERR_NO_ERROR;
  }
  //////////////////////////
  if (compStr(rx_buf, REQ_SET_DEF_GATEWAY )) {
    *cmd_id = REQ_SET_DEF_GATEWAY_ID;
    return ERR_NO_ERROR;
  }
  if (compStr(rx_buf, CMD_SET_IDN )) {
    *cmd_id = CMD_SET_IDN_ID;
    return ERR_NO_ERROR;
  }
  ////////////////////////////////////////////
  *cmd_id = WRONG_CMD_ID;
  return ERR_UNKNOWN_CMD;
}

/*
  parseIPaddr(char *buf)
  parseIPmask(char *buf)
  parseIPgateway(char *buf)
  parseMAC(char *buf)
*/

bool compStr(char *char1, const char *char2) {
  for (int i = 0; i < 255; i++) {
    switch (*char2) {
      case 0:
        return true;
        break;
      default:
        if (*char1 != *char2) {
          return false;
        }
        break;
    }
    char1++;
    char2++;
  }
}

unsigned char parStr(char *buf) {
  unsigned char numb;
  unsigned char rez = 0;

  for (int i = 0; i < 3; i++) {
    numb = *buf - 48;
    if (numb > 9) {
      return rez;
    }
    else {
      rez = (rez << 1) + (rez << 3)  + numb;
    }
    buf++;
  }
  return rez;
}

unsigned int parStrPulse(char *buf) {
  unsigned char numb;
  unsigned int rez = 0;

  for (int i = 0; i < 3; i++) {
    numb = *buf - 48;
    if (numb > 9) {
      return rez;
    }
    else {
      rez = (rez << 1) + (rez << 3)  + numb;
    }
    buf++;
  }
  return rez;
}

bool parIP(char *buf, char *res) {
  for (int i = 0; i < 3; i++) {
    *res = parStr(buf);
    if (*res < 10) {
      buf++;
    }
    else {
      if (*res < 100) {
        buf = buf + 2;
      }
      else {
        buf = buf + 3;
      }
    }
    if (*buf == 46) {
      buf++;
    }
    else {
      return false;
    }
    res++;
  }
  *res = parStr(buf);
  return true;
}

void toUpCase(char *buf) {
  for (int i = 0; i < 255; i++) {
    if (*buf == 0) {
      return;
    }
    else {
      if ((*buf >= 97) && (*buf <= 122)) {
        *buf -= 32;
      }
    }
    buf++;
  }
}

void buf_null(char *buf) {
  for (int i = 0; i < 255; i++) {
    *buf = 0; buf++;
  }
}


void server_start(unsigned char *mac, unsigned char *ip, unsigned char *gateway, unsigned char *subnet) {
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
}

void send_str_to_lan(String *data) {
  client.println(*data);
}
