#ifndef lan_if
#define lan_if
#include <SPI.h>
#include <Ethernet.h>


#define def_tcp_port 5025

const char REQ_IDN[] = "*IDN?";
const char CMD_SET_IDN[] = ":SYSTEM:CONFIG:IDN ";
const char CMD_SET_IP_ADDR[] = ":SYSTEM:IPADDRESS ";
const char REQ_SET_IP_ADDR[] = ":SYSTEM:IPADDRESS?";
const char CMD_SET_IP_MASK[] = ":SYSTEM:IPMASK ";
const char REQ_SET_IP_MASK[] = ":SYSTEM:IPMASK?";
const char CMD_SET_DEF_GATEWAY[] = ":SYSTEM:IPDEFGATEWAY ";
const char REQ_SET_DEF_GATEWAY[] = ":SYSTEM:IPDEFGATEWAY?";
const char CMD_SET_MAC_ADDR[] = ":SYSTEM:MACADDRESS ";
const char REQ_SET_MAC_ADDR[] = ":SYSTEM:MACADDRESS?";
const char CMD_IN_SELECT[] = ":INPUT:SELECT ";
const char REQ_IN_SELECT[] = ":INPUT:SELECT?";
const char REQ_IN_POLARITY[] = ":INPUT:POLARITY?";
const char CMD_IN_POLARITY_POS[] = ":INPUT:POLARITY POS";
const char CMD_IN_POLARITY_NEG[] = ":INPUT:POLARITY NEG";
const char REQ_SYSTEM_ERROR[] = ":SYSTEM:ERROR?";

const char ER0[] = "NoError";
const char ER1[] = "WrongCommand";
const char ER2[] = "WrongParameter";

bool check_lan(void);
void server_start(unsigned char *mac, unsigned char *ip, unsigned char *gateway, unsigned char *subnet);
void server_stop(void);

char check_cmd(unsigned char *cmd_id, unsigned int *parameter);
bool compStr(char *char1, const char *char2);
unsigned char parStr(char *buf);
unsigned int parStrPulse(char *buf);
void toUpCase(char *buf);
bool parIP(char *buf, char *res);
void buf_null(char *buf);
void send_str_to_lan(String *data);

#endif
