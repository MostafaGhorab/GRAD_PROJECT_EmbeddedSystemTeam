/*
 * IMU_Priv.h
 *
 *  Created on: Mar 8, 2020
 *      Author: dodo_
 */

#ifndef IMU_PRIV_H_
#define IMU_PRIV_H_
#include "std_types.h"
#include "IMU_int.h"
#define CALIBRATION_LIMIT      (u8)10
#define MODULE_ADRDRESS        (u8) 0x68
#define IMU_UPDATE_LIMIT        8
#define pi                      3.14519
#define RAWDATALENGTH           14
#define RAWDATANUMBER           7
#define LAST_CALIBRATION_STEP   25


typedef enum
{
    CalibrationDone,
    CalibrationNotDone
}enum_IMU_PrivCalibrtionResult;

typedef enum
{
    IMU_NOT_INITIALIZED,
    IMU_INITIALIZED,
    IMU_IDLE,
    IMU_BUSY,
    IMU_SUSPENDED
}IMU_Module_StatusType;

typedef enum
{
    SetFlagLow,
    SetFlagHigh
}enum_ReceiveReqFlagStat;


typedef struct
{
 u8 I2C_ChannelID;
 u8 IMU_RegisterReadings[14];      /* Raw Register readings before converting to signed */
 f32 ACCEL_XVal;       /*data  after the fixed values division and using the arctan functions */
 f32 ACCEL_YVal;
 f32 ACCEL_ZVal;
 f32 GYRO_XVal;
 f32 GYRO_YVal;
 f32 GYRO_ZVal;
 s16 RawDataBuffer[7];    /* raw data without any processing */
 f32 BaseVals[7];        /* after calibration values */
 f32 RollPitchYaw[3];             /* calculated Roll Pitch and Yaw angle */
 f32 * UserBuffer;
 enum_ReceiveReqFlagStat ReciveReqFlag;
 enum_IMU_ReqDataType UserRequestedDataType;
 IMU_Module_StatusType ModuleStatus; /*Initialised or not*/

}IMU;


f32 NumPow( f32 num, u8 pow);
f32 arctan ( f32 z );
f32 sqrt(f32 x);
void IMU_PrivConvertToSignedShort(s16 * SignedShortData);
enum_IMU_PrivCalibrtionResult IMU_PrivCalibrate( void );
void IMU_privUpdateRawData(void);
void IMU_PrivConvertToDegPerSec(void);
void IMU_PrivConvertToAngle(void);
void IMU_PrivApplyCompelementaryFilter(void);
#endif /* IMU_PRIV_H_ */
