#include "ROM_Memory.h"
//#include <EEPROM.h>
#include <arduino.h>

void ROM_Memory::Store_LAN(unsigned char *mac, unsigned char *ip, unsigned char *snet, unsigned char *gway){
  EEPROMClass::write(LAN_MARKER_ADDRESS, MARKER_VALUE);
  EEPROMClass::write(MAC_ADDRESS + 0, *mac++);
  EEPROMClass::write(MAC_ADDRESS + 1, *mac++);
  EEPROMClass::write(MAC_ADDRESS + 2, *mac++);
  EEPROMClass::write(MAC_ADDRESS + 3, *mac++);
  EEPROMClass::write(MAC_ADDRESS + 4, *mac++);
  EEPROMClass::write(MAC_ADDRESS + 5, *mac++);
  EEPROMClass::write(IP_ADDRESS + 0, *ip++);
  EEPROMClass::write(IP_ADDRESS + 1, *ip++);
  EEPROMClass::write(IP_ADDRESS + 2, *ip++);
  EEPROMClass::write(IP_ADDRESS + 3, *ip++);
  EEPROMClass::write(SUBNET_ADDRESS + 0, *snet++);
  EEPROMClass::write(SUBNET_ADDRESS + 1, *snet++);
  EEPROMClass::write(SUBNET_ADDRESS + 2, *snet++);
  EEPROMClass::write(SUBNET_ADDRESS + 3, *snet++);
  EEPROMClass::write(GATEWAY_ADDRESS + 0, *gway++);
  EEPROMClass::write(GATEWAY_ADDRESS + 1, *gway++);
  EEPROMClass::write(GATEWAY_ADDRESS + 2, *gway++);
  EEPROMClass::write(GATEWAY_ADDRESS + 3, *gway++);
}

bool ROM_Memory::Recall_LAN(unsigned char *mac, unsigned char *ip, unsigned char *snet, unsigned char *gway){
  if(EEPROMClass::read(LAN_MARKER_ADDRESS) == MARKER_VALUE){
    *mac++ = EEPROMClass::read(MAC_ADDRESS + 0);
    *mac++ = EEPROMClass::read(MAC_ADDRESS + 1);
    *mac++ = EEPROMClass::read(MAC_ADDRESS + 2);
    *mac++ = EEPROMClass::read(MAC_ADDRESS + 3);
    *mac++ = EEPROMClass::read(MAC_ADDRESS + 4);
    *mac++ = EEPROMClass::read(MAC_ADDRESS + 5);
    *ip++ =  EEPROMClass::read(IP_ADDRESS + 0);
    *ip++ =  EEPROMClass::read(IP_ADDRESS + 1);
    *ip++ =  EEPROMClass::read(IP_ADDRESS + 2);
    *ip++ =  EEPROMClass::read(IP_ADDRESS + 3);
    *snet++ = EEPROMClass::read(SUBNET_ADDRESS + 0);
    *snet++ = EEPROMClass::read(SUBNET_ADDRESS + 1);
    *snet++ = EEPROMClass::read(SUBNET_ADDRESS + 2);
    *snet++ = EEPROMClass::read(SUBNET_ADDRESS + 3);
    *gway++ = EEPROMClass::read(GATEWAY_ADDRESS + 0);
    *gway++ = EEPROMClass::read(GATEWAY_ADDRESS + 1);
    *gway++ = EEPROMClass::read(GATEWAY_ADDRESS + 2);
    *gway++ = EEPROMClass::read(GATEWAY_ADDRESS + 3);
    return true;
  }
  else{
    return false;
  }
}

void ROM_Memory::Store_PM_Coef(float *Coef){
  byte *pntr = (byte *)Coef;
  EEPROMClass::write(PM_MARKER_ADDRESS, MARKER_VALUE);
  EEPROMClass::write(PM_COEF_A + 0, *pntr++);
  EEPROMClass::write(PM_COEF_A + 1, *pntr++);
  EEPROMClass::write(PM_COEF_A + 2, *pntr++);
  EEPROMClass::write(PM_COEF_A + 3, *pntr++);
  Coef++;
  pntr = (byte *)Coef;
  EEPROMClass::write(PM_COEF_B + 0, *pntr++);
  EEPROMClass::write(PM_COEF_B + 1, *pntr++);
  EEPROMClass::write(PM_COEF_B + 2, *pntr++);
  EEPROMClass::write(PM_COEF_B + 3, *pntr++);
}

bool ROM_Memory::Recall_PM_Coef(float *Coef){
  byte data[4];
  byte *pntr = data;
  float *numb;
  if(EEPROMClass::read(PM_MARKER_ADDRESS) == MARKER_VALUE){
    *pntr++ = EEPROMClass::read(PM_COEF_A + 0);
    *pntr++ = EEPROMClass::read(PM_COEF_A + 1);
    *pntr++ = EEPROMClass::read(PM_COEF_A + 2);
    *pntr++ = EEPROMClass::read(PM_COEF_A + 3);
    numb = (float *)data;
    *Coef = *numb;
    Coef++;
    pntr = data;
    *pntr++ = EEPROMClass::read(PM_COEF_B + 0);
    *pntr++ = EEPROMClass::read(PM_COEF_B + 1);
    *pntr++ = EEPROMClass::read(PM_COEF_B + 2);
    *pntr++ = EEPROMClass::read(PM_COEF_B + 3);
    numb = (float *)data;
    *Coef = *numb;
    return true;
  }
  else{
    return false;
  }
}

void ROM_Memory::Store_IDN(char *Buf){
  char Length = 0;
  EEPROMClass::write(IDN_MARKER_ADDRESS, MARKER_VALUE);
  for(char i=0; i < IDN_MAX_LENGHT; i++){
    if(*Buf == 0){
      break;
    }
    Length++;
    EEPROMClass::write(IDN_ADDRESS + i, *Buf);
    Buf++;
  }
  if(Length == IDN_MAX_LENGHT){
    EEPROMClass::write(IDN_ADDRESS + IDN_MAX_LENGHT - 1, 0);
  }
  EEPROMClass::write(IDN_LENGHT_ADDRESS, Length);
}

bool ROM_Memory::Recall_IDN(char *Buf, unsigned char *Length){
  if(EEPROMClass::read(IDN_MARKER_ADDRESS) == MARKER_VALUE){
    *Length = EEPROMClass::read(IDN_LENGHT_ADDRESS);
    for (char i = 0; i < *Length; i++){
      *Buf++ = EEPROMClass::read(IDN_ADDRESS + i);
    }
    return true;
  }
  else{
    return false;
  }
}
