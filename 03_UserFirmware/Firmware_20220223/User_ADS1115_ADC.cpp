#include "User_ADS1115_ADC.h"

extern ADS1115<TwoWire> ads;//IIC
extern String rec_string;
extern bool command_flag;

/**
   @
   @
   @
*/
bool ADS1115_Init(uint8_t address)
{
  uint8_t loop = 0;
  while (!ads.begin(address)) {
    delay(100);
    loop++;
    if (loop > 100)
    {
      Serial.print("ads1115 init failed!!!");
      return false;
    }
    else
    {
      Serial.print("ads1115 init OK!!!");
      break;
    }

  }
  //ads.begin(0x49)
  //ads.begin(0x4A)
  //ads.begin(0x4B)

  ads.setOperateMode(ADS1115_MODE_SINGLE);
  ads.setOperateStaus(ADS1115_OS_SINGLE);

  ads.setPGAGain(ADS1115_PGA_6_144);    // 2/3x gain +/- 6.144V  1 bit =  0.1875mV (default)
  // ads.setPGAGain(ADS1115_PGA_4_096); // 1x gain   +/- 4.096V  1 bit =  0.125mV
  // ads.setPGAGain(ADS1115_PGA_2_048); // 2x gain   +/- 2.048V  1 bit =  0.0625mV
  // ads.setPGAGain(ADS1115_PGA_1_024); // 4x gain   +/- 1.024V  1 bit =  0.03125mV
  // ads.setPGAGain(ADS1115_PGA_0_512); // 8x gain   +/- 0.512V  1 bit =  0.015625mV
  // ads.setPGAGain(ADS1115_PGA_0_256); // 16x gain  +/- 0.256V  1 bit =  0.0078125mV

  // ads.setSampleRate(ADS1115_DR_8); //8 SPS
  // ads.setSampleRate(ADS1115_DR_16); //16 SPS
  // ads.setSampleRate(ADS1115_DR_32); //32 SPS
  ads.setSampleRate(ADS1115_DR_64); //64 SPS
  // ads.setSampleRate(ADS1115_DR_128); //128 SPS
  // ads.setSampleRate(ADS1115_DR_250); //250 SPS
  // ads.setSampleRate(ADS1115_DR_475); //475 SPS
  // ads.setSampleRate(ADS1115_DR_860); //860 SPS

  return true;
}


/**
   @
   @
   @
*/
void ADS1115_ChanelValue(void)
{
  char buf[10];
  int16_t adc[4][10] = {0};
  float avg = 0;
  for (uint8_t i = 0; i < 5; i++)
  {
    adc[0][i] = ads.getConversionResults(channel0); //P = AIN0, N = GND
    adc[1][i] = ads.getConversionResults(channel1); //P = AIN1, N = GND
    adc[2][i] = ads.getConversionResults(channel2); //P = AIN2, N = GND
    adc[3][i] = ads.getConversionResults(channel3); //P = AIN3, N = GND
    delay(1);
  }

  for (uint8_t i = 0; i < 4; i++)
  {
    avg = 0;
    for (uint8_t j = 0; j < 5; j++)
    {
      avg += adc[i][j];   //sum
    }
    avg = avg / 5 / 10000.0 * 3.7; //avg

    sprintf(buf, "AIN %d: ", i);
    SerialUSB.print(buf);
    Serial.print(avg);
    Serial.println(" V");
  }
}


/**
   @
   @
   @
*/
/*
  void ADS1115_Com(void)
  {
  if (strcmp(rec_string.c_str(), "ads1115\n") == 0)
  {
    SerialUSB.println("ADC command start.");
    ADS1115_Init(0x49) ? SerialUSB.println("ADS1115 Init OK.") : SerialUSB.println("ADS1115 Init FAILL.");
    ADS1115_ChanelValue();
    command_flag = true;
  }
  }
*/
void ADS1115_Com(void)
{
  uint8_t address = 0x48;
  if (strstr(rec_string.c_str(), "ads1115") != NULL)
  {
    if (strstr(rec_string.c_str(), "0x49") != NULL)address = 0x49;
    if (strstr(rec_string.c_str(), "0x4A") != NULL)address = 0x4A;
    if (strstr(rec_string.c_str(), "0x4B") != NULL)address = 0x4B;
    SerialUSB.println("ADC command start.");
    ADS1115_Init(address) ? SerialUSB.println("ADS1115 Init OK.") : SerialUSB.println("ADS1115 Init FAILL.");
    ADS1115_ChanelValue();
    command_flag = true;
  }
}
