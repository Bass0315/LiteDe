#include "Scr.h"

CANBus CAN(SPI_CS_PIN);     // Set CS pin

extern String rec_string;


//===============================================================================================
//                                          Init
//===============================================================================================
/**
   @
   @
   @
*/
void SerialUSB_Init(void)
{
  SerialUSB.begin(115200);  // setup serial
}

/**
   @
   @
   @
*/
void SerialUART_Init(void)
{
  SerialUART.begin(115200); // setup serial
}

/**
   @
   @
   @
*/
void SerialRS485_Init(void)
{
  //Contrl pin init
  pinMode(RS485_ContrlPin, OUTPUT);
  digitalWrite(RS485_ContrlPin, LOW);

  SerialRS485.begin(115200); // setup serial
}

/**
   @
   @
   @
*/
void CAN_Init(void)
{
  while (CAN_OK != CAN.begin(CAN_500KBPS))  // init can bus : baudrate = 500k
  {
    Serial.println("CAN init fail, retry...");
    delay(100);
  }
  Serial.println("CAN init ok!");
}

/**
   @
   @
   @
*/
void DIO_Init(void)
{
  for (uint8_t i = 0; i < dioPin_count; i++)
  {
    pinMode(dioPin[i], OUTPUT);
    digitalWrite(dioPin[i], LOW);
  }
}

/**
   @
   @
   @
*/
void Sys_Init(void)
{
  SerialUSB_Init();
  SerialUART_Init();
  CAN_Init();
  SerialRS485_Init();
  DIO_Init();
}




//===============================================================================================
//                                          UART
//===============================================================================================
/**
   @
   @
   @
*/
bool SerialUSB_Recive(void)
{
  rec_string = "";

  while (SerialUSB.available() > 0)
  {
    rec_string += char(SerialUSB.read());
    delay(2);
  }
  //return
  if (rec_string.length() > 0)
    return true;
  else
    return false;
}


bool loop_flag = false;
extern bool command_flag;

/**
   @
   @
   @
*/
void SerialUART_Send(void)  //!!!! (last "1" is one) !!!!!
{
  if (strcmp(rec_string.c_str(), "uart\n") == 0)
  {
    SerialUSB.println("UART command start.");
    loop_flag = true;
    while (loop_flag)
    {
      SerialUART_Recive();
      CAN_Recive();
      RS485_Recive();

      if (SerialUSB_Recive())
      {
        if (strcmp(rec_string.c_str(), "exit\n") == 0)
        {
          loop_flag = false;
        }
        else
        {
          SerialUSB.println(rec_string);
          SerialUART.println(rec_string);
        }
      }
    }
    command_flag = true;
  }
}

/**
   @
   @
   @
*/
void SerialUART_Recive(void)
{
  rec_string = "";

  while (SerialUART.available() > 0)
  {
    rec_string += char(SerialUART.read());
    delay(2);
  }

  //return
  if (rec_string.length() > 0)
  {
    SerialUSB.print("UART data:");
    SerialUSB.println(rec_string);
  }
}


//===============================================================================================
//                                          CAN
//===============================================================================================
/**
   @
   @
   @
*/
void CAN_Send(void)
{
  char buf[] = {0};
  char temp[20] = {0};
  uint8_t address = 0;
  uint8_t string_len = 0;

  //Get CAN command
  strcpy(temp, rec_string.c_str());  //Get "can" and save it to temporary array.
  memset(temp + 3, NULL, sizeof(temp) - 3);
  if (strcmp(temp, "can") == 0)
  {
    memset(temp, NULL, sizeof(temp)); //clear temp array

    //Get address
    strcpy(temp, rec_string.c_str());
    if (temp[3] != 10)
    {
      if ((temp[3] - 48) * 100 + (temp[4] - 48) * 10 + (temp[5] - 48) < 0xff)
        address = (temp[3] - 48) * 100 + (temp[4] - 48) * 10 + (temp[5] - 48);
      else
      {
        SerialUSB.println("Address is error,Please restart.");
        command_flag = true;
        return;
      }
      SerialUSB.print("address:0x");
      SerialUSB.println(address, HEX);
    }
    else
    {
      SerialUSB.println("canxxx :xxx is address,Please restart.");
      command_flag = true;
      return;
    }

    //Start CAN send
    SerialUSB.println("CAN data start.");
    loop_flag = true;
    while (loop_flag)
    {
      SerialUART_Recive();
      CAN_Recive();
      RS485_Recive();

      if (SerialUSB_Recive())
      {
        string_len = rec_string.length();
        if (string_len - 1 < 9)
        {
          strcpy(buf, rec_string.c_str());
          SerialUSB.println(buf);
          if (strcmp(rec_string.c_str(), "exit\n") == 0)
            loop_flag = false;
          else
            CAN.sendMsgBuf(address, 1, string_len, (unsigned char*)buf); //数组
        }
        else
          SerialUSB.println("Please send 8 byte data.");
        memset(buf, NULL, strlen(buf));
      }
    }
    command_flag = true;
  }
}



