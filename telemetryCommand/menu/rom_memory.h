#ifndef rom_memory_h
#define rom_memory_h
#include <EEPROM.h>

#define MARKER_ADDRESS 0
#define MARKER_VALUE 0xAA
#define MAC_ADDRESS 1
#define IP_ADDRESS 7
#define GATEWAY_ADDRESS 11
#define SUBNET_ADDRESS 15
#define IDN_MARKER_ADDRESS 19
#define IDN_ADDRESS 20
#define IDN_MAX_LENGHT 32

void store_data(unsigned char *mac, unsigned char *ip, unsigned char *gateway, unsigned char *subnet);
bool recall_data(unsigned char *mac, unsigned char *ip, unsigned char *gateway, unsigned char *subnet);
void store_idn(char *idn_string);
bool recall_idn(char *idn_string);

#endif
