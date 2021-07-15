#ifndef lcd_display_h
#define lcd_display_h
#include "switch_matrix.h"
#include <LiquidCrystal_I2C.h>

#define DISPLAY_ADDRESS 0x27
#define NUMBER_OF_COLUMNS 16
#define NUMBER_OF_ROWS 2

class lcd_display : private LiquidCrystal_I2C{
public:
  lcd_display(void) : LiquidCrystal_I2C(DISPLAY_ADDRESS,NUMBER_OF_COLUMNS,NUMBER_OF_ROWS){};
  void init(void);
  void update(uchar *mode, uchar *state, uchar *error, bool *remote_access);
  //void show_info(char *data, char line, char size);
  void show_info(String *data, char line);
  void clear(void);
};

#endif
