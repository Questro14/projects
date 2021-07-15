#ifndef lan_if
#define lan_if
#include <SPI.h>
#include <Ethernet.h>
#include "switch_matrix.h"

#define def_tcp_port 5025

const char REQ_IDN[] = "*IDN?";
const char CMD_OUT_STATE[] = "OUTPUT:STATE ";
const char REQ_OUT_STATE[] = "OUTPUT:STATE?";
const char CMD_OUT_MODE_NORM[] = "OUTPUT:MODE NORMAL";
const char CMD_OUT_MODE_CAL1[] = "OUTPUT:MODE CAL1";
const char CMD_OUT_MODE_CAL2[] = "OUTPUT:MODE CAL2";
const char CMD_OUT_MODE_CAL3[] = "OUTPUT:MODE CAL3";
const char REQ_OUT_MODE[] = "OUTPUT:MODE?";
const char REQ_SYSTEM_ERROR[] = "SYSTEM:ERROR?";
const char CMD_SET_MAC_ADDR[] = "SYSTEM:MACADDRESS ";
const char REQ_SET_MAC_ADDR[] = "SYSTEM:MACADDRESS?";
const char CMD_SET_IP_ADDR[] = "SYSTEM:IPADDRESS ";
const char REQ_SET_IP_ADDR[] = "SYSTEM:IPADDRESS?";
const char CMD_SET_IP_MASK[] = "SYSTEM:IPMASK ";
const char REQ_SET_IP_MASK[] = "SYSTEM:IPMASK?";
const char CMD_SET_DEF_GATEWAY[] = "SYSTEM:IPDEFGATEWAY ";
const char REQ_SET_DEF_GATEWAY[] = "SYSTEM:IPDEFGATEWAY?";
const char CMD_SET_IDN[] = "SYSTEM:CONFIG:IDN ";

bool check_lan(void);
void server_start(unsigned char *mac, unsigned char *ip, unsigned char *gateway, unsigned char *subnet);
void server_stop(void);

char check_cmd(unsigned char *cmd_id, unsigned char *parameter);
bool compStr(char *char1, const char *char2);
unsigned char parStr(char *buf);
void toUpCase(char *buf);
bool parIP(char *buf, char *res);
void buf_null(char *buf);
void send_str_to_lan(String *data);

#endif
