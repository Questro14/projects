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
  Serial.begin(2000000);
  addressBoard = dip.getAddres(); // запись адреса платы
}

void loop() {


  if (addressBoard >= 0 && addressBoard <= 6 ) {
    //платы 4-36
    if(addressBoard == 0){
      digitalWrite(12, HIGH);
      delay(100);
      digitalWrite(13, HIGH);
      
      }
    while (1) {
      // Receive data
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
      //Transmite data
      else if (State & needTxData) {
        Serial.write(DataByte, 2);
        State ^= needTxData;
      }
      //Check Address Board
      else if (State & needCheckAddress) {
        if ((DataByte[0] & 0x0F) == addressBoard) {
          State |= needCheckRxData;
        }
        else {
          State |= needTxData;
        }
        State ^= needCheckAddress;
      }
      //Check Pack Data
      else if (State & needCheckRxData) {
        NumberSwitch = (DataByte[1] >> 3) & 0x0F;
        NumberChannel = DataByte[1] & 0x07;
        if (NumberSwitch > MaxSwitch || NumberChannel > my_sw.SwBlock436[NumberSwitch].MaxState) {
          DataByte[0] |= 0x20;
          DataByte[0] |= 0x10;
          NumberSwitch = 0;
          NumberChannel = 0;
          State |= needTxData;
        }
        else {
          State |= needUpdateHardware;
        }
        State ^= needCheckRxData;
      }
      else if (State & needUpdateHardware) {
        my_sw.SwState[NumberSwitch] = NumberChannel;
        DataByte[0] |= 0x20;
        State |= needTxData | needLoadData;
        State ^= needUpdateHardware;
      }
      //Load date to hardware
      else if (State & needLoadData) {
        my_sw.UpdateSwitch436();
        my_sw.SwitchPower(NumberSwitch);
        State ^= needLoadData;
      }
    }
  }

  else if (addressBoard == 8) {
    //SPDT Matrix
    digitalWrite(SPDTpower, HIGH);
    while (1) {
      // Receive data
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
      //Transmite data
      else if (State & needTxData) {
        Serial.write(DataByte, 2);
        State ^= needTxData;
      }
      //Check Address Board
      else if (State & needCheckAddress) {
        if ((DataByte[0] & 0x0F) == addressBoard) {
          State |= needCheckRxData;
        }
        else {
          State |= needTxData;
        }
        State ^= needCheckAddress;
      }
      //Check Pack Data
      else if (State & needCheckRxData) {
        NumberSwitch = (DataByte[1] >> 3) & 0x0F;
        NumberChannel = DataByte[1] & 0x07;
        if (NumberSwitch > MaxSwitchSPDT || NumberChannel > my_sw.SwBlockSPDT[NumberSwitch].MaxState) {
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
      else if (State & needUpdateHardware) {
        switch (NumberSwitch) {
          case 1:
            NumberSwitch = 3;
            break;
          case 2:
            NumberSwitch = 4;
            break;
          case 3:
            NumberSwitch = 1;
            break;
          case 4:
            NumberSwitch = 2;
            break;
          case 5:
            break;
          case 6:
            break;
          default:
            break;
        }
        my_sw.SwState[NumberSwitch] = NumberChannel;
        DataByte[0] |= 0x20;
        State |= needTxData | needLoadData;
        State ^= needUpdateHardware;

      }
      //Load date to hardware
      else if (State & needLoadData) {
        my_sw.UpdateSwitchSPDT();
        State ^= needLoadData;
      }
    }
  }
}
