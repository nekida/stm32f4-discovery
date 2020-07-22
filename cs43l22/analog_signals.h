#ifndef __ANALOG_SIGNAL_H
#define __ANALOG_SIGNAL_H

#include "main.h"
#include "i2s.h"
#include <math.h>

void analog_signal_create(uint32_t frequency);
void analog_signal_generate(void);

#endif /* __ANALOG_SIGNAL_H */
