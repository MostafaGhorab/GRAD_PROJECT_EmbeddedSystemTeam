/*
 * SPI_Prog.c
 *
 *  Created on: Jan 22, 2020
 *      Author: Dell
 */

#include "SPI_int.h"
#include "SPI_config.h"
#include "SPI_MemoryMap.h"
#include "std_types.h"
#include "SPI_Priv.h"
#include "utils.h"
#include "Port.h"
#include "Port_Priv.h"
#include "DIO.h"
extern void delay(u32 count);
extern Spi_Sequence ArrOfSequences[NumOfModules];
extern Spi_Job ArrOfJobs[NumOfJobs];
extern Spi_Channel ArrOfChannels[NumOfChannels];
Module_Info ModuleInfoArr[NumOfModules]={0};   /* has the state of the modules and their IDs */
Channel_Info ChannelInfoArr[NumOfChannels]={0}; /* has the (state of the channel/sentdatalength/receiveddatalength/channelid and the number of frames to be transmitted or received) */
u8 InitializedSequencersArr[NumOfModules]={FalseNumber,FalseNumber};  /* in order not to initialize previously initialized sequencers */
void SPI_VidInit(void)
{
    u8 ChLoopIndex=0;
    u8 SeqNum=0;
    u8 SeqMode=0;
    u32 SSIClk=0;
    u8 i=0;    /*For Sequence Number*/
    for(ChLoopIndex=0; ChLoopIndex < NumOfChannels ; ChLoopIndex++)
    {
        SeqNum=ArrOfChannels[ChLoopIndex].AssignedJobPtr -> AssignedSeqPtr -> ModuleID;

        /* first initialize the channel info array which is not visible to the user */
        ChannelInfoArr[ChLoopIndex].ChannelMode = ArrOfChannels[ChLoopIndex].AssignedJobPtr -> AssignedSeqPtr -> mode;
        ChannelInfoArr[ChLoopIndex].framelength = ArrOfChannels[ChLoopIndex].DataFrameLength;
        ChannelInfoArr[ChLoopIndex].ModuleID = SeqNum;
        ChannelInfoArr[ChLoopIndex].InfoChannelId = ArrOfChannels[ChLoopIndex].ChannelID;
        ChannelInfoArr[ChLoopIndex].DefaultVal = ArrOfChannels[ChLoopIndex].DefaultTransmitVal;
        ChannelInfoArr[ChLoopIndex].ChannelStatus = IDLE;
        ChannelInfoArr[ChLoopIndex].SentdataLength = 0;
        ChannelInfoArr[ChLoopIndex].ReceiveddataLength = 0;

        /* here we initialize the registers for the selected spi module */

        if( ( SequencerPrevInitializCheck(SeqNum) != Initialized ) )
        {
            SeqMode=ArrOfChannels[ChLoopIndex].AssignedJobPtr-> AssignedSeqPtr->mode;
            SSIClk=ArrOfChannels[ChLoopIndex].AssignedJobPtr-> SpiClk;
            RCGCSSI|=1<<SeqNum;
            SSICR1(SeqNum)&= ~(1<<SSE);
            SSICR1(SeqNum)|=SeqMode;
            SSICC(SeqNum)=0x00;
            Priv_VidClkCalc(SSIClk,SeqNum);
            SSICR0(SeqNum)|=( ArrOfChannels[ChLoopIndex].AssignedJobPtr-> ClkPhase ) <<SPH;
            SSICR0(SeqNum)|=( ArrOfChannels[ChLoopIndex].AssignedJobPtr-> ClkPol ) <<SPO;
            SSICR0(SeqNum)&=~(3<<FRF);

            //SSICR1(0)|=0x1;

            SSICR0(SeqNum)|=ChannelInfoArr[ChLoopIndex].framelength;
            SSICR1(SeqNum)|=(1<<SSE);
            ModuleInfoArr[i].ModuleId = SeqNum;             /*To initialize the info array */
            ModuleInfoArr[i].ModuleStatus = IDLE;
            InitializedSequencersArr[i] = SeqNum;     /*To indicate that this sequence has previously been initialized*/
            i++;
        }
    }
}
/*======================================================Request To Send Function ====================================================================================*/

