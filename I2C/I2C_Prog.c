/*
 * I2C_Prog.c
 *
 *  Created on: Feb 16, 2020
 *      Author: dodo_
 */


#include "I2C_Priv.h"
#include "I2C_MemMap.h"
#include "I2C_Config.h"
#include "utils.h"
#include "GPIO_MemMap.h"
enum_Semaphore Semaphore = Semaphore_Available;
#define CLEAR_INTERRUPT_FLAG          1
#define ENABLE                        1
#define DISABLE                       0
#define DATA_COUNTER_INITIALIZE_VAL   0
static const u8 I2CInterruptNumbers[4] = { 8, 37, 68, 69 };/* the number divided by 32 gives the ENx register number and the remainder is the position inside the reg */
extern delay(u32 max);
extern struct_I2C_UserChannel_t I2C_s_UserChannelArr[I2C_NUM_OF_CHANNELS];
I2C_Channel_Info I2C_MasterChannelInfoArr[I2C_NUM_OF_CHANNELS];

u8 ChannelHavingTheBus ; /* initialized by the initializer function to have the least channel index in the array which is most probable to be the first to take control on the bus */

/* =========================================== I2C_Initialization Function =====================================================*/

void I2C_VidInit(void)
{
    u8 ChLoopIndex=0;
    enum_I2C_ModuleNumber_t e_Loc_I2CModuleNum;
    for(ChLoopIndex=0; ChLoopIndex < I2C_NUM_OF_CHANNELS ; ChLoopIndex++)
    {
        e_Loc_I2CModuleNum = I2C_s_UserChannelArr[ChLoopIndex].I2C_Module_num;  /* saving the module number in order to use it through the rest of the function */
        if( I2C_s_UserChannelArr[ChLoopIndex].I2C_e_ChannelType == I2C_Master)
        {
            ChannelHavingTheBus = I2C_s_UserChannelArr[0].I2C_x_ChannelID;
            I2C_MasterChannelInfoArr[ChLoopIndex].I2C_InfoChannelId = I2C_s_UserChannelArr[ChLoopIndex].I2C_x_ChannelID;
            I2C_MasterChannelInfoArr[ChLoopIndex].I2C_ReceiveRequestFlag = DATA_COUNTER_INITIALIZE_VAL;
            I2C_MasterChannelInfoArr[ChLoopIndex].I2C_SendRequestFlag = DATA_COUNTER_INITIALIZE_VAL;
            I2C_MasterChannelInfoArr[ChLoopIndex].I2C_ReceiveddataLength = DATA_COUNTER_INITIALIZE_VAL;
            I2C_MasterChannelInfoArr[ChLoopIndex].I2C_SentdataLength = DATA_COUNTER_INITIALIZE_VAL;
            I2C_s_UserChannelArr[ChLoopIndex].I2C_s_MasterChannel.I2C_ModuleStatus = I2C_Module_Is_Idle;
            I2C_s_UserChannelArr[ChLoopIndex].I2C_s_MasterChannel.ChannelStatus = I2C_Channel_Is_Idle;
            RCGCI2C |= 1 << e_Loc_I2CModuleNum ;
            I2CMCR( I2C_s_UserChannelArr[ChLoopIndex].I2C_Module_num ) |= ENABLE << MasterEnableRegShiftBits;
        }
        else if ( I2C_s_UserChannelArr[ChLoopIndex].I2C_e_ChannelType == I2C_Slave )
        {
            /* Register clock and interrupt ENABLE for stop , start , and data requests */
            RCGCI2C |= 1 << e_Loc_I2CModuleNum ;
            I2CSIMR( e_Loc_I2CModuleNum ) |= ( ( ENABLE << STOPIM ) | ( ENABLE << STARTIM ) | ( ENABLE << DATAIM ) ) ;
        }
        Priv_I2CInterruptEnable( e_Loc_I2CModuleNum );  /*global interrupt enable*/
    }
}

/*=============================================== I2C Transfer Request function ============================================*/

