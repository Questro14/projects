#ifndef __Core__
#define __Core__

#include <arduino.h>

#define no_error 0
#define wrong_comand 1
#define wrong_parameter 2
#define parameter_out_of_range 3

#define initErrorNum no_error

struct Errorz{
  unsigned char Number;
  String Description;
};


struct CoreData{
  unsigned char Flag;
  unsigned char ErrorNum; 
};

class CoreDataClass{
  public:
  int channel = 0;
  int chSend = 2;
  CoreData *my_core_data;
  CoreDataClass(CoreData *Data);
  void Init(void);
  void Update(void);
};



#endif