enum_RequestStatus SPI_RequestToSend( u8 channelID , u8 DataLength , u8 * SendDataBufferPtr )
{
    u8 i;
    u8 IDfound=0;
    enum_RequestStatus state;
    state = RequestNotAccepted;
    for( i=0 ; i<NumOfChannels ; i++ ) /* searching for the channel that made the request channelinfoarr */
    {
        if( channelID == ChannelInfoArr[i].InfoChannelId)
        {
            if( ChannelInfoArr[i].ChannelStatus == IDLE )   /* checks if the channel itself is idle or it's suspended or busy from prvious requests */
            {
                ChannelInfoArr[i].SendRequestFlag = 1;
                ChannelInfoArr[i].ChannelStatus = SUSPENDED;
                ChannelInfoArr[i].datalength = DataLength;
                ArrOfChannels[i].ptrTobuffer = SendDataBufferPtr;
                state = RequestAccepted;
                IDfound = 1 ;
                break;
            }
        }
    }

    if( IDfound == 0)/* User sent a wrong ID */
    {
        state = RequestNotAccepted;
    }
    return state;
}
/*============================================================= Request to Receive func ===================================================================*/

enum_RequestStatus SPI_RequestToReceive( u8 channelID, u8 DataLength , u8 * ReceiveDataBufferPtr )
{
    u8 i;
    u8 IDfound=0;
    enum_RequestStatus state;
    state=RequestNotAccepted;
    for(i=0;i<NumOfChannels;i++)
    {
        if( channelID == ChannelInfoArr[i].InfoChannelId )
        {
            if( ChannelInfoArr[i].ChannelStatus == IDLE )
            {
                ChannelInfoArr[i].ReceiveRequestFlag = 1;
                ChannelInfoArr[i].ChannelStatus = SUSPENDED;
                ChannelInfoArr[i].datalength = DataLength;
                ArrOfChannels[i].ptrTobuffer = ReceiveDataBufferPtr;
                state = RequestAccepted;
                IDfound=1;
                break;
            }
        }
    }

    if( IDfound == 0)/* User sent a wrong ID */
    {
        state = RequestNotAccepted;
    }

    return state;
}
/*============================================================= Main function =====================================================================================*/
void SPI_Mainfunc(void)
{
    u8 LoopIndex;
    u8 SeqID;
    u8 moduleindex;
    for(LoopIndex=0 ; LoopIndex<NumOfChannels ; LoopIndex++)
    {
        if( ChannelInfoArr[LoopIndex].SendRequestFlag == 1 )                   /* in Case of sending */
        {
            SeqID = ChannelInfoArr[LoopIndex].ModuleID;
            moduleindex = Priv_GetModuleIndex(SeqID);
            if( ModuleInfoArr[moduleindex].ModuleStatus == IDLE )
            {
                ModuleInfoArr[moduleindex].ModuleStatus = BUSY;
                ChannelInfoArr[LoopIndex].ChannelStatus = BUSY;
                while( ( GET_BIT( SSISR(SeqID),TNF ) ) && ( ChannelInfoArr[LoopIndex].SentdataLength < ChannelInfoArr[LoopIndex].datalength ) )
                {

                    Priv_SendFrame( ArrOfChannels[LoopIndex].ptrTobuffer[ ChannelInfoArr[LoopIndex].SentdataLength ] , SeqID );
                    ChannelInfoArr[LoopIndex].SentdataLength++;
                }
                if( ChannelInfoArr[LoopIndex].SentdataLength == ChannelInfoArr[LoopIndex].datalength )
                {
                    ChannelInfoArr[LoopIndex].SendRequestFlag = 0;
                    ChannelInfoArr[LoopIndex].ChannelStatus = IDLE;
                    ModuleInfoArr[moduleindex].ModuleStatus = IDLE;
                    //ArrOfSequences[moduleindex].SeqEndNotificationPtr();
                }
                if( ChannelInfoArr[LoopIndex].SentdataLength < ChannelInfoArr[LoopIndex].datalength )
                {
                    ChannelInfoArr[LoopIndex].ChannelStatus = SUSPENDED ;
                    ModuleInfoArr[moduleindex].ModuleStatus = IDLE;
                }
            }
        }

        if( ChannelInfoArr[LoopIndex].ReceiveRequestFlag == 1 )                   /* in Case of Receiving */
        {
            SeqID= ChannelInfoArr[LoopIndex].ModuleID;
            moduleindex = Priv_GetModuleIndex(SeqID);
            if( ( ModuleInfoArr[moduleindex].ModuleStatus == IDLE ) )
            {
                ModuleInfoArr[moduleindex].ModuleStatus = BUSY;
                ChannelInfoArr[LoopIndex].ChannelStatus = BUSY;
                if(ChannelInfoArr[LoopIndex].ChannelMode == Master)  /* in case a master wants to receive then it must send the number of bytes it wants to receive */
                {
                    while( ChannelInfoArr[LoopIndex].ReceiveddataLength < ChannelInfoArr[LoopIndex].datalength  )
                    {
                        Priv_SendFrame( ChannelInfoArr[LoopIndex].DefaultVal , SeqID );
                        ArrOfChannels[LoopIndex].ptrTobuffer[ ChannelInfoArr[LoopIndex].ReceiveddataLength ] = Priv_ReceiveFrame(SeqID);
                        ChannelInfoArr[LoopIndex].ReceiveddataLength++;
                    }
                    if( ChannelInfoArr[LoopIndex].ReceiveddataLength == ChannelInfoArr[LoopIndex].datalength )
                    {
                        ChannelInfoArr[LoopIndex]. ReceiveRequestFlag = 0;
                        ChannelInfoArr[LoopIndex].ChannelStatus = IDLE;
                        ModuleInfoArr[moduleindex].ModuleStatus = IDLE;
                    }
                    if( ChannelInfoArr[LoopIndex].ReceiveddataLength < ChannelInfoArr[LoopIndex].datalength )
                    {
                        ChannelInfoArr[LoopIndex].ChannelStatus = SUSPENDED ;
                        ModuleInfoArr[moduleindex].ModuleStatus = IDLE;
                    }
                }
                else if ( ( ChannelInfoArr[LoopIndex].ChannelMode == SlaveWithOP ) || ( ChannelInfoArr[LoopIndex].ChannelMode == SlaveWithoutOP ) )
                {
                    while( ( GET_BIT( SSISR(SeqID),RNE ) ) && ( ChannelInfoArr[LoopIndex].ReceiveddataLength < ChannelInfoArr[LoopIndex].datalength ) )
                    {
                        ArrOfChannels[LoopIndex].ptrTobuffer[ ChannelInfoArr[LoopIndex].ReceiveddataLength ] = Priv_ReceiveFrame(SeqID);
                        ChannelInfoArr[LoopIndex].ReceiveddataLength++;
                    }
                    if( ChannelInfoArr[LoopIndex].ReceiveddataLength == ChannelInfoArr[LoopIndex].datalength )
                    {
                        ChannelInfoArr[LoopIndex]. ReceiveRequestFlag = 0;
                        ChannelInfoArr[LoopIndex].ChannelStatus = IDLE;
                        ModuleInfoArr[moduleindex].ModuleStatus = IDLE;
                    }
                    if( ChannelInfoArr[LoopIndex].ReceiveddataLength < ChannelInfoArr[LoopIndex].datalength )
                    {
                        ChannelInfoArr[LoopIndex].ChannelStatus = SUSPENDED ;
                        ModuleInfoArr[moduleindex].ModuleStatus = IDLE;
                    }
                }
            }
        }
    }
}
/*==================================== Checks if the sequencer is previously initialized to avoid multiple initializations ===============================*/
enum_SeqInit SequencerPrevInitializCheck(u8 Seqnum)
{
    enum_SeqInit flag = NotInitialized;
    u8 i;
    for(i=0;i<NumOfModules;i++)
    {
        if(InitializedSequencersArr[i] == Seqnum)
        {
            flag=Initialized;
            break;
        }
    }
    return flag;
}