enum_I2C_RequestStatus I2C_MasterRequestToSend(u8 ChannelID, u8 * BufferPtr , u8 DataLength , u8 SlaveAddress,enum_I2C_ReqType ReqType )
{
    enum_I2C_RequestStatus state;
    state = I2C_RequestNotAccepted;
    if( I2C_s_UserChannelArr[ChannelID].I2C_s_MasterChannel.ChannelStatus == I2C_Channel_Is_Idle )   /* checks if the channel itself is idle or it's suspended  from prvious requests */
    {
        Semaphore = Semaphore_NotAvailable;   /* As the following section is a critical section */
        I2C_s_UserChannelArr[ChannelID].I2C_s_MasterChannel.ChannelStatus = I2C_Channel_Is_Suspended;
        I2C_MasterChannelInfoArr[ChannelID].I2C_SendRequestFlag = 1;
        I2C_MasterChannelInfoArr[ChannelID].I2C_senddatalength = DataLength;
        I2C_s_UserChannelArr[ChannelID].I2C_s_MasterChannel.Sendbuffer = BufferPtr;
        I2C_s_UserChannelArr[ChannelID].I2C_s_MasterChannel.I2C_SendSlaveAddress =  SlaveAddress;
        I2C_s_UserChannelArr[ChannelID].I2C_s_MasterChannel.RequestType = ReqType;
        state = I2C_RequestAccepted;
        Semaphore = Semaphore_Available;
    }
    return state;
}


/*================================================== I2C Receive Request function ============================================*/

enum_I2C_RequestStatus I2C_MasterRequestToReceive(u8 ChannelID, u8 * BufferPtr , u8 DataLength , u8 SlaveAddress, u8 receivereg ,enum_I2C_ReqType ReqType )
{
    enum_I2C_RequestStatus state;
    state = I2C_RequestNotAccepted;
    if( I2C_s_UserChannelArr[ChannelID].I2C_s_MasterChannel.ChannelStatus == I2C_Channel_Is_Idle )   /* checks if the channel itself is idle or it's suspended  from previous requests */
    {
        Semaphore = Semaphore_NotAvailable;   /* As the following section is a critical section */
        I2C_s_UserChannelArr[ChannelID].I2C_s_MasterChannel.ChannelStatus = I2C_Channel_Is_Suspended;
        I2C_MasterChannelInfoArr[ChannelID].I2C_ReceiveRequestFlag = 1;
        I2C_MasterChannelInfoArr[ChannelID].I2C_receivedatalength = DataLength;
        I2C_s_UserChannelArr[ChannelID].I2C_s_MasterChannel.ReceiveBuffer = BufferPtr;
        I2C_s_UserChannelArr[ChannelID].I2C_s_MasterChannel.I2C_ReceiveSlaveAddress =  SlaveAddress;
        I2C_s_UserChannelArr[ChannelID].I2C_s_MasterChannel.RequestType = ReqType;
        I2C_s_UserChannelArr[ChannelID].I2C_s_MasterChannel.ReceiveRegisterAddress = receivereg;
        state = I2C_RequestAccepted;
        Semaphore = Semaphore_Available;
    }
    return state;
}


