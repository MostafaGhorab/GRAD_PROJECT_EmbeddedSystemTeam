/*
 * UART_config.c
 *
 *  Created on: 3 Dec 2019
 *      Author: WEST
 */
#include "UART_priv.h"
#include "UART_int.h"
#include "UART_Config.h"

void HOSSAM_KHIARY (void)
{

}

UART_s_module_t UART_ConfigArray [NUMBER_OF_MODULES] =

{
 {9600 ,
  UART1,
  UART_8BitData ,
  UART_ONEStopBit ,
  UART_FIFO_Enabled,
  UART_ParityBitDisabled ,
  UART_EvenParity ,
  UART_NORMAL_SPEED_MODE,
  { UART_2ByteTriggerLength,UART_INT_ENABLED,HOSSAM_KHIARY },
  { UART_4ByteTriggerLength,UART_INT_ENABLED,HOSSAM_KHIARY } ,
 }
};
