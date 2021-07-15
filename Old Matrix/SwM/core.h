#ifndef core_h
#define core_h
#include "switch_matrix.h"

class core{
  public:
  uchar mode;
  uchar state;
  uchar error;
  uchar flag;
  bool remote_access;
  
  core(void);
  void update(void);
};

#endif