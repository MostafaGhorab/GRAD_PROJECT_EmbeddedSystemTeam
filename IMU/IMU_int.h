/*
 * IMU_int.h
 *
 *  Created on: Feb 28, 2020
 *      Author: dodo_
 */

#ifndef IMU_INT_H_
#define IMU_INT_H_

#include "std_types.h"
#include "I2C_Int.h"
typedef enum
{
  InitializationDone,
  InitializationNotDone
}enum_IMU_InitilizationfunctionStatus;

typedef enum
{

     Internal_8MHz_oscillator,
     PLL_with_X_axis_gyroscope_reference,
     PLL_with_Y_axis_gyroscope_reference,
     PLL_with_Z_axis_gyroscope_reference,
     PLL_with_external_32kHz_reference,
     PLL_with_external_192MHz_reference

}Clock_Source;
typedef enum
{

    GYRO_250=0,
    GYRO_500,
    GYRO_1000,
    GYRO_2000


}GYRO_Range;

typedef enum
{

    ACCEL_2g=0,
    ACCEL_4g,
    ACCEL_8g,
    ACCEL_16g


}ACCEL_Range;

typedef enum
{
    FIFO_DISABLED=0,
    FIFO_ENABLED

}FIFO_Status;

typedef enum
{
    AD0_LOW=0,
    AD0_HIGH

}Adress_selector;

typedef struct
{
    ACCEL_Range Accelerometer_Range;
    GYRO_Range  Gyroscope_Range;
    FIFO_Status FifoStatus ;
    Clock_Source Clock;
    Adress_selector AD0_status;
    u8 I2C_Channel_Config;
    void (*SuccessHandlerPtr)(void) ;
    void (*FailureHandlerPtr)(void) ;
}IMU_config_type;


typedef enum
{
   IMU_OK,
   IMU_NOT_OK

}IMU_Read_Func_Return;

typedef enum
{
   IMU_ReqOK,
   IMU_ReqNOT_OK

}IMU_Req_Func_Return;

typedef enum
{
    Roll,
    Pitch,
    Yaw,
    RawData,
    RPY
}enum_IMU_ReqDataType;

enum_IMU_InitilizationfunctionStatus IMU_VidInit(void);
IMU_Req_Func_Return IMU_ReqToReceive( f32 * PtrToBuffer , enum_IMU_ReqDataType  RequestedDataType );
void IMU_MainFunc(void);

#endif /* IMU_INT_H_ */
