#ifndef hw_interface_h
#define hw_interface_h
#include "switch_matrix.h"

#define FIX_DELAY 1

class hw_interface{
  private:
  uchar pin_data;
  uchar pin_clk;
  uchar pin_nle;

  public:
  hw_interface(uchar data_pin, uchar clk_pin, uchar nle_pin);
  void init(void);
  void tx_data(const unsigned char *data);
};

#endif
