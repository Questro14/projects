#ifndef buttonz_h
#define buttonz_h
#include "switch_matrix.h"

class Buttonz{
 
  private:
	unsigned char pin;  // Номер контакта, к которой подключена кнопка
	unsigned long press_time; // Время реакции на нажатие кнопки (антидребег)
	unsigned long time_stamp; // Текущая метка времени
  
  public:
	unsigned char state;
	Buttonz(unsigned char sw_pin,unsigned long sw_delay);
	void init(void);
	void CheckStatus(void);
};

#endif