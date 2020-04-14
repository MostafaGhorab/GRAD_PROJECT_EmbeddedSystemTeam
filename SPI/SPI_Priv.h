/*
 * SPI_Priv.h
 *
 *  Created on: Jan 22, 2020
 *      Author: Dell
 */

#ifndef SPI_PRIV_H_
#define SPI_PRIV_H_

#include "SPI_int.h"


typedef enum
{
    Initialized,
    NotInitialized
}enum_SeqInit;

typedef enum
{
    SPI_ok,
    SPI_nok
}enum_SPIError;

typedef enum
{
    IDLE,
    SUSPENDED,
    BUSY
}enum_States;

typedef struct
{
    u8 ModuleId;
    enum_States ModuleStatus; /* IDLE BUSY OR SUSPENDED */
}Module_Info;

typedef struct
{
    u8 InfoChannelId;
    u8 datalength;    /* size in bytes */
    u8 framelength;   /* in bits */
    u8 ModuleID;
    u8 DefaultVal;
    u8 SentdataLength;
    u8 ReceiveddataLength;
    u8 SendRequestFlag;    /* if high then the channel has requested a transmit operation */
    u8 ReceiveRequestFlag; /* if high then the channel has requested a receive operation */
    enum_SpiModuleType ChannelMode; /* slave or slave without Output or Master */
    enum_States ChannelStatus;    /* IDLE BUSY OR SUSPENDED */
}Channel_Info;

#define FalseNumber 5
#define SCRValShift 8

enum_SeqInit SequencerPrevInitializCheck(u8 Seqnum);
void Priv_VidClkCalc(u32 SPICLK,u8 spinum);
void SPI_Mainfunc(void);
u8 Priv_GetModuleIndex(u8 Module_num);
void Priv_SendFrame(u8 Data,u8 MouduleID);
u8 Priv_ReceiveFrame(u8 MouduleID);

#endif /* SPI_PRIV_H_ */
