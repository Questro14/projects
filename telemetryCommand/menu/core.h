#ifndef core_h
#define core_h

class core{
  public:
  int channel;
  int polarity;
  int error;

  core(void);
  void update(void);
};

#endif
