

/**
 * main.c
 */
#include "std_types.h"
#include "UART_int.h"
#include "UART_priv.h"
#include "UART_Config.h"
#include "UART_MemMap.h"
#include "utils.h"
#include "GPIO_MemMap.h"

extern UART_s_module_t UART_ConfigArray [NUMBER_OF_MODULES];
UART_s_PrivInfo_t      UART_PrivInfoArr [NUMBER_OF_MODULES];

void(*(TX_Ptrtofuncarr[MaxNumOFmodules]))(void)={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};

void(*(RX_Ptrtofuncarr[MaxNumOFmodules]))(void)={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};

#define RxFIFOTriggerLenShift 3
static const u8 UARTInterruptNumbers[MaxNumOFmodules] = { 5, 6, 33 , 59, 60, 61, 62, 63 };
UART_e_Semaphore_t UART_Semaphore;
static u8 UART_ChannelIndex = 0;
static u32 RemainingReceiveBytes = 0;
void  UART_init(void)
{


    u8 i=0;
    UART_e_UARTNumber_t UART_Num;
    for (i=0; i<NUMBER_OF_MODULES; i++)
    {
        UART_Num = UART_ConfigArray[i].UARTNum;
        UART_PrivInfoArr[i].UART_NUM = UART_Num;
        RCGCUART |= (1<<UART_Num);                                    /*enable UART peripheral clock register*/
        CLR_BIT(UARTCTL(UART_Num),0);                                 /*disable UARTn*/
        UART_vidBaudRateCalc(i);                                      /*i is the index of the config array*/
        UARTLCRH(UART_Num)|=(UART_ConfigArray[i].ParityBit)<<PEN;     /*either Enabled or disabled parity*/
        UARTLCRH(UART_Num)|=(UART_ConfigArray[i].Parity_Type)<<EPS;   /*0 for odd parity and one for even parity*/
        UARTLCRH(UART_Num)|=(UART_ConfigArray[i].StopLength)<<STP2;   /*0 for one stop bit and one for two stop bits */
        UARTLCRH(UART_Num)|=(UART_ConfigArray[i].FIFO_Status)<<FEN;
        UARTLCRH(UART_Num)|=(UART_ConfigArray[i].DataLength)<<WLEN;
        UARTCC(UART_Num)|=CLOCK_SOURCE;                               /* Either PIOSC (16 Mhz) or system clock */
        UARTCTL(UART_Num)|= UART_ConfigArray[i].Speed_Mode;
        if((UART_ConfigArray[i].TransmitterFifo_Config.UART_TX_INT_Status)==UART_INT_ENABLED)
        {
            UART_PrivInfoArr[i].TxFIFOTrigLength = UART_PrivInterpretTriggerLengthToNums( UART_ConfigArray[i].TransmitterFifo_Config.UART_TX_FIFO_TriggerLength );
            Priv_UARTInterruptEnable(UART_Num);
            UARTIFLS(UART_Num) = UART_ConfigArray[i].TransmitterFifo_Config.UART_TX_FIFO_TriggerLength;  /*selecting the fifo length*/
            TX_Ptrtofuncarr[UART_Num] = UART_ConfigArray[i].TransmitterFifo_Config.UART_TxHandlerPtr ;

        }
        if((UART_ConfigArray[i].RecieverFifo_Config.UART_RX_INT_Status)==UART_INT_ENABLED)
        {
            UART_PrivInfoArr[i].RxFIFOTrigLength =  UART_PrivInterpretTriggerLengthToNums( UART_ConfigArray[i].RecieverFifo_Config.UART_RX_FIFO_TriggerLength );
            Priv_UARTInterruptEnable(UART_Num);
            UARTIFLS(UART_Num) = (( UART_ConfigArray[i].RecieverFifo_Config.UART_RX_FIFO_TriggerLength)<<RxFIFOTriggerLenShift);
            RX_Ptrtofuncarr[UART_Num] = UART_ConfigArray[i].RecieverFifo_Config.UART_RxHandlerPtr ;
        }

        SET_BIT(UARTCTL(UART_Num),0); //Enable UARTn
    }
}

