#include "core.h"
#include <arduino.h>

core::core(void) {
  channel = 0;
  polarity = 1;
  error = 0;
}

void core::update(void) {
  if (channel > 2)
    channel = 0;
  if (channel < 0)
    channel = 2;
  if (polarity > 2)
    polarity = 1;
  if (polarity < 1)
    polarity = 2;


}
