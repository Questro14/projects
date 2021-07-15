#include <Wire.h>
#include "lcd_display.h"
#include <LiquidCrystal_I2C.h>
#include <arduino.h>
#include "buttonz.h"

void lcd_display::init(void){
    LiquidCrystal_I2C::init();
    LiquidCrystal_I2C::backlight();
    LiquidCrystal_I2C::clear();
	  LiquidCrystal_I2C::setCursor(0,0);
	  LiquidCrystal_I2C::print(" SWITCH  MATRIX ");
	  LiquidCrystal_I2C::setCursor(0,1);
	  LiquidCrystal_I2C::print("    ATE-AESA    ");
	  delay(2000);
    LiquidCrystal_I2C::clear();
    }

void lcd_display::clear(void){
    LiquidCrystal_I2C::clear();
}

void lcd_display::update(uchar *mode, uchar *state, uchar *error, bool *remote_access){
	  LiquidCrystal_I2C::setCursor(0,0);
      switch(*mode){
        case modeNorm:
          LiquidCrystal_I2C::print("MODE:NORM");
          break;
        case modeCal1:
          LiquidCrystal_I2C::print("MODE:CAL1");
          break;
        case modeCal2:
          LiquidCrystal_I2C::print("MODE:CAL2");
          break;
        case modeCal3:
          LiquidCrystal_I2C::print("MODE:CAL3");
          break;
        }
      
      setCursor(10,0);
      if(*state < 10) {print("OUT: ");}
      else {LiquidCrystal_I2C::print("OUT:");}
      LiquidCrystal_I2C::print(String(*state));

      LiquidCrystal_I2C::setCursor(0,1);
      LiquidCrystal_I2C::print("ERR:" + String(*error));

      LiquidCrystal_I2C::setCursor(13,1);
      if(*remote_access) {LiquidCrystal_I2C::print("R.A");}
      else {LiquidCrystal_I2C::print("L.A");}
    }

void lcd_display::show_info(String *data, char line){
	LiquidCrystal_I2C::setCursor(0,line);
	//for(char i=0;i<size;i++){
	LiquidCrystal_I2C::print(*data); //data++;
	//}	
}	