/**
   @
   @
   @
*/
void CAN_Recive(void)
{
  uint8_t len = 0;
  uint8_t buf[8];

  if (CAN_MSGAVAIL == CAN.checkReceive()) {         // check if data coming
    CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf

    unsigned long canId = CAN.getCanId();

    SerialUSB.print("CAN data from ID: 0x");
    SerialUSB.println(canId, HEX);

    for (int i = 0; i < len; i++) { // print the data
      SerialUSB.print(buf[i], HEX);
      SerialUSB.print("\t");
    }
    SerialUSB.println();
  }
}



//===============================================================================================
//                                          RS485
//===============================================================================================
/**
   @
   @
   @
*/
void RS485_Send(void)
{
  if (strcmp(rec_string.c_str(), "rs485\n") == 0)
  {
    SerialUSB.println("RS485 command start.");
    loop_flag = true;
    while (loop_flag)
    {
      SerialUART_Recive();
      CAN_Recive();
      RS485_Recive();

      if (SerialUSB_Recive())
      {
        if (strcmp(rec_string.c_str(), "exit\n") == 0)
        {
          loop_flag = false;
        }
        else
        {
          digitalWrite(RS485_ContrlPin, HIGH);
          SerialUSB.println(rec_string);
          SerialRS485.println(rec_string);
          digitalWrite(RS485_ContrlPin, LOW);
        }
      }
    }
    command_flag = true;
  }
}


/**
   @
   @
   @
*/
void RS485_Recive(void)
{
  rec_string = "";

  while (SerialRS485.available() > 0)
  {
    rec_string += char(SerialRS485.read());
    delay(1);
  }

  //return
  if (rec_string.length() > 0)
  {
    SerialUSB.print("RS485 data:");
    SerialUSB.println(rec_string);
  }
}


//===============================================================================================
//                                          ADC
//===============================================================================================
/**
   @
   @
   @
*/
void ADC_AINxx(void)
{
  int val = 0;
  char buf[20];
  float analogValue = 0;
  if (strcmp(rec_string.c_str(), "adc\n") == 0)
  {
    SerialUSB.println("ADC command start.");
    for (unsigned char i = 0; i < analogPin_count; i++)
    {

      for (unsigned char j = 0; j < 5; j++) //if j=10,
      {
        val += analogRead(analogPin[i]);
      }
      if (analogPin[i] == AIN_20mA)
      {
        analogValue = val / 5 / 4096.0 * 3.3;
        SerialUSB.print("AIN_20mA: ");
      }
      else
      {
        analogValue = val / 5 / 4096.0 * 3.3 * (11300 + 3300) / 3300;
        sprintf(buf, "AIN%d: ", i + 1);
        SerialUSB.print(buf);
      }

      SerialUSB.println(analogValue);
      val = 0;
      memset(buf, NULL, strlen(buf));
    }
    command_flag = true;
  }
}


//===============================================================================================
//                                          DIO
//===============================================================================================
/**
   @
   @
   @
*/
bool gb_DIOFlag1, gb_DIOFlag2;
void DIOxx(void)
{
  char temp[10] = {0};
  //Get DIO command
  strcpy(temp, rec_string.c_str());  //Get "can" and save it to temporary array.
  memset(temp + 2, NULL, sizeof(temp) - 2);
  if (strcmp(temp, "io") == 0)
  {
    SerialUSB.println("DIO command start.");
    strcpy(temp, rec_string.c_str());
    //SerialUSB.println(temp[2]);   //DEBUG
    switch (temp[2])
    {
      case '1':
        {
          gb_DIOFlag1 = !gb_DIOFlag1;
          digitalWrite(dioPin[0], gb_DIOFlag1);
          break;
        }
      case '2':
        {
          gb_DIOFlag2 = !gb_DIOFlag2;
          digitalWrite(dioPin[1], gb_DIOFlag2);
          break;
        }
    }
    command_flag = true;
  }
}
