#ifndef USER_TCA9548A_IICHUB_H
#define USER_TCA9548A_IICHUB_H

#include "TCA9548A.h"
#include <Wire.h>
#define WIRE Wire


void TCA9548A_Init(void);
void ScanAddress(void);
void HubIICScan_Com(void);

#endif
