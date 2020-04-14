/*
 * IMU_Priv.c
 *
 *  Created on: Mar 8, 2020
 *      Author: dodo_
 */


#include "IMU_Priv.h"
#include "IMU_int.h"
#include "IMU_Config.h"
#include "IMU_MemMap.h"
#include "I2C_Config.h"







#define RADIANS_TO_DEGREES      (f32)180/pi
#define ALPHA                   (f32) (1-MAIN_FUNC_PERIODICITY)
extern IMU IMU_Module;
extern u8 u8_global_deltaTimeFactor ;
u8 u8_global_IMUStepNumber = 0;
extern stuct_I2C_UserChannel I2C_UserChannelArr[I2C_NUM_OF_CHANNELS];
/*============================================ power function ========================================================*/
f32 NumPow( f32 num, u8 pow)
{
    u8 i=1;
    f32 temp = num;
    if(pow==0)
    {
        return 1;
    }
    while ( i < pow )
    {
        num = num * temp ;
        i++;
    }
    return num;
}

/*============================================ arctan function ========================================================*/

f32 arctan ( f32 z )
{
    f32 result;
    if ( z > 1.0)
    {
        result = ( pi/2 ) - ( ( 1/z ) / ( 1 + ( 0.28125/ NumPow( z , 2 ) ) ) );
    }
    else if ( z < -1.0 )
    {
        result = ( -pi / 2 ) - ( ( 1/z ) / ( 1 + ( 0.28125/ NumPow( z , 2 ) ) ) );
    }
    else
    {
        result = ( ( z ) / ( 1 + ( 0.28125 * NumPow( z , 2 ) ) ) );
    }
    return result;
}

/*============================================ sqrt function ========================================================*/

f32 sqrt(f32 x)
{
    union
    {
        s32 i;
        f32 x;
    } u;

    u.x = x;
    u.i = (1<<29) + (u.i >> 1) - (1<<22);
    return u.x;
}

void IMU_privUpdateRawData(void)
{
    u8 i;

    for(i=0;i<7;i++)
    {

        IMU_Module.RawDataBuffer[i]  = ( (s16) (IMU_Module.IMU_RegisterReadings[i*2]<<8) );
        IMU_Module.RawDataBuffer[i]  |= ( (s16)(IMU_Module.IMU_RegisterReadings[(i*2)+1]) );

    }

}


/*============================================ Convert from Raw To degree/second function ========================================================*/

void IMU_PrivConvertToDegPerSec(void)
{
    IMU_Module.ACCEL_XVal = ( (f32)IMU_Module.RawDataBuffer[0] ) / 4096.0;
    IMU_Module.ACCEL_YVal = ( (f32)IMU_Module.RawDataBuffer[1] ) / 4096.0;
    IMU_Module.ACCEL_ZVal = ( (f32)IMU_Module.RawDataBuffer[2] ) /4096.0;
    IMU_Module.GYRO_XVal =  ( ( (f32)IMU_Module.RawDataBuffer[4] ) - IMU_Module.BaseVals[4] ) / 131.0;
    IMU_Module.GYRO_YVal =  ( ( (f32)IMU_Module.RawDataBuffer[5] ) - IMU_Module.BaseVals[5] ) / 131.0;
    IMU_Module.GYRO_ZVal =  ( ( (f32)IMU_Module.RawDataBuffer[6] ) - IMU_Module.BaseVals[6] ) / 131.0;
}

/*============================================ Convert from degree/second to angle function ========================================================*/
void IMU_PrivConvertToAngle(void)
{
    IMU_Module.ACCEL_YVal = arctan(-1*IMU_Module.ACCEL_XVal/sqrt(NumPow(IMU_Module.ACCEL_YVal ,2) + NumPow(IMU_Module.ACCEL_ZVal ,2)))*RADIANS_TO_DEGREES;
    IMU_Module.ACCEL_XVal = arctan(IMU_Module.ACCEL_YVal /sqrt(NumPow(IMU_Module.ACCEL_XVal,2) + NumPow(IMU_Module.ACCEL_ZVal,2)))*RADIANS_TO_DEGREES;
    IMU_Module.ACCEL_ZVal = 0;
    IMU_Module.GYRO_XVal = IMU_Module.GYRO_XVal *  MAIN_FUNC_PERIODICITY * u8_global_deltaTimeFactor + IMU_Module.RollPitchYaw [0];
    IMU_Module.GYRO_YVal = IMU_Module.GYRO_YVal *  MAIN_FUNC_PERIODICITY * u8_global_deltaTimeFactor + IMU_Module.RollPitchYaw [1];
    IMU_Module.GYRO_ZVal = IMU_Module.GYRO_ZVal *  MAIN_FUNC_PERIODICITY * u8_global_deltaTimeFactor + IMU_Module.RollPitchYaw [2];
}

