#include "arduino.h"
#include <EEPROM.h>
#include "rom_memory.h"

void store_data(unsigned char *mac, unsigned char *ip, unsigned char *gateway, unsigned char *subnet){
	EEPROM.write(MARKER_ADDRESS, MARKER_VALUE);
  EEPROM.write(MAC_ADDRESS+0, *mac++);
  EEPROM.write(MAC_ADDRESS+1, *mac++);
  EEPROM.write(MAC_ADDRESS+2, *mac++);
  EEPROM.write(MAC_ADDRESS+3, *mac++);
  EEPROM.write(MAC_ADDRESS+4, *mac++);
  EEPROM.write(MAC_ADDRESS+5, *mac);
  EEPROM.write(IP_ADDRESS+0, *ip++);
  EEPROM.write(IP_ADDRESS+1, *ip++);
  EEPROM.write(IP_ADDRESS+2, *ip++);
  EEPROM.write(IP_ADDRESS+3, *ip);
  EEPROM.write(GATEWAY_ADDRESS+0, *gateway++);
  EEPROM.write(GATEWAY_ADDRESS+1, *gateway++);
  EEPROM.write(GATEWAY_ADDRESS+2, *gateway++);
  EEPROM.write(GATEWAY_ADDRESS+3, *gateway);
  EEPROM.write(SUBNET_ADDRESS+0, *subnet++);
  EEPROM.write(SUBNET_ADDRESS+1, *subnet++);
  EEPROM.write(SUBNET_ADDRESS+2, *subnet++);
  EEPROM.write(SUBNET_ADDRESS+3, *subnet++);

}

void store_idn(char *idn_string){
  EEPROM.write(IDN_MARKER_ADDRESS, MARKER_VALUE);
  for(int i=0;i<IDN_MAX_LENGHT;i++){
    EEPROM.write(IDN_ADDRESS+i,*idn_string);
    if(*idn_string == 0){return;}
    idn_string++;
  }
}

bool recall_idn(char *idn_string){
char data;
  if(EEPROM.read(IDN_MARKER_ADDRESS) != MARKER_VALUE){
    return false;
  }
  for(int i=0;i<IDN_MAX_LENGHT;i++){
    data = EEPROM.read(IDN_ADDRESS+i);
    *idn_string = data;
    if(data == 0){return true;}
    idn_string++;
  }
  idn_string--;
  *idn_string = 0;
  return true;
}

void recall_data(unsigned char *mac, unsigned char *ip, unsigned char *gateway, unsigned char *subnet){
	if(EEPROM.read(MARKER_ADDRESS) != MARKER_VALUE){
		//return false;
   // Serial.println("false");
	}
	else{
    *mac++ = EEPROM.read(MAC_ADDRESS+0);
    *mac++ = EEPROM.read(MAC_ADDRESS+1);
    *mac++ = EEPROM.read(MAC_ADDRESS+2);
    *mac++ = EEPROM.read(MAC_ADDRESS+3);
    *mac++ = EEPROM.read(MAC_ADDRESS+4);
    *mac = EEPROM.read(MAC_ADDRESS+5);
    *ip++ = EEPROM.read(IP_ADDRESS+0);
    *ip++ = EEPROM.read(IP_ADDRESS+1);
    *ip++ = EEPROM.read(IP_ADDRESS+2);
    *ip = EEPROM.read(IP_ADDRESS+3);
    *gateway++ = EEPROM.read(GATEWAY_ADDRESS+0);
    *gateway++ = EEPROM.read(GATEWAY_ADDRESS+1);
    *gateway++ = EEPROM.read(GATEWAY_ADDRESS+2);
    *gateway = EEPROM.read(GATEWAY_ADDRESS+3);
    *subnet++ = EEPROM.read(SUBNET_ADDRESS+0);
    *subnet++ = EEPROM.read(SUBNET_ADDRESS+1);
    *subnet++ = EEPROM.read(SUBNET_ADDRESS+2);
    *subnet = EEPROM.read(SUBNET_ADDRESS+3);
    //return true;
    //Serial.println("true");
	}
}
