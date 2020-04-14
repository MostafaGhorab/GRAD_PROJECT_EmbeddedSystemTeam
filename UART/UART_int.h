/*
 * UART_int.h
 *
 *  Created on: 3 Dec 2019
 *      Author: WEST
 */

#include "std_types.h"
#include "Port.h"
#ifndef UART_INT_H_
#define UART_INT_H_


typedef enum
{
    UART_SemaphoreAvailable,
    UART_SemaphoreNotAvailable
}UART_e_Semaphore_t;

typedef enum
{
 UART0,
 UART1,
 UART2,
 UART3,
 UART4,
 UART5,
 UART6,
 UART7
}UART_e_UARTNumber_t;

typedef enum
{
    UART_okay=0,
    UART_nokay=1,
}UART_e_Check_t;

typedef enum
{
    UART_5BitData = 0,
    UART_6BitData = 1,
    UART_7BitData = 2,
    UART_8BitData = 3

}UART_e_DataLength_t;

typedef enum
{
    UART_ONEStopBit =0,
    UART_TWOStopBit =1
}UART_e_StopBitLength_t;

typedef enum
{
    UART_ParityBitDisabled=0,
    UART_ParityBitEnabled=1
}UART_e_ParityBitStatus_t;

typedef enum
{
    UART_EvenParity=1,
    UART_OddParity=0
}UART_e_Parity_t;

typedef enum
{
    UART_FIFO_Enabled=1,
    UART_FIFO_Disabled=0
}UART_e_FIFOEnDis_t;

typedef enum
{
    UART_2ByteTriggerLength = 0,

    UART_4ByteTriggerLength = 1,

    UART_8ByteTriggerLength = 2,

    UART_12ByteTriggerLength = 3,

    UART_14ByteTriggerLength = 4,

}UART_e_FIFOTriggerLength_t;






typedef enum
{

    UART_Requested=0,
    UART_NotRequested

}UART_e_RequestStatus_t;

typedef enum
{
    UART_IDLE_State,
    UART_SUSPENDED_State,
    UART_Job_Done

}UART_e_ChannelStatus_t;

typedef enum
{
    UART_INT_ENABLED=1,
    UART_INT_DISABLED=0
}UART_INTStatusType;

typedef enum
{
    UART_NORMAL_SPEED_MODE,
    UART_DOUBLE_SPEED_MODE
}UART_e_SpeedMode_t;



typedef struct
{
    UART_e_FIFOTriggerLength_t UART_TX_FIFO_TriggerLength;
    UART_INTStatusType UART_TX_INT_Status;
    void (*UART_TxHandlerPtr)(void) ;
}UART_s_TxInfo_t;

typedef struct
{
    UART_e_FIFOTriggerLength_t UART_RX_FIFO_TriggerLength;
    UART_INTStatusType UART_RX_INT_Status;
    void (*UART_RxHandlerPtr)(void) ;

}UART_s_RxInfo_t;


typedef struct
{
    u32                      BaudRate ;
    UART_e_UARTNumber_t      UARTNum;
    UART_e_DataLength_t      DataLength;
    UART_e_StopBitLength_t   StopLength ;
    UART_e_FIFOEnDis_t       FIFO_Status;
    UART_e_ParityBitStatus_t ParityBit ;    /* Either Enabled or disabled */
    UART_e_Parity_t          Parity_Type ;  /* Either Even parity or odd parity */
    UART_e_SpeedMode_t       Speed_Mode;
    UART_s_TxInfo_t          TransmitterFifo_Config;
    UART_s_RxInfo_t          RecieverFifo_Config;
}UART_s_module_t;


void UART_init(void);

UART_e_Check_t UART_enumRequestToSend (u8 copy_u8UARTID, u32 copy_DataLength , u8 * PtrBuffer);

UART_e_Check_t UART_enumRequestToRecieve (u8 copy_u8UARTID, u32 copy_DataLength , u8 * PtrBuffer);

void UART1_ISR (void);





#endif /* UART_INT_H_ */
