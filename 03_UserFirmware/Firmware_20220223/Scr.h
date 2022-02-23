

#ifndef SRC_H
#define SRC_H

#include "Arduino.h"
#include "CANBus-SOLDERED.h"

#define SerialUART Serial1
#define SerialRS485 Serial2

/*  Change according to your setup  */
const uint8_t SPI_CS_PIN = 21;
const uint8_t CAN_INT_PIN = 22;
const uint8_t RS485_ContrlPin = 24;

const uint8_t AIN_20mA = 5;
const uint8_t analogPin_count = 3;
const uint8_t analogPin[analogPin_count] = {3, 4, 5};

const uint8_t dioPin_count = 2;
const uint8_t dioPin[dioPin_count] = {1, 2};

void SerialUSB_Init(void);
void SerialUART_Init(void);
void CAN_Init(void);
void SerialRS485_Init(void);
void DIO_Init(void);
void Sys_Init(void);

bool SerialUSB_Recive(void);
void SerialUART_Send(void);
void SerialUART_Recive(void);

void CAN_Send(void);
void CAN_Recive(void);

void RS485_Send(void);
void RS485_Recive(void);

void ADC_AINxx(void);
void DIOxx(void);
#endif
