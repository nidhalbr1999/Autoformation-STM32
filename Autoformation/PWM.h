#include "stm32f401xe.h"

void GPIOinit(void);
void PWMConfig(void);

long map(long x, long in_min, long in_max, long out_min, long out_max);

void servo_write(int angle);