/*==================================================== I2C main function function ============================================*/
void I2C_MasterMainFunc(void)
{
    u8 ChLoopIndex;
    u8 SA;
    enum_I2C_BusStates busstatus;
    u8 BusIdleFlag=0;
    enum_I2C_ChannelStates ChannelStatus;
    enum_I2C_ModuleNumber_t e_Loc_I2CModuleNum;
    if ( Semaphore == Semaphore_Available )
    {
        for ( ChLoopIndex = 0; ChLoopIndex < I2C_NUM_OF_CHANNELS; ChLoopIndex++)
        {
            busstatus = Priv_I2C_BusCheck(ChLoopIndex);
            switch( busstatus )
            {
            case I2C_Bus_Is_Idle:
                BusIdleFlag = 1;
                break;
            case I2C_Bus_ClkTimeOut:
                BusIdleFlag = 0;
                I2C_s_UserChannelArr[ChLoopIndex].I2C_s_MasterChannel.ChannelStatus = I2C_Channel_Encountered_BTO;
                break;
            }


            ChannelStatus = I2C_s_UserChannelArr[ChLoopIndex].I2C_s_MasterChannel.ChannelStatus;
            e_Loc_I2CModuleNum = I2C_s_UserChannelArr[ChLoopIndex].I2C_Module_num;
            switch ( ChannelStatus )
            {
            case I2C_Channel_Is_Suspended:
                if ( BusIdleFlag == 1 )
                {
                    if( ( I2C_MasterChannelInfoArr[ChLoopIndex].I2C_SendRequestFlag ) == 1 )
                    {
                        ChannelHavingTheBus = I2C_s_UserChannelArr[ChLoopIndex].I2C_x_ChannelID;
                        I2CMIMR( I2C_s_UserChannelArr[ChLoopIndex].I2C_Module_num ) |= ENABLE;
                        I2C_s_UserChannelArr[ChLoopIndex].I2C_s_MasterChannel.ChannelStatus = I2C_Channel_Request_Queued;
                        Priv_I2C_ModuleClkCalc( e_Loc_I2CModuleNum , I2C_s_UserChannelArr[ChLoopIndex].I2C_s_MasterChannel.I2C_Module_Speed);
                        I2CMSA( e_Loc_I2CModuleNum ) = ( ( I2C_s_UserChannelArr[ChLoopIndex].I2C_s_MasterChannel.I2C_SendSlaveAddress ) << 1 );
                        I2CMDR( e_Loc_I2CModuleNum ) = I2C_s_UserChannelArr[ChLoopIndex].I2C_s_MasterChannel.Sendbuffer[I2C_MasterChannelInfoArr[ChLoopIndex].I2C_SentdataLength];
                        I2CMCS( e_Loc_I2CModuleNum ) = STARTRUNCOMMAND ;
                    }
                    else if (  ( I2C_MasterChannelInfoArr[ChLoopIndex].I2C_ReceiveRequestFlag ) == 1 )
                    {
                        I2C_s_UserChannelArr[ChLoopIndex].I2C_s_MasterChannel.ChannelStatus = I2C_Channel_Request_Queued;
                        I2CMIMR( e_Loc_I2CModuleNum ) |= ENABLE;
                        ChannelHavingTheBus = I2C_s_UserChannelArr[ChLoopIndex].I2C_x_ChannelID;
                        Priv_I2C_ModuleClkCalc( e_Loc_I2CModuleNum , I2C_s_UserChannelArr[ChLoopIndex].I2C_s_MasterChannel.I2C_Module_Speed);
                        I2CMSA( e_Loc_I2CModuleNum ) =  ( I2C_s_UserChannelArr[ChLoopIndex].I2C_s_MasterChannel.I2C_ReceiveSlaveAddress ) << 1 ;
                        if( I2C_s_UserChannelArr[ChLoopIndex].I2C_s_MasterChannel.RequestType == Repeated_Start_Condition)
                        {
                            I2CMDR( e_Loc_I2CModuleNum ) = I2C_s_UserChannelArr[ChLoopIndex].I2C_s_MasterChannel.ReceiveRegisterAddress;
                            I2CMCS( e_Loc_I2CModuleNum ) = STARTRUNCOMMAND  ;
                        }
                        else if( I2C_s_UserChannelArr[ChLoopIndex].I2C_s_MasterChannel.RequestType == Single_Start_Condition )
                        {
                            I2CMSA( e_Loc_I2CModuleNum ) = ( SA |= 1);
                            I2CMCS( e_Loc_I2CModuleNum ) = ACKSTARTRUNCOMMAND  ;
                        }
                    }
                }
                break;

            case I2C_Channel_Lost_Arb:
                I2C_s_UserChannelArr[ChLoopIndex].I2C_s_MasterChannel.ArbitrationLostHandler();
                break;

            case I2C_Channel_Encountered_BTO:
                I2C_s_UserChannelArr[ChLoopIndex].I2C_s_MasterChannel.BusClockTimeOutHandler();
                break;

            case I2C_Channel_Encountered_Error:
                I2CMCS( I2C_s_UserChannelArr[ChLoopIndex].I2C_Module_num ) = STOPCOMMAND;
                I2C_MasterChannelInfoArr[ChLoopIndex].I2C_ReceiveRequestFlag = 0;   /* here we don't make the (received data) and (sent data) counter equals to zero as the user may request again in the handler so it continues from where it has stopped */
                I2C_MasterChannelInfoArr[ChLoopIndex].I2C_SentdataLength = 0;
                I2C_s_UserChannelArr[ChLoopIndex].I2C_s_MasterChannel.ChannelStatus = I2C_Channel_Is_Idle;
                I2C_s_UserChannelArr[ChLoopIndex].I2C_s_MasterChannel.ErrorHandler();
                break;

            case I2C_Channel_Job_Done:
                I2C_s_UserChannelArr[ChLoopIndex].I2C_s_MasterChannel.ChannelStatus = I2C_Channel_Is_Idle;
                I2C_MasterChannelInfoArr[ChLoopIndex].I2C_ReceiveRequestFlag = 0;
                I2C_MasterChannelInfoArr[ChLoopIndex].I2C_SendRequestFlag = 0;
                I2C_MasterChannelInfoArr[ChLoopIndex].I2C_ReceiveddataLength = 0;
                I2C_MasterChannelInfoArr[ChLoopIndex].I2C_SentdataLength = 0;
                I2CMICR( I2C_s_UserChannelArr[ChannelHavingTheBus].I2C_Module_num ) = CLEAR_INTERRUPT_FLAG;
                I2C_s_UserChannelArr[ChLoopIndex].I2C_s_MasterChannel.ChannelEndNotificationHandler();
                break;
            }
        }
    }
}

