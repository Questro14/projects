#ifndef __LAN__
#define __LAN__

#include <Ethernet.h>

  
#define def_tcp_port 5025

#define buf_size 128

class LAN : public EthernetClass, EthernetServer, EthernetClient{
  public:
  char rx_buf[buf_size];
  unsigned char prx_buf;
  char last_rx_byte, rx_byte;
  unsigned char cur_mac[6] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDA, 0x02 };
  unsigned char cur_ip[4] = { 192, 168, 00, 100 };
  unsigned char cur_subnet[4] = { 255, 255, 255, 0 };
  unsigned char cur_gateway[4] = { 0, 0, 0, 0 };

  EthernetClient client;
  
  LAN(void) : EthernetServer(def_tcp_port), EthernetClient(){};
  void InitLAN(void);
  bool CheckLAN(void);
  bool Connected(void);
  int RxNoB(void);
  void SendData(char *Buf, unsigned char Length);
  void send_str_to_lan(String *data);
  void send_term_char(void);
  void StopLAN(void);

};

#endif
