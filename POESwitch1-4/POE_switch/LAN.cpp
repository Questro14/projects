#include "LAN.h"
#include <Ethernet.h>

void LAN::InitLAN(void){
  EthernetClass::begin(cur_mac, cur_ip, cur_gateway, cur_subnet);
  EthernetServer::begin();
}

bool LAN::CheckLAN(void){
  client = EthernetServer::available();
  if(client){
    if(client.available() > 0){
      last_rx_byte = rx_byte;
      rx_byte = client.read();
      if(rx_byte == 0x08){
        prx_buf = (prx_buf == 0) ? 0 : prx_buf--;
        return false;
      }
      else if(rx_byte == 0x0A){
        if(last_rx_byte == 0x0D){
          prx_buf--;
        }
        rx_buf[prx_buf] = 0x00;
        prx_buf = 0;
        return true;
      }
      else{
        rx_buf[prx_buf] = rx_byte;
        prx_buf++;
        return false;
      }
    }
  }
  return false;
}

bool LAN::Connected(void){
  if(EthernetServer::available()){
    return true;
  }
  else{
    return false;
  }
}
int LAN::RxNoB(void){
  return client.available();
}

void LAN::SendData(char *Buf, unsigned char Length){
  client.write(Buf, Length);
}

void LAN::send_str_to_lan(String *data){
  client.println(*data);
}

void LAN::send_term_char(void){
  char term_char [2] = {0x0D, 0x0A};
  client.write(term_char, 2);
}

void LAN::StopLAN(void){
  client.stop();
}
