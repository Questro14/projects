#include "Dip.h"
#include "Switch.h"


byte addressBoard;

char Data[16];
unsigned char Param[2];
unsigned char NumberSwitch;
unsigned char NumberChannel;

#define secondByteMarker 0x80

#define needCheckRxData 0x01
#define needTxData 0x02
#define needUpdateHardware 0x04
#define needCheckAddress 0x08
#define needLoadData 0x10

byte RxByte;
byte DataByte[2];
unsigned char State;


Switch my_sw;
Dip dip(pin_dip1, pin_dip2, pin_dip3, pin_dip4);

void setup() {
  dip.init();
  my_sw.Init();
  Serial.begin(115200);
  addressBoard = dip.getAddres(); // запись адреса платы
  
}

void loop() {
  /// Receive data from USART
  if (Serial.available() > 0) {
    Serial.readBytes(&RxByte, 1);
    if (RxByte & secondByteMarker) {
      DataByte[1] = RxByte;
      State |= needCheckAddress;
    }
    else {
      DataByte[0] = RxByte;
    }
  }

  /// Transmite data to USART
  else if (State & needTxData) {
    Serial.write(DataByte, 2);
    State ^= needTxData;
  }
  /// Check Board Address
  else if (State & needCheckAddress) {
    if ((DataByte[0] & 0x0F) == addressBoard) {
      State |= needCheckRxData;
    }
    else {
      State |= needTxData;
    }
    State ^= needCheckAddress;
  }
  // Check Pack Data
  else if (State & needCheckRxData) {
    NumberSwitch = (DataByte[1] >> 3) & 0x0F;
    NumberChannel = DataByte[1] & 0x07;
    if (NumberSwitch > MaxSwitch || NumberChannel > my_sw.SwBlock[NumberSwitch].MaxState) {
      DataByte[0] |= 0x20 | 0x10;
      NumberSwitch = 0;
      NumberChannel = 0;
      State |= needTxData;
    }
    else {
      State |= needUpdateHardware;
    }
    State ^= needCheckRxData;
  }
  // Update register
  else if (State & needUpdateHardware) {

    switch(NumberSwitch){
      case 1:
        NumberSwitch = 3;
        break;
      case 2:
        NumberSwitch = 6;
        break;
      case 3:
        NumberSwitch = 9;
        break;
      case 4:
        NumberSwitch = 2;
        break;
      case 5:
        NumberSwitch = 5;
        break;
      case 6:
        NumberSwitch = 8;
        break;
      default:
        break;  
      }
    
    my_sw.SwState[NumberSwitch] = NumberChannel;
    DataByte[0] |= 0x20;
    State |= needTxData | needLoadData;
    State ^= needUpdateHardware;
  }

  // Load date to hardware
  else if (State & needLoadData) {
    my_sw.UpdateSwitch();
    my_sw.SwitchPower(NumberSwitch);
    State ^= needLoadData;
  }
}
