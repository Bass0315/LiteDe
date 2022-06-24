/*
    library:
    https://github.com/e-radionicacom/Soldered-CAN-Bus-Breakout-MCP2518-Arduino-Library.git
    

*/

#include "Scr.h"


String rec_string = "";
bool command_flag = false;




/**
   @
   @
   @
*/
void setup()
{
  Sys_Init();
}

/**
   @
   @
   @
*/
void loop()
{
  SerialUART_Recive();
  CAN_Recive();
  RS485_Recive();

  if (SerialUSB_Recive())
  {
    SerialUART_Send();
    CAN_Send();
    RS485_Send();
    ADC_AINxx();
    DIOxx();

    if (!command_flag)
    {
      SerialUSB.println("ERROR: Command is error,Please input again.");
      SerialUSB.println("Baord: ");
      SerialUSB.println("uart/rs485/can<address>/adc/io1/io2 or exit.");
      SerialUSB.println(" ");
      SerialUSB.println("Funtion: ");
      SerialUSB.println("ads1115_<0x48/0x49/0x4A/0x4B>");
      SerialUSB.println("tca9548a_scan");
    }
    else
      SerialUSB.println("Stop command.");
    command_flag = false;
    rec_string = "";
  }
}
