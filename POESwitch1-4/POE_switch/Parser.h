#ifndef __PARSER__
#define __PARSER__

#include <arduino.h>

const PROGMEM char REQ_IDN[] = "*IDN?";
const PROGMEM char CMD_RST[] = "*RST";
const PROGMEM char REQ_OPC[] = "*OPC?";
const PROGMEM char CMD_STATE_SWITCH1[] = "STATE:SWITCH1,2";
const PROGMEM char CMD_STATE_SWITCH2[] = "STATE:SWITCH1,3";
const PROGMEM char CMD_STATE_SWITCH3[] = "STATE:SWITCH1,5";
const PROGMEM char CMD_STATE_SWITCH4[] = "STATE:SWITCH1,6";
const PROGMEM char REQ_STATE_SWITCH[] = "STATE:SWITCH1?";
const PROGMEM char CMD_SET_MAC_ADDR[] = "SYSTEM:CONFIG:MACADDRESS ";
const PROGMEM char REQ_SET_MAC_ADDR[] = "SYSTEM:CONFIG:MACADDRESS?";
const PROGMEM char CMD_SET_IP_ADDR[] = "SYSTEM:CONFIG:IPADDRESS ";
const PROGMEM char REQ_SET_IP_ADDR[] = "SYSTEM:CONFIG:IPADDRESS?";
const PROGMEM char CMD_SET_IP_MASK[] = "SYSTEM:CONFIG:IPMASK ";
const PROGMEM char REQ_SET_IP_MASK[] = "SYSTEM:CONFIG:IPMASK?";
const PROGMEM char CMD_SET_DEF_GATEWAY[] = "SYSTEM:CONFIG:IPDEFGATEWAY ";
const PROGMEM char REQ_SET_DEF_GATEWAY[] = "SYSTEM:CONFIG:IPDEFGATEWAY?";
const PROGMEM char CMD_SET_IDN[] = "SYSTEM:CONFIG:IDN ";
const PROGMEM char REQ_SYSTEM_ERROR[] = "SYSTEM:ERROR?";

class parser{
  public:
  bool compStr(char *Str1, const char *Str2);
  void toUpCase(char *Buf);
  bool parStr(char *Buf, unsigned char *Rez);
  bool parseMAC(char *Buf, unsigned char *MAC);
  bool parseIP(char *Buf, unsigned char *IP);
  bool parsePM(char *Buf, float *Coef);
  void bufNull(char *Buf, unsigned char Length);
};

#endif