/*============================================================ SSI clk speed Calculations =========================================================================*/
void Priv_VidClkCalc(u32 SPICLK,u8 spinum)
{
    u8 volatile Divisor;
    u8 volatile SCR;
    for(Divisor=2; Divisor<=254; Divisor+=2)
    {
        SCR=((SysClk)/(SPICLK*Divisor))-1;
        if( (SCR>0) && (SCR<256) )
        {
            SSICPSR(spinum) = Divisor;
            SSICR0(spinum) |= SCR<<SCRValShift;
            break;
        }
    }
}

/*============================== Private function to calculate the loop index of the module in the ArrOfSequencers =============================================*/
u8 Priv_GetModuleIndex(u8 Module_num)
{
    u8 i;
    for(i=0 ; i<NumOfModules ; i++)
    {
        if( ModuleInfoArr[i].ModuleId == Module_num )
        {
            break;
        }
    }
    return i;
}

/*=========================================  Private function to send the data =========================================*/
void Priv_SendFrame(u8 Data,u8 MouduleID)
{
    SSIDR(MouduleID)=Data ;
    Dio_WriteChannel(PORTA_PIN3,HIGH);
}

/*============================================ Private function to Receive the data =====================================*/

u8 Priv_ReceiveFrame(u8 MouduleID)
{
    u8 data;
    data = SSIDR(MouduleID);
    return data;
}


