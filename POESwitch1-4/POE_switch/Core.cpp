#include "Core.h"

CoreDataClass::CoreDataClass(CoreData *Data) {
  my_core_data = Data;
}


void CoreDataClass::Init() {
  my_core_data->ErrorNum = initErrorNum;
}

void CoreDataClass::Update(void) {
  if (channel > 3) {
    channel = 0;
  }
}
