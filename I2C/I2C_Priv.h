/*
 * I2C_Priv.h
 *
 *  Created on: Feb 16, 2020
 *      Author: dodo_
 */

#ifndef I2C_PRIV_H_
#define I2C_PRIV_H_

#include "std_types.h"
#include "I2C_Int.h"

/*
 * x  means  variable   type
 * e  means  enum       type
 * s  means  struct     type
 *
 * note that the i2c number for this driver is location in the userarray
 */

#define I2C_FalseInitVal            5
#define MasterEnableRegShiftBits    4
#define SlaveEnableRegShiftBits     5
#define BUSBUSY                     6
#define CLOCKTIMEOUT                7
#define MODULEBUSY                  0
#define ERROR                       1
#define ARBLST                      4


#define RUNCOMMAND                  0x01
#define STARTRUNCOMMAND             0x03
#define STOPCOMMAND                 0x04
#define STOPRUNCOMMAND              0x05
#define STOPSTARTRUNCOMMAND         0x07
#define ACKSTARTRUNCOMMAND          0x0B
#define ACKRUNCOMMAND               0x09

#define SOAR2_ENABLE_BIT              7
#define RREQ                          0
#define TREQ                          1
#define STOPIM                        2
#define STARTIM                       1
#define DATAIM                        0

typedef enum
{
   Semaphore_Available,
   Semaphore_NotAvailable
}enum_Semaphore;
typedef struct
{
    u8 I2C_InfoChannelId;
    u8 I2C_senddatalength;                                      /* size in bytes */
    u8 I2C_receivedatalength;
    u8 I2C_SentdataLength;
    u8 I2C_ReceiveddataLength;
    u8 I2C_SendRequestFlag;                                 /* if high then the channel has requested a transmit operation */
    u8 I2C_ReceiveRequestFlag;                              /* if high then the channel has requested a receive operation */
}I2C_Channel_Info;

void Priv_I2CInterruptEnable(enum_I2C_ModuleNumber_t ModuleNum );
void Priv_I2C_ModuleClkCalc(u8 ModuleNum, u32 ModuleClk);
enum_I2C_ErrorTypes Priv_I2C_ErrorCheck(u8 ChannelInfoArrIndex);
enum_I2C_BusStates Priv_I2C_BusCheck( u8 ChannelInfoArrIndex );

#endif /* I2C_PRIV_H_ */
