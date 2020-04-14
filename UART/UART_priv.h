/*
 * UART_priv.h
 *
 *  Created on: 3 Dec 2019
 *      Author: WEST
 */

#ifndef UART_PRIV_H_
#define UART_PRIV_H_

#include "UART_int.h"

#define PEN   1
#define EPS   2
#define STP2  3
#define FEN   4
#define WLEN  5
#define TXIM  5
#define RXIM  4

#define RXFE  4
#define TXRIS 5
#define RXRIS 4

//#define NORMAL_SPEED_MODE 0
//#define DOUBLE_SPEED_MODE 1

#define MaxNumOFmodules 8

typedef struct
{
    UART_e_UARTNumber_t   UART_NUM ;
    u8 * ptrToBuffer ;
    u8   UART_ReceiveRequestFlag;
    u8   UART_TransmitRequestFlag;
    u32   UART_ReceivedDatalength;
    u32   UART_SentDataLength;
    u8 TxFIFOTrigLength;
    u8 RxFIFOTrigLength;
    UART_e_ChannelStatus_t UART_Channel_Status;
    u32  UART_DataLength;
}UART_s_PrivInfo_t;

typedef enum
{
    UART_FIFOAvailable ,
    UART_FIFONotAvailable
}UART_e_FIFOStatus_t;

typedef enum
{
    UART_TxFIFO ,
    UART_RxFIFO
}UART_e_FIFOType_t;

UART_e_Check_t UART_PrivGetFIFOStatus (UART_e_UARTNumber_t copy_u8UARTnum, UART_e_FIFOType_t copy_FIFO_TypeTx_Rx,UART_e_FIFOStatus_t * StatusGetter );
void UART_mainFunc (void);
void UART_vidBaudRateCalc(u8 IndexInConfigArr);
u8 UART_privGetConfigIndix(u8 copy_u8ID) ;
void Priv_UARTInterruptEnable(UART_e_UARTNumber_t ModuleNum );
u8 UART_PrivInterpretTriggerLengthToNums( UART_e_FIFOTriggerLength_t Fifolength);

#define SYSCLOCK 0x00
#define PIOSC 0x5






#endif /* UART_PRIV_H_ */