/* ===================================================== Module frequency calculation function =======================================*/

void Priv_I2C_ModuleClkCalc(u8 ModuleNum, u32 ModuleClk)
{
    I2CMTPR( ModuleNum ) = ( SysClk / ( 2 * ( SCL_LP + SCL_HP )* ModuleClk ))-1;
}

/*======================================================= Error Check Function ======================================================*/

enum_I2C_ErrorTypes Priv_I2C_ErrorCheck(u8 ChannelInfoArrIndex)
{
    enum_I2C_ErrorTypes ErrorStatus;
    if( GET_BIT( I2CMCS( I2C_s_UserChannelArr[ChannelInfoArrIndex].I2C_Module_num ), ERROR ) == 1 )
    {
        if(  GET_BIT( I2CMCS( I2C_s_UserChannelArr[ChannelInfoArrIndex].I2C_Module_num ), ARBLST ) == 1  )
        {
            ErrorStatus = ArbitrationLost;
        }
        else
        {
            ErrorStatus = ErrorOccured;
        }
    }
    else
    {
        ErrorStatus = NoErrors;
    }
    return ErrorStatus;

}

/*=============================================== Bus Status check function =======================================*/

enum_I2C_BusStates Priv_I2C_BusCheck( u8 ChannelInfoArrIndex )
{
    enum_I2C_BusStates BusStatus = I2C_Bus_Is_Idle;
    u8 ClkTimeOutFlag = 0;
    while( GET_BIT( I2CMCS( I2C_s_UserChannelArr[ChannelInfoArrIndex].I2C_Module_num), MODULEBUSY ) == 1 )
    {
        if( GET_BIT( I2CMCS( I2C_s_UserChannelArr[ChannelInfoArrIndex].I2C_Module_num ), CLOCKTIMEOUT ) == 1 )
        {
            ClkTimeOutFlag = 1;
            break;
        }
    }
    if ( ClkTimeOutFlag == 1 )
    {
        BusStatus = I2C_Bus_ClkTimeOut;
    }
    return BusStatus;
}

/*================================================== I2C_Master_ISR =====================================================*/

