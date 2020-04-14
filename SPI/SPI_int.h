/*
 * SPI_int.h
 *
 *  Created on: Jan 20, 2020
 *      Author: Dell
 */

#include "std_types.h"
#ifndef SPI_INT_H_
#define SPI_INT_H_


typedef enum
{
    FourBitFrame = 0x3,
    FiveBitFrame = 0x4,
    SixBitFrame = 0x5,
    SevenBitFrame = 0x6,
    EightBitFrame = 0x7,
    NineBitFrame = 0x8,
    TenBitFrame = 0x9,
    ElevenBitFrame = 0xA,
    TwelveBitFrame = 0xB,
    ThirteenBitFrame = 0xC,
    FourteenBitFrame = 0xD,
    FifteenBitFrame = 0xE,
    SixteenBitFrame = 0xF
}enum_frameLenghth;

typedef enum
{
 RequestAccepted,
 RequestNotAccepted
}enum_RequestStatus;

typedef enum
{
    SlaveWithOP=(u32)0x4,
    SlaveWithoutOP=(u32)0xC,
    Master=(u32)0x0
}enum_SpiModuleType;

typedef enum
{
SSI0,
SSI1,
SSI2,
SSI3
}enum_SpiModuleNumber;

typedef enum{
    first_clk_edge_transition,
    second_clk_edge_transition
}enum_SpiClkphase;

typedef enum{
    steady_state_low,
    steady_state_high
}enum_SpiClkPolarity;

typedef struct
{
    u8 ModuleID;
    enum_SpiModuleType mode;
    void(*SeqEndNotificationPtr)(void);
}Spi_Sequence;

typedef struct{
    u8 JobID;
    u8 AssignedChipSelectPin;
    enum_SpiClkPolarity ClkPol;    /*Idle Clock state*/
    enum_SpiClkphase ClkPhase;
    void(*JobEndNotificationPtr)(void);
    Spi_Sequence * AssignedSeqPtr;
    u32 SpiClk;
}Spi_Job;

typedef struct
{
    u8 ChannelID;
    u8 DefaultTransmitVal;
    enum_frameLenghth DataFrameLength ; /* from 4 to 16 bits */
    Spi_Job * AssignedJobPtr;
    u8 * ptrTobuffer ;
}Spi_Channel;


void SPI_VidInit(void);
enum_RequestStatus SPI_RequestToSend( u8 channelID , u8 DataLength , u8 * SendDataBufferPtr );
enum_RequestStatus SPI_RequestToReceive( u8 channelID, u8 DataLength , u8 * ReceiveDataBufferPtr );

#endif /* SPI_INT_H_ */
