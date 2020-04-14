/*
 * I2C_Config.c
 *
 *  Created on: Feb 16, 2020
 *      Author: dodo_
 */

#include "I2C_Config.h"
#include "I2C_Int.h"
#define NULL ((void *)0)
void error_func (void);
void Stopbit_func (void);
u8 ReceiveArr[24]={0};
u8 TransmitArr[24]={1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9,1,2,3,4,5,6};
struct_I2C_UserChannel_t I2C_s_UserChannelArr[I2C_NUM_OF_CHANNELS] =
{
 {
  0,
  I2C0,
  I2C_Slave,
  {
   NULL,
   NULL,
   0,
   0,
   0,
   NULL ,
   NULL,
   NULL,
   NULL,
   Single_Start_Condition,
   Fast_mode,
   I2C_Module_Is_Idle,
   I2C_Channel_Is_Idle
  },
  {
   I2C_Single_Slave_address,
   I2C_Slave_Ack_Enabled,
   0,
   TransmitArr,                      /*Slave Send Buffer pointer*/
   ReceiveArr,                       /*Slave Receive Buffer pointer*/
   Stopbit_func
  }
 },
 {
   1,
   I2C1,
   I2C_Master,
   {
    NULL,
    NULL,
    0,
    0,
    0,
    NULL ,
    NULL,
    error_func,
    NULL,
    Single_Start_Condition,
    Fast_mode,
    I2C_Module_Is_Idle,
    I2C_Channel_Is_Idle
   },
   {
    I2C_Single_Slave_address,
    I2C_Slave_Ack_Enabled,
    0,
    NULL,                /*Slave Send Buffer pointer*/
    NULL,                 /*Slave Receive Buffer pointer*/
    NULL
   }
  }
};



void error_func (void)
{


}

void Stopbit_func (void)
{


}

//typedef struct
//{
//    u8 I2C_ChannelID;
//    u8 * Sendbuffer ;
//    u8 * ReceiveBuffer;
//    u8 I2C_SendSlaveAddress;
//    u8 I2C_ReceiveSlaveAddress;
//    u8 ReceiveRegisterAddress;
//    void(*ErrorHandler)(void);
//    void(*BusClockTimeOutHandler)(void);
//    void(*ChannelEndNotificationHandler)(void);
//    void(*ArbitrationLostHandler)(void);
//   enum_I2C_ReqType RequestType;
//    enum_I2C_ModuleNumber_t I2C_Module_num;
//    enum_I2C_ModuleType_t I2C_Module_type;
//    enum_I2C_SpeedModes I2C_Module_Speed;
//    enum_I2C_ModuleStates I2C_ModuleStatus;
//    enum_I2C_ChannelStates ChannelStatus;
//
//}stuct_I2C_MasterUserChannel_t;