void I2C1_MasterISR(void)
{
    u8 SA;
    enum_I2C_ErrorTypes ErrorCheck;
    ErrorCheck = Priv_I2C_ErrorCheck( ChannelHavingTheBus );
    I2CMICR( I2C_s_UserChannelArr[ChannelHavingTheBus].I2C_Module_num ) = CLEAR_INTERRUPT_FLAG;
    switch ( ErrorCheck )
    {
    case ArbitrationLost:
        I2C_s_UserChannelArr[ChannelHavingTheBus].I2C_s_MasterChannel.ChannelStatus = I2C_Channel_Lost_Arb;
        I2CMICR( I2C_s_UserChannelArr[ChannelHavingTheBus].I2C_Module_num ) = CLEAR_INTERRUPT_FLAG;
        break;

    case ErrorOccured:
        I2C_s_UserChannelArr[ChannelHavingTheBus].I2C_s_MasterChannel.ChannelStatus = I2C_Channel_Encountered_Error;
        I2CMICR( I2C_s_UserChannelArr[ChannelHavingTheBus].I2C_Module_num ) = CLEAR_INTERRUPT_FLAG;
        break;

    case NoErrors:
        if( ( I2C_MasterChannelInfoArr[ChannelHavingTheBus].I2C_SendRequestFlag == 1 )  )
        {
            if( I2C_MasterChannelInfoArr[ChannelHavingTheBus].I2C_SentdataLength == 0 )
            {
                I2C_MasterChannelInfoArr[ChannelHavingTheBus].I2C_SentdataLength++;  // as we sent one byte in the main so we start from the next
                if(  I2C_s_UserChannelArr[ChannelHavingTheBus].I2C_s_MasterChannel.RequestType == Repeated_Start_Condition )
                {
                    I2CMSA( I2C_s_UserChannelArr[ChannelHavingTheBus].I2C_Module_num ) = ( ( I2C_s_UserChannelArr[ChannelHavingTheBus].I2C_s_MasterChannel.I2C_SendSlaveAddress ) << 1 );
                    I2CMDR( I2C_s_UserChannelArr[ChannelHavingTheBus].I2C_Module_num ) = I2C_s_UserChannelArr[ChannelHavingTheBus].I2C_s_MasterChannel.Sendbuffer[ I2C_MasterChannelInfoArr[ChannelHavingTheBus].I2C_SentdataLength ];
                    I2CMCS( I2C_s_UserChannelArr[ChannelHavingTheBus].I2C_Module_num ) = STARTRUNCOMMAND ;
                }
            }
            I2C_s_UserChannelArr[ChannelHavingTheBus].I2C_s_MasterChannel.ChannelStatus = I2C_Channel_Is_Busy;
            I2CMDR( I2C_s_UserChannelArr[ChannelHavingTheBus].I2C_Module_num ) = I2C_s_UserChannelArr[ChannelHavingTheBus].I2C_s_MasterChannel.Sendbuffer[ I2C_MasterChannelInfoArr[ChannelHavingTheBus].I2C_SentdataLength ];
            I2CMCS( I2C_s_UserChannelArr[ChannelHavingTheBus].I2C_Module_num ) = RUNCOMMAND ;
            I2C_MasterChannelInfoArr[ChannelHavingTheBus].I2C_SentdataLength++;
            if( (  I2C_MasterChannelInfoArr[ChannelHavingTheBus].I2C_SentdataLength == I2C_MasterChannelInfoArr[ChannelHavingTheBus].I2C_senddatalength ) )
            {
                I2CMIMR( I2C_s_UserChannelArr[ChannelHavingTheBus].I2C_Module_num ) = DISABLE;
                I2CMCS( I2C_s_UserChannelArr[ChannelHavingTheBus].I2C_Module_num  ) = STOPCOMMAND ;
                I2C_s_UserChannelArr[ChannelHavingTheBus].I2C_s_MasterChannel.ChannelStatus = I2C_Channel_Job_Done;
            }
        }
        else if ( ( I2C_MasterChannelInfoArr[ChannelHavingTheBus].I2C_ReceiveRequestFlag == 1 ) )
        {
            if( ( I2C_s_UserChannelArr[ChannelHavingTheBus].I2C_s_MasterChannel.RequestType == Repeated_Start_Condition ) && (  I2C_s_UserChannelArr[ChannelHavingTheBus].I2C_s_MasterChannel.ChannelStatus == I2C_Channel_Request_Queued ))
            {
                I2C_s_UserChannelArr[ChannelHavingTheBus].I2C_s_MasterChannel.ChannelStatus = I2C_Channel_Is_Busy;
                SA = ( I2C_s_UserChannelArr[ChannelHavingTheBus].I2C_s_MasterChannel.I2C_ReceiveSlaveAddress ) << 1 ;
                I2CMSA( I2C_s_UserChannelArr[ChannelHavingTheBus].I2C_Module_num ) = ( SA |= 1);
                I2CMCS( I2C_s_UserChannelArr[ChannelHavingTheBus].I2C_Module_num ) = ACKSTARTRUNCOMMAND  ;
            }
            else
            {
                I2C_s_UserChannelArr[ChannelHavingTheBus].I2C_s_MasterChannel.ReceiveBuffer[ I2C_MasterChannelInfoArr[ChannelHavingTheBus].I2C_ReceiveddataLength ] = I2CMDR( I2C_s_UserChannelArr[ChannelHavingTheBus].I2C_Module_num );
                I2C_MasterChannelInfoArr[ChannelHavingTheBus].I2C_ReceiveddataLength++;
                if(  I2C_MasterChannelInfoArr[ChannelHavingTheBus].I2C_ReceiveddataLength == (I2C_MasterChannelInfoArr[ChannelHavingTheBus].I2C_receivedatalength - 1 ) )
                {
                    I2CMCS( I2C_s_UserChannelArr[ChannelHavingTheBus].I2C_Module_num  ) = RUNCOMMAND ;
                }
                else if( (  I2C_MasterChannelInfoArr[ChannelHavingTheBus].I2C_ReceiveddataLength < I2C_MasterChannelInfoArr[ChannelHavingTheBus].I2C_receivedatalength ) )
                {
                    I2CMCS( I2C_s_UserChannelArr[ChannelHavingTheBus].I2C_Module_num  ) = ACKRUNCOMMAND ;
                }
                else if ( I2C_MasterChannelInfoArr[ChannelHavingTheBus].I2C_ReceiveddataLength == (I2C_MasterChannelInfoArr[ChannelHavingTheBus].I2C_receivedatalength  ) )
                {
                    I2CMIMR( I2C_s_UserChannelArr[ChannelHavingTheBus].I2C_Module_num ) = DISABLE;
                    I2CMCS( I2C_s_UserChannelArr[ChannelHavingTheBus].I2C_Module_num  ) = STOPCOMMAND ;
                    I2C_s_UserChannelArr[ChannelHavingTheBus].I2C_s_MasterChannel.ChannelStatus = I2C_Channel_Job_Done;
                }
            }
        }
        break;
    }
}

