#ifndef switch_matrix_h
#define switch_matrix_h

#define modeUp_pin 41
#define modeDown_pin 43
#define stateUp_pin 45
#define stateDown_pin 47
#define pin_timeout 150

#define sw_data_pin 11
#define sw_sclk_pin 12
#define sw_nle_pin 8

#define uchar unsigned char
#define modeNorm 0
#define modeCal1 1
#define modeCal2 2
#define modeCal3 3

#define minStateValue 1
#define maxStateValue 36
#define mode_cntr_mask 0x03

#define ERR_NO_ERROR 0
#define ERR_UNKNOWN_CMD 1

#define BTN_NON_PRESSED 0
#define BTN_PRESSED 1
#define BTN_CHECKED 2
#define BTN_LONG_PRESSED 3

#define mask_core_update 0x01
#define mask_lcd_update 0x02
#define mask_hardware_update 0x04

#define WRONG_CMD_ID 0
#define REQ_IDN_ID 1
#define CMD_OUT_STATE_ID 2
#define REQ_OUT_STATE_ID 3
#define CMD_OUT_MODE_NORM_ID 4
#define CMD_OUT_MODE_CAL1_ID 5
#define CMD_OUT_MODE_CAL2_ID 6
#define CMD_OUT_MODE_CAL3_ID 7
#define REQ_OUT_MODE_ID 8
#define REQ_SYSTEM_ERROR_ID 9
#define CMD_SET_MAC_ADDR_ID 10
#define REQ_SET_MAC_ADDR_ID 11
#define CMD_SET_IP_ADDR_ID 12
#define REQ_SET_IP_ADDR_ID 13
#define CMD_SET_IP_MASK_ID 14
#define REQ_SET_IP_MASK_ID 15
#define CMD_SET_DEF_GATEWAY_ID 16
#define REQ_SET_DEF_GATEWAY_ID 17
#define CMD_SET_IDN_ID 18

const unsigned char regSwitch[37][8] = {{0x48, 0x01, 0x01, 0x40, 0x40, 0x40, 0x40, 0x40},
          {0x48, 0x01, 0x01, 0x40, 0x40, 0x40, 0x40, 0x40},
          {0x48, 0x01, 0x02, 0x40, 0x40, 0x40, 0x40, 0x40},
          {0x48, 0x01, 0x04, 0x40, 0x40, 0x40, 0x40, 0x40},
          {0x48, 0x01, 0x08, 0x40, 0x40, 0x40, 0x40, 0x40},
          {0x48, 0x01, 0x10, 0x40, 0x40, 0x40, 0x40, 0x40},
          {0x48, 0x01, 0x20, 0x40, 0x40, 0x40, 0x40, 0x40},
//
          {0x48, 0x02, 0x40, 0x01, 0x40, 0x40, 0x40, 0x40},
          {0x48, 0x02, 0x40, 0x02, 0x40, 0x40, 0x40, 0x40},
          {0x48, 0x02, 0x40, 0x04, 0x40, 0x40, 0x40, 0x40},
          {0x48, 0x02, 0x40, 0x08, 0x40, 0x40, 0x40, 0x40},
          {0x48, 0x02, 0x40, 0x10, 0x40, 0x40, 0x40, 0x40},
          {0x48, 0x02, 0x40, 0x20, 0x40, 0x40, 0x40, 0x40},
//
          {0x48, 0x20, 0x40, 0x40, 0x01, 0x40, 0x40, 0x40},
          {0x48, 0x20, 0x40, 0x40, 0x02, 0x40, 0x40, 0x40},
          {0x48, 0x20, 0x40, 0x40, 0x04, 0x40, 0x40, 0x40},
          {0x48, 0x20, 0x40, 0x40, 0x08, 0x40, 0x40, 0x40},
          {0x48, 0x20, 0x40, 0x40, 0x10, 0x40, 0x40, 0x40},
          {0x48, 0x20, 0x40, 0x40, 0x20, 0x40, 0x40, 0x40},
//
          {0x48, 0x10, 0x40, 0x40, 0x40, 0x01, 0x40, 0x40},
          {0x48, 0x10, 0x40, 0x40, 0x40, 0x02, 0x40, 0x40},
          {0x48, 0x10, 0x40, 0x40, 0x40, 0x04, 0x40, 0x40},
          {0x48, 0x10, 0x40, 0x40, 0x40, 0x08, 0x40, 0x40},
          {0x48, 0x10, 0x40, 0x40, 0x40, 0x10, 0x40, 0x40},
          {0x48, 0x10, 0x40, 0x40, 0x40, 0x20, 0x40, 0x40},
//
          {0x48, 0x04, 0x40, 0x40, 0x40, 0x40, 0x01, 0x40},
          {0x48, 0x04, 0x40, 0x40, 0x40, 0x40, 0x02, 0x40},
          {0x48, 0x04, 0x40, 0x40, 0x40, 0x40, 0x04, 0x40},
          {0x48, 0x04, 0x40, 0x40, 0x40, 0x40, 0x08, 0x40},
          {0x48, 0x04, 0x40, 0x40, 0x40, 0x40, 0x10, 0x40},
          {0x48, 0x04, 0x40, 0x40, 0x40, 0x40, 0x20, 0x40},
//
          {0x48, 0x08, 0x40, 0x40, 0x40, 0x40, 0x40, 0x01},
          {0x48, 0x08, 0x40, 0x40, 0x40, 0x40, 0x40, 0x02},
          {0x48, 0x08, 0x40, 0x40, 0x40, 0x40, 0x40, 0x04},
          {0x48, 0x08, 0x40, 0x40, 0x40, 0x40, 0x40, 0x08},
          {0x48, 0x08, 0x40, 0x40, 0x40, 0x40, 0x40, 0x10},
          {0x48, 0x08, 0x40, 0x40, 0x40, 0x40, 0x40, 0x20}
          };

const unsigned char regState[4][8] =   {{0x48, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40},
          {0x12, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40},
          {0x21, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40},
          {0x84, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40}
          };

#endif
