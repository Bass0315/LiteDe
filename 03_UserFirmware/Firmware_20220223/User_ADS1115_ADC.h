
#ifndef USER_ADS1115_ADC_H
#define USER_ADS1115_ADC_H

#include "ADS1115.h"
#include <Wire.h>

bool ADS1115_Init(uint8_t address);
void ADS1115_ChanelValue(void);
void ADS1115_Com(void);

#endif