/*================================================= I2C Slave Main Func ==============================================*/

void I2C_SlaveMainFunc(void)
{
    u8 ChLoopIndex=0;
    for(ChLoopIndex=0; ChLoopIndex < I2C_NUM_OF_CHANNELS ; ChLoopIndex++)
    {
        if( I2C_s_UserChannelArr[ChLoopIndex].I2C_e_ChannelType == I2C_Slave)
        {
            if( I2C_s_UserChannelArr[ChLoopIndex].I2C_s_SlaveChannel.u8_SlaveEndNotificationFlag == 1 )
            {
                I2C_s_UserChannelArr[ChLoopIndex].I2C_s_SlaveChannel.u8_SlaveEndNotificationFlag = 0;
                I2C_s_UserChannelArr[ChLoopIndex].I2C_s_SlaveChannel.I2C_Slave_StopBitCallBack();
            }
        }
    }
}

/*=================================================== I2C Slave ISR ====================================================*/
void I2C0_SlaveISR(void)
{
    static u32 u32_loc_ReceiveCounter = 0;
    static u32 u32_loc_TransmitCounter = 0;

    if ( GET_BIT( I2CSRIS( I2C0 ) , STARTIM ) )
    {
        if( GET_BIT( I2CSCSR( I2C0 ),RREQ ) )
        {
            u32_loc_ReceiveCounter = 0;
        }
        else if ( GET_BIT( I2CSCSR( I2C0 ),TREQ ) )
        {
            u32_loc_TransmitCounter = 0;
        }
    }
    if( GET_BIT( I2CSRIS( I2C0 ) , STOPIM ) )
    {
        I2C_s_UserChannelArr[I2C0].I2C_s_SlaveChannel.u8_SlaveEndNotificationFlag = 1;
    }
    if( GET_BIT( I2CSCSR( I2C0 ),RREQ ) )
    {
        I2C_s_UserChannelArr[I2C0].I2C_s_SlaveChannel.I2C_Slave_ReceiveBuffer[u32_loc_ReceiveCounter] = I2CSDR(I2C0);
        u32_loc_ReceiveCounter++;
    }
    else if ( GET_BIT( I2CSCSR( I2C0 ),TREQ ) )
    {
        I2CSDR(I2C0) = I2C_s_UserChannelArr[I2C0].I2C_s_SlaveChannel.I2C_Slave_SendBuffer[u32_loc_TransmitCounter];
        u32_loc_TransmitCounter++;
    }
    SET_BIT(I2CSICR( I2C0 ), STARTIM );  /* Clearing the interrupt status bits  */
    SET_BIT(I2CSICR( I2C0 ), DATAIM );
    SET_BIT(I2CSICR( I2C0 ), STOPIM );

}
/*=========================================== Slave Enable func ======================================================  */