/*======================================================Request To Send=============================================================*/

UART_e_Check_t UART_enumRequestToSend (u8 copy_u8UARTID, u32 copy_DataLength , u8 * PtrBuffer)
{
    UART_e_Check_t ErrorFunctionValidation = UART_okay ;
    u8 LoopIndex ;
    if (0!=copy_DataLength)
    {


        for (LoopIndex =0; LoopIndex<NUMBER_OF_MODULES;LoopIndex++)
        {

            if ( copy_u8UARTID == UART_ConfigArray[LoopIndex].UARTNum)
            {
                if ( UART_PrivInfoArr[LoopIndex].UART_Channel_Status == UART_IDLE_State )
                {
                    UART_Semaphore = UART_SemaphoreNotAvailable;
                    UARTIM(copy_u8UARTID)|=(UART_INT_ENABLED<<TXIM);
                    UART_PrivInfoArr[LoopIndex].UART_DataLength = copy_DataLength;
                    UART_PrivInfoArr[LoopIndex].ptrToBuffer = PtrBuffer;
                    UART_PrivInfoArr[LoopIndex].UART_TransmitRequestFlag = 1;
                    UART_PrivInfoArr[LoopIndex].UART_Channel_Status  = UART_SUSPENDED_State ;
                    UART_Semaphore = UART_SemaphoreAvailable;
                    break;
                }

            }

        }

        if (LoopIndex == NUMBER_OF_MODULES )    //no break operation happened
        {
            ErrorFunctionValidation = UART_nokay ;
        }
    }
    else
    {
        ErrorFunctionValidation=UART_nokay;
    }
    return ErrorFunctionValidation ;
}

/*======================================================Request To Receive =============================================================*/
UART_e_Check_t UART_enumRequestToRecieve (u8 copy_u8UARTID, u32 copy_DataLength , u8 * PtrBuffer)
{

    UART_e_Check_t ErrorFunctionValidation = UART_okay ;
    u8 LoopIndex ;
    if (0!=copy_DataLength)
    {


        for (LoopIndex =0; LoopIndex<NUMBER_OF_MODULES;LoopIndex++)
        {

            if ( copy_u8UARTID == UART_ConfigArray[LoopIndex].UARTNum)
            {
                if ( UART_PrivInfoArr[LoopIndex].UART_Channel_Status == UART_IDLE_State )
                {
                    UART_Semaphore = UART_SemaphoreNotAvailable;
                    UARTIM(copy_u8UARTID)|=(UART_INT_ENABLED<<RXIM);
                    UART_PrivInfoArr[LoopIndex].UART_DataLength = copy_DataLength;
                    RemainingReceiveBytes = copy_DataLength;
                    UART_PrivInfoArr[LoopIndex].ptrToBuffer = PtrBuffer;
                    UART_PrivInfoArr[LoopIndex].UART_ReceiveRequestFlag = 1;
                    UART_PrivInfoArr[LoopIndex].UART_Channel_Status  = UART_SUSPENDED_State ;
                    UART_Semaphore = UART_SemaphoreAvailable;
                    break;
                }

            }

        }

        if (LoopIndex == NUMBER_OF_MODULES )    //no break operation happened
        {
            ErrorFunctionValidation = UART_nokay ;
        }
    }
    else
    {
        ErrorFunctionValidation=UART_nokay;
    }
    return ErrorFunctionValidation ;
}

/*======================================================Get FIFO Status func=============================================================*/

