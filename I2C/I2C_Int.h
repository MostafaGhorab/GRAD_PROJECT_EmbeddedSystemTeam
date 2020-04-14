/*
 * I2C_int.h
 *
 *  Created on: Feb 16, 2020
 *      Author: dodo_
 */

#ifndef I2C_INT_H_
#define I2C_INT_H_

#include "std_types.h"

typedef enum
{
    Single_Start_Condition,
    Repeated_Start_Condition
}enum_I2C_ReqType;

typedef enum
{
    I2C_Bus_Is_Idle,
    I2C_Bus_ClkTimeOut

}enum_I2C_BusStates;

typedef enum
{
    I2C_Module_Is_Idle,
    I2C_Module_Is_Busy

}enum_I2C_ModuleStates;


typedef enum
{
    Standard = 100000,                              /* (100 Kbps) */
    Fast_mode = 400000,                             /* (400 Kbps) */
    Fast_mode_plus = 1000000,                       /*  (1 Mbps)  */
    High_speed_mode = 3330000                       /*  (3.33 Mbps)  */

}enum_I2C_SpeedModes;

typedef enum
{
    ArbitrationLost,
    ErrorOccured,
    NoErrors
}enum_I2C_ErrorTypes;

typedef enum
{
    I2C_RequestAccepted,
    I2C_RequestNotAccepted

}enum_I2C_RequestStatus;

typedef enum
{
    I2C0,
    I2C1,
    I2C2,
    I2C3

}enum_I2C_ModuleNumber_t;

typedef enum
{
    I2C_Master,
    I2C_Slave

}enum_I2C_ModuleType_t;

typedef enum
{
    I2C_Single_Slave_address,
    I2C_Dual_Slave_address
}enum_I2C_SlaveAddMode_t;

typedef enum
{
    I2C_Channel_Is_Idle,         /* channel has done no requests */
    I2C_Channel_Is_Suspended,    /* channel has done a request but not yet done handling */
    I2C_Channel_Request_Queued,  /* In the case that the main function has sent the slave address and the register address but the I2C_ISR has not been fired yet */
    I2C_Channel_Is_Busy,         /* channel's request is being handled */
    I2C_Channel_Lost_Arb,
    I2C_Channel_Encountered_BTO,
    I2C_Channel_Encountered_Error,
    I2C_Channel_Job_Done          /* channel's request is successfully done */

}enum_I2C_ChannelStates;

typedef struct
{
    u8 * Sendbuffer ;
    u8 * ReceiveBuffer;
    u8 I2C_SendSlaveAddress;
    u8 I2C_ReceiveSlaveAddress;
    u8 ReceiveRegisterAddress;
    void(*ErrorHandler)(void);
    void(*BusClockTimeOutHandler)(void);
    void(*ChannelEndNotificationHandler)(void);
    void(*ArbitrationLostHandler)(void);
    enum_I2C_ReqType RequestType;
    enum_I2C_SpeedModes I2C_Module_Speed;
    enum_I2C_ModuleStates I2C_ModuleStatus;
    enum_I2C_ChannelStates ChannelStatus;

}struct_I2C_MasterUserChannel_t;

typedef enum{
    I2C_Slave_Ack_Enabled,
    I2C_Slave_Ack_Disabled
}enum_I2C_ACKBitStatus_t;

typedef struct
{
    enum_I2C_SlaveAddMode_t I2C_e_SlaveAddressMode;
    enum_I2C_ACKBitStatus_t AckbitStatus;
    u8  u8_SlaveEndNotificationFlag;  /* used to be raised to one when the slave receives a stop condition */
    u8 * I2C_Slave_SendBuffer;
    u8 * I2C_Slave_ReceiveBuffer;
    void (*I2C_Slave_StopBitCallBack)(void);
}struct_I2C_SlaveUserChannel_t;

typedef struct{
    u8 I2C_x_ChannelID;
    enum_I2C_ModuleNumber_t I2C_Module_num;
    enum_I2C_ModuleType_t I2C_e_ChannelType;
    struct_I2C_MasterUserChannel_t I2C_s_MasterChannel;
    struct_I2C_SlaveUserChannel_t I2C_s_SlaveChannel;
}struct_I2C_UserChannel_t;

void I2C_VidInit(void);
enum_I2C_RequestStatus I2C_MasterRequestToSend(u8 ChannelID, u8 * BufferPtr , u8 DataLength , u8 SlaveAddress,enum_I2C_ReqType ReqType );
enum_I2C_RequestStatus I2C_MasterRequestToReceive(u8 ChannelID, u8 * BufferPtr , u8 DataLength , u8 SlaveAddress, u8 receivereg ,enum_I2C_ReqType ReqType );
void I2C_SlaveEnable(enum_I2C_ModuleNumber_t e_I2CModuleNum,u8 u8_SlaveAddress1,u8 u8_SlaveAddress2 );
void I2C_SlaveDisable(enum_I2C_ModuleNumber_t e_I2CModuleNum);
void I2C_MasterMainFunc(void);
void I2C_SlaveMainFunc(void);
void I2C1_MasterISR(void);
void I2C0_SlaveISR(void);

#endif /* I2C_INT_H_ */
