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
      SerialUSB.print("Command is error,Please input: ");
      SerialUSB.println("uart/rs485/can/iic/spi/adc/io1/io2 or exit");
    }
    else
      SerialUSB.println("Stop command.");
    command_flag = false;
    rec_string = "";
  }
}