UART_e_Check_t UART_PrivGetFIFOStatus (UART_e_UARTNumber_t copy_u8UARTnum, UART_e_FIFOType_t copy_FIFO_TypeTx_Rx,UART_e_FIFOStatus_t * StatusGetter )
{
    UART_e_Check_t ErrorFuncValid = UART_okay ;

    if (copy_u8UARTnum > NUMBER_OF_MODULES)
    {
        ErrorFuncValid = UART_nokay;
    }
    * StatusGetter= UART_FIFOAvailable ;

    if (copy_FIFO_TypeTx_Rx == UART_TxFIFO )
    {
        if (GET_BIT(UARTFR(copy_u8UARTnum),5)!=0) //if the TxFIFO is  full
        {
            *StatusGetter = UART_FIFONotAvailable ;
        }
    }

    else if (copy_FIFO_TypeTx_Rx == UART_RxFIFO )
    {
        if (GET_BIT(UARTFR(copy_u8UARTnum),4)!=0) //if the RxFIFO is  Empty
        {
            *StatusGetter = UART_FIFONotAvailable ;
        }
    }
    return ErrorFuncValid ;
}
/*======================================================  Main Function ================================================================*/
void UART_mainFunc (void)
{

    u8 LoopIndex;
    UART_e_UARTNumber_t UART_ID ;
    UART_e_ChannelStatus_t Local_ChannelStatus;
    if ( UART_Semaphore == UART_SemaphoreAvailable )
    {
        for (LoopIndex = 0 ; LoopIndex<NUMBER_OF_MODULES ; LoopIndex ++ )
        {
            Local_ChannelStatus = UART_PrivInfoArr[LoopIndex].UART_Channel_Status;
            switch ( Local_ChannelStatus )
            {
            case UART_SUSPENDED_State:
                UART_ID =  UART_PrivInfoArr[LoopIndex].UART_NUM;
                if ( UART_PrivInfoArr[LoopIndex].UART_TransmitRequestFlag == 1 )
                {
                    if (  UART_PrivInfoArr[LoopIndex].UART_DataLength < ( UART_PrivInterpretTriggerLengthToNums( UART_2ByteTriggerLength ) + 2 ) ) // this is the minimum datalength that generates interrupt
                    {
                        while (  UART_PrivInfoArr[LoopIndex].UART_SentDataLength < UART_PrivInfoArr[LoopIndex].UART_DataLength  )
                        {
                            UARTDR(UART_ID) = UART_PrivInfoArr[LoopIndex].ptrToBuffer[UART_PrivInfoArr[LoopIndex].UART_SentDataLength] ;
                            UART_PrivInfoArr[LoopIndex].UART_SentDataLength++ ;
                        }
                        UART_PrivInfoArr[LoopIndex].UART_Channel_Status = UART_Job_Done;  /* means that the request is transfered to the UART ISR as we reached the FIFO trigger length */
                        CLR_BIT(UARTIM( UART_PrivInfoArr[LoopIndex].UART_NUM ) , TXIM );
                    }
                    else
                    {
                        UART_ChannelIndex =  LoopIndex;  /* in order to make the index in the info and config arrays of the UART known by the ISR instead searching again */
                        while (  UART_PrivInfoArr[LoopIndex].UART_SentDataLength < ( UART_PrivInfoArr[LoopIndex].TxFIFOTrigLength + 2 )  )
                        {
                            UARTDR(UART_ID) = UART_PrivInfoArr[LoopIndex].ptrToBuffer[UART_PrivInfoArr[LoopIndex].UART_SentDataLength] ;
                            UART_PrivInfoArr[LoopIndex].UART_SentDataLength++ ;
                        }
                    }
                }
                else if ( UART_PrivInfoArr[LoopIndex].UART_ReceiveRequestFlag == 1 )
                {
                    /* if the receive fifo is not empty and the Remaining recieve bytes is less than the trigger length then no interrupt will occur then this should be handeld by the main */
                    if ( ( !( GET_BIT( UARTFR(UART_ID) , RXFE ) ) ) && ( RemainingReceiveBytes < ( UART_PrivInfoArr[LoopIndex].RxFIFOTrigLength + 1 ) ) ) /*  */
                    {
                        while ( ( !( GET_BIT( UARTFR(UART_ID) , RXFE ) ) ) && ( UART_PrivInfoArr[LoopIndex].UART_ReceivedDatalength < UART_PrivInfoArr[LoopIndex].UART_DataLength ) )
                        {
                            UART_PrivInfoArr[LoopIndex].ptrToBuffer[UART_PrivInfoArr[LoopIndex].UART_ReceivedDatalength]  = UARTDR( UART_PrivInfoArr[LoopIndex].UART_NUM ) ;
                            UART_PrivInfoArr[LoopIndex].UART_ReceivedDatalength++ ;
                            if ( UART_PrivInfoArr[LoopIndex].UART_ReceivedDatalength == UART_PrivInfoArr[LoopIndex].UART_DataLength  )
                            {
                                UART_PrivInfoArr[LoopIndex].UART_Channel_Status = UART_Job_Done;
                                CLR_BIT( UARTIM( UART_PrivInfoArr[LoopIndex].UART_NUM ) , RXIM );
                            }
                        }
                    }
                }
                break;
            case UART_Job_Done:
                UART_PrivInfoArr[LoopIndex].UART_Channel_Status = UART_IDLE_State;
                UART_PrivInfoArr[LoopIndex].UART_DataLength = 0;
                if (   UART_PrivInfoArr[LoopIndex].UART_TransmitRequestFlag == 1)
                {
                    UART_PrivInfoArr[LoopIndex].UART_SentDataLength = 0;
                    UART_PrivInfoArr[LoopIndex].UART_TransmitRequestFlag = 0;
                    UART_ConfigArray[LoopIndex].TransmitterFifo_Config.UART_TxHandlerPtr();
                }
                else if ( UART_PrivInfoArr[LoopIndex].UART_ReceiveRequestFlag == 1 )
                {
                    UART_PrivInfoArr[LoopIndex].UART_ReceivedDatalength = 0;
                    UART_PrivInfoArr[LoopIndex].UART_ReceiveRequestFlag = 0;
                    UART_ConfigArray[LoopIndex].RecieverFifo_Config.UART_RxHandlerPtr();

                }
                break;
            }
        }

    }
}

