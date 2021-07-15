#include "core.h"
#include <arduino.h>
#include "formingCommands.h"

core::core(void) {
  channel = 0;
  pulse = 30;
  error = ERR_NO_ERROR;
}

void core::update(void) {
  if (channel > 4)
    channel = 0;
  if (channel < 0)
    channel = 4;
  if (pulse > 200)
    pulse = 30;
  if (pulse < 30)
    pulse = 200;

}
