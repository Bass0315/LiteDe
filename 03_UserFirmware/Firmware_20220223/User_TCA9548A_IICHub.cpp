#include "User_TCA9548A_IICHub.h"

extern TCA9548A<TwoWire> TCA;
extern String rec_string;
extern bool command_flag;

uint8_t TCA_CHANNEL[] = {TCA_CHANNEL_0, TCA_CHANNEL_1, TCA_CHANNEL_2, TCA_CHANNEL_3, TCA_CHANNEL_4, TCA_CHANNEL_5, TCA_CHANNEL_6, TCA_CHANNEL_7};

void TCA9548A_Init()
{
  TCA.begin(WIRE);
}


void ScanAddress(void)
{
  uint8_t error, i2cAddress, devCount, unCount;
  for (uint8_t i = 0; i < 8; i++)
  {
    TCA.openChannel(TCA_CHANNEL[i]);
    SerialUSB.print("CHANNEL: ");
    SerialUSB.println(i);

    devCount = 0;
    unCount = 0;
    for (i2cAddress = 1; i2cAddress < 127; i2cAddress++ )
    {
      WIRE.beginTransmission(i2cAddress);
      error = WIRE.endTransmission();

      if (error == 0)
      {
        SerialUSB.print("I2C device found at 0x");
        if (i2cAddress < 16) SerialUSB.print("0");
        SerialUSB.println(i2cAddress, HEX);
        devCount++;
      }
      else if (error == 4)
      {
        SerialUSB.print("Unknow error at 0x");
        if (i2cAddress < 16) SerialUSB.print("0");
        SerialUSB.println(i2cAddress, HEX);
        unCount++;
      }
    }

    if (devCount + unCount == 0)
      SerialUSB.println("No I2C devices found\n");
    else {
      SerialUSB.print(devCount);
      SerialUSB.print(" device(s) found");
      if (unCount > 0) {
        SerialUSB.print(", and unknown error in ");
        SerialUSB.print(unCount);
        SerialUSB.print(" address");
      }
      SerialUSB.println();
    }
    SerialUSB.println();
    delay(1000);
    TCA.closeChannel(TCA_CHANNEL[i]);
  }
}

void HubIICScan_Com(void)
{
  TCA9548A_Init();
  if (strstr(rec_string.c_str(), "tca9548a_scan") != NULL)
  {
    ScanAddress();
    command_flag = true;
  }
}