/*============================================================= Baud Rate Calculation =============================================================*/
void UART_vidBaudRateCalc(u8 IndexInConfigArr)
{
    u32 baudrate=UART_ConfigArray[IndexInConfigArr].BaudRate;
    u8  speed_mode;
    if(UART_ConfigArray[IndexInConfigArr].Speed_Mode == UART_NORMAL_SPEED_MODE )
    {
        speed_mode = 16;
    }
    else
    {
        speed_mode = 8;
    }
    f32 calc=0;
    u16 FractionPart ;


    calc=CRYSTAL_FREQ/(baudrate*speed_mode*1.0);
    u16 integerPart= (u16)calc;
    calc=((calc-integerPart)*64)+0.5; //calculate the float part
    FractionPart = (u16) calc;
    UARTIBRD(UART_ConfigArray[IndexInConfigArr].UARTNum)=integerPart;
    UARTFBRD(UART_ConfigArray[IndexInConfigArr].UARTNum)=FractionPart;

}

/*======================================================== UART ISR ============================================================*/
void UART1_ISR (void)
{
    u8 Local_i = 0;
    u8 sentbytes = 0;
    if ( GET_BIT( UARTRIS(UART1),TXRIS ) && ( UART_PrivInfoArr[UART_ChannelIndex].UART_Channel_Status == UART_SUSPENDED_State ) )  /*The received FIFO level has passed through the condition defined in the UARTIFLS register */
    {
        while ( ( UART_PrivInfoArr[UART_ChannelIndex].UART_SentDataLength < UART_PrivInfoArr[UART_ChannelIndex].UART_DataLength ) && ( sentbytes <= ( UART_PrivInfoArr[UART_ChannelIndex].TxFIFOTrigLength + 1 )  ) )
        {
            UARTDR(UART1) = UART_PrivInfoArr[UART_ChannelIndex].ptrToBuffer[UART_PrivInfoArr[UART_ChannelIndex].UART_SentDataLength] ;
            UART_PrivInfoArr[UART_ChannelIndex].UART_SentDataLength++;
            sentbytes++;
        }
        if ( ( UART_PrivInfoArr[UART_ChannelIndex].UART_SentDataLength )  == ( UART_PrivInfoArr[UART_ChannelIndex].UART_DataLength ) )
        {
            UART_PrivInfoArr[UART_ChannelIndex].UART_Channel_Status = UART_Job_Done;  /* means that the request is transfered to the UART ISR as we reached the FIFO trigger length */
            CLR_BIT(UARTIM( UART_PrivInfoArr[UART_ChannelIndex].UART_NUM ) , TXIM );
        }
    }
    else if ( GET_BIT( UARTRIS(UART1),RXRIS ) && ( UART_PrivInfoArr[UART_ChannelIndex].UART_Channel_Status == UART_SUSPENDED_State ) ) /*The Transmitted FIFO level has passed through the condition defined in the UARTIFLS register */
    {
        while( !( GET_BIT( UARTFR(UART1) , RXFE ) ) && ( UART_PrivInfoArr[UART_ChannelIndex].UART_ReceivedDatalength < UART_PrivInfoArr[UART_ChannelIndex].UART_DataLength ) )
        {
            UART_PrivInfoArr[UART_ChannelIndex].ptrToBuffer[UART_PrivInfoArr[UART_ChannelIndex].UART_ReceivedDatalength]  = UARTDR(UART1) ;
            UART_PrivInfoArr[UART_ChannelIndex].UART_ReceivedDatalength++ ;
            Local_i++;
        }
        if ( UART_PrivInfoArr[UART_ChannelIndex].UART_ReceivedDatalength == UART_PrivInfoArr[UART_ChannelIndex].UART_DataLength  )
        {
            UART_PrivInfoArr[UART_ChannelIndex].UART_Channel_Status = UART_Job_Done;
            CLR_BIT(UARTIM( UART_PrivInfoArr[UART_ChannelIndex].UART_NUM ) , RXIM );
        }
        RemainingReceiveBytes = UART_PrivInfoArr[UART_ChannelIndex].UART_DataLength - UART_PrivInfoArr[UART_ChannelIndex].UART_ReceivedDatalength ;
    }
}



