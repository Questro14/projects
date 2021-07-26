#ifndef __ROM_MEMORY__
#define __ROM_MEMORY__

#include <EEPROM.h>
//#include <arduino.h>

#define MARKER_VALUE 0xAA

#define LAN_MARKER_ADDRESS 0
#define MAC_ADDRESS 1
#define IP_ADDRESS 7
#define SUBNET_ADDRESS 11
#define GATEWAY_ADDRESS 15

#define PM_MARKER_ADDRESS 19
#define PM_COEF_A 20
#define PM_COEF_B 24

#define IDN_MARKER_ADDRESS 31
#define IDN_ADDRESS 32
#define IDN_MAX_LENGHT 64
#define IDN_LENGHT_ADDRESS 96


class ROM_Memory : public EEPROMClass {
  public:
  
  ROM_Memory(void) : EEPROMClass(){};
  void Store_LAN(unsigned char *mac, unsigned char *ip, unsigned char *snet, unsigned char *gway);
  bool Recall_LAN(unsigned char *mac, unsigned char *ip, unsigned char *snet, unsigned char *gway);
  void Store_IDN(char *Buf);
  bool Recall_IDN(char *Buf, unsigned char *Length);
};

#endif
