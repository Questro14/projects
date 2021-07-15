#ifndef core_h
#define core_h

class core{
  public:
  int channel;
  unsigned int pulse;
  unsigned int error;

  core(void);
  void update(void);
};

#endif