/* ================================================== Enable Register Interrupt =================================================*/
void Priv_UARTInterruptEnable(UART_e_UARTNumber_t ModuleNum )
{
    switch( ModuleNum )
    {
    case UART0:
        SET_BIT( EN0 , UARTInterruptNumbers[UART0] );
        break;
    case UART1:
        SET_BIT( EN0 , UARTInterruptNumbers[UART1] );
        break;
    case UART2:
        SET_BIT( EN1 , (UARTInterruptNumbers[UART2]%ENn_REG_SIZE) );
        break;
    case UART3:
        SET_BIT( EN1 , (UARTInterruptNumbers[UART3]%ENn_REG_SIZE) );
        break;
    case UART4:
        SET_BIT( EN1 , (UARTInterruptNumbers[UART4]%ENn_REG_SIZE) );
        break;
    case UART5:
        SET_BIT( EN1 , (UARTInterruptNumbers[UART5]%ENn_REG_SIZE) );
        break;
    case UART6:
        SET_BIT( EN1 , (UARTInterruptNumbers[UART6]%ENn_REG_SIZE) );
        break;
    case UART7:
        SET_BIT( EN1 , (UARTInterruptNumbers[UART7]%ENn_REG_SIZE) );
        break;

    }
}

/* =============================================== Interpret FIFO trigger length to number ============================================*/

u8 UART_PrivInterpretTriggerLengthToNums( UART_e_FIFOTriggerLength_t Fifolength)
{
    u8 length;
    switch ( Fifolength )
    {
    case UART_2ByteTriggerLength:
        length = 2;
        break;
    case UART_4ByteTriggerLength:
        length = 4;
        break;
    case UART_8ByteTriggerLength:
        length = 8;
        break;
    case UART_12ByteTriggerLength:
        length = 12;
        break;
    case UART_14ByteTriggerLength:
        length = 14;
        break;
    }
    return length;
}