void I2C_SlaveEnable(enum_I2C_ModuleNumber_t e_I2CModuleNum,u8 u8_SlaveAddress1,u8 u8_SlaveAddress2 )
{
    if( I2C_s_UserChannelArr[e_I2CModuleNum].I2C_s_SlaveChannel.I2C_e_SlaveAddressMode == I2C_Dual_Slave_address )
    {
        I2CSOAR2( e_I2CModuleNum) |= ( ENABLE << SOAR2_ENABLE_BIT );
        I2CSOAR2( e_I2CModuleNum ) = u8_SlaveAddress2;
    }
    if(  I2C_s_UserChannelArr[e_I2CModuleNum].I2C_s_SlaveChannel.AckbitStatus == I2C_Slave_Ack_Enabled  )
    {
        I2CSACKCTL( e_I2CModuleNum )  |= ENABLE;
    }
    I2CMCR( e_I2CModuleNum ) |= ENABLE << SlaveEnableRegShiftBits;
    I2CSCSR( e_I2CModuleNum ) = ENABLE;
    I2CSOAR(e_I2CModuleNum )  = u8_SlaveAddress1;

}

/*================================================= I2C Slave Disable ================================================*/

void I2C_SlaveDisable(enum_I2C_ModuleNumber_t e_I2CModuleNum)
{
    I2CMCR( e_I2CModuleNum ) &= !( 1 << SlaveEnableRegShiftBits );
    I2CSCSR( e_I2CModuleNum ) &= DISABLE;
}

/*================================================== I2C Interrupt Enable ==============================================*/

void Priv_I2CInterruptEnable(enum_I2C_ModuleNumber_t ModuleNum )
{
    switch( ModuleNum )
    {
    case I2C0:
        SET_BIT( EN0 , I2CInterruptNumbers[I2C0] );
        break;
    case I2C1:
        SET_BIT( EN1 , (I2CInterruptNumbers[I2C1]%ENn_REG_SIZE) );
        break;
    case I2C2:
        SET_BIT( EN2 , (I2CInterruptNumbers[I2C2]%ENn_REG_SIZE) );
        break;
    case I2C3:
        SET_BIT( EN2 , (I2CInterruptNumbers[I2C3]%ENn_REG_SIZE) );
        break;

    }
}
