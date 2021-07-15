#include "hw_interface.h"
#include "switch_matrix.h"
#include "arduino.h"

hw_interface::hw_interface(uchar data_pin, uchar clk_pin, uchar nle_pin){
    pin_data = data_pin;
    pin_clk = clk_pin;
    pin_nle = nle_pin;
  }
 
void hw_interface::init(void){
    pinMode(pin_data, OUTPUT);
    pinMode(pin_clk, OUTPUT);
    pinMode(pin_nle, OUTPUT);
    delay(FIX_DELAY);
    digitalWrite(pin_data, LOW);
    digitalWrite(pin_clk, LOW);
    digitalWrite(pin_nle, HIGH);
    delay(FIX_DELAY);
    tx_data(&regState[0][0]);
  }

void hw_interface::tx_data(const unsigned char *data){
//void hw_interface::tx_data(const char *data){
    digitalWrite(pin_nle, LOW);
    delay(FIX_DELAY);
	for(int z=0;z<8;z++){
		uchar mask = 0x80;
		for(int i=0;i<8;i++){
			if(*data&mask){
				digitalWrite(pin_clk, LOW);
				//delay(FIX_DELAY);
				digitalWrite(pin_data, HIGH);
				//delay(FIX_DELAY);
				digitalWrite(pin_clk, HIGH);
				//delay(FIX_DELAY);
			}
			else{
				digitalWrite(pin_clk, LOW);
				//delay(FIX_DELAY);
				digitalWrite(pin_data, LOW);
				//delay(FIX_DELAY);
				digitalWrite(pin_clk, HIGH);
				//delay(FIX_DELAY);
			}
			mask = mask >> 1;
		}
		data++;
	}
	digitalWrite(pin_nle, HIGH);
}