void IMU_PrivApplyCompelementaryFilter (void)
{
    static u8 counter = 0;
    static s16 Initial_pitch,Initial_roll,Initial_Yaw;
    if ( counter == 0 )
    {
        IMU_Module.RollPitchYaw [0] = ( 1 - (MAIN_FUNC_PERIODICITY * u8_global_deltaTimeFactor) ) * IMU_Module.GYRO_XVal + MAIN_FUNC_PERIODICITY * u8_global_deltaTimeFactor  *  IMU_Module.ACCEL_XVal ;
        IMU_Module.RollPitchYaw [1] = ( 1 - (MAIN_FUNC_PERIODICITY * u8_global_deltaTimeFactor) ) * IMU_Module.GYRO_YVal + MAIN_FUNC_PERIODICITY * u8_global_deltaTimeFactor  *  IMU_Module.ACCEL_YVal ;
        IMU_Module.RollPitchYaw [2] =  IMU_Module.GYRO_ZVal ;
        Initial_pitch = IMU_Module.RollPitchYaw [0] ;
        Initial_roll = IMU_Module.RollPitchYaw [1] ;
        Initial_Yaw = IMU_Module.RollPitchYaw [2] ;
        counter =1;
    }
    else if ( counter == 1)
    {
        if ( Initial_pitch > 0 )
        {
            IMU_Module.RollPitchYaw [0] = (( 1 - (MAIN_FUNC_PERIODICITY * u8_global_deltaTimeFactor) ) * IMU_Module.GYRO_XVal + MAIN_FUNC_PERIODICITY * u8_global_deltaTimeFactor  *  IMU_Module.ACCEL_XVal )- Initial_pitch ;
        }
        else
        {
            IMU_Module.RollPitchYaw [0] = (( 1 - (MAIN_FUNC_PERIODICITY * u8_global_deltaTimeFactor) ) * IMU_Module.GYRO_XVal + MAIN_FUNC_PERIODICITY * u8_global_deltaTimeFactor  *  IMU_Module.ACCEL_XVal ) + Initial_pitch;
        }
        IMU_Module.RollPitchYaw [1] = (( 1 - (MAIN_FUNC_PERIODICITY * u8_global_deltaTimeFactor) ) * IMU_Module.GYRO_YVal + MAIN_FUNC_PERIODICITY * u8_global_deltaTimeFactor  *  IMU_Module.ACCEL_YVal );
        IMU_Module.RollPitchYaw [2] =  IMU_Module.GYRO_ZVal ;
    }
}


/*============================================ Calibration function ========================================================*/
enum_IMU_PrivCalibrtionResult IMU_PrivCalibrate(void)
{
    static u8 RequestFlag = 1;
    u8 j;
    static s32 AccumArr[7] = {0};
    enum_IMU_PrivCalibrtionResult CalibrationResult = CalibrationNotDone;
    s16 SignedData[RAWDATANUMBER]={0};
    if( u8_global_IMUStepNumber == LAST_CALIBRATION_STEP )
    {
        for( j = 0 ; j < RAWDATANUMBER ; j++ )
        {
            IMU_Module.BaseVals[j] = ( (f32)AccumArr[j] / CALIBRATION_LIMIT);
        }
        CalibrationResult = CalibrationDone;
    }
    else
    {
        /* the even steps make requests and the request flag is to avoid multiple committing of requests while the first has not been finished */
        if( ( ( u8_global_IMUStepNumber % 2 ) == 0 ) && ( RequestFlag == 1 ) )
        {
            I2C_MasterRequestToReceive( IMU_Module.I2C_ChannelID , IMU_Module.IMU_RegisterReadings , RAWDATALENGTH , MODULE_ADRDRESS , ACCEL_XOUT_H ,Repeated_Start_Condition);
            RequestFlag = 0;
        }
        else if  ( ( ( u8_global_IMUStepNumber % 2 ) != 0 ) && ( RequestFlag == 0 ) )           /* the odd steps take actions */
        {
            IMU_PrivConvertToSignedShort(SignedData);
            for( j = 0 ; j < RAWDATANUMBER ; j++ )
            {
                AccumArr[j] += SignedData[j];
            }
            RequestFlag = 1;
            u8_global_IMUStepNumber++;
        }
    }
    return CalibrationResult;
}



void IMU_PrivConvertToSignedShort(s16 * SignedShortData)
{
    u8 i ;
    u8 counter = 0;
    for ( i = 0 ; i < RAWDATALENGTH ; i+=2 )
    {
        SignedShortData[counter] = ( (s16) (IMU_Module.IMU_RegisterReadings[i] << 8) |  (s16)( IMU_Module.IMU_RegisterReadings[i+1] ) );
        counter++;
    }
}



