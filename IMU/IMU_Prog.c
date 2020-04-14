/*
 * IMU_Prog.c
 *
 *  Created on: Feb 28, 2020
 *      Author: dodo_
 */
#include "std_types.h"
#include "IMU_MemMap.h"
#include "IMU_int.h"
#include "IMU_Priv.h"
#include "IMU_Config.h"
#include "utils.h"
#include "I2C_Priv.h"
#include "I2C_MemMap.h"
#include "I2C_Config.h"
#include "UART_int.h"
#include "UART_priv.h"
#include "UART_Config.h"
#include "timer_int.h"
extern u8 u8_global_IMUStepNumber;
extern u8 ChannelHavingTheBus;
IMU IMU_Module;
 u32 m;
//#define NULL (void*)0
extern enum_Semaphore Semaphore;
extern void IMU_SUCCESS_HANDLER (void); /*call back called when user data requested is available*/
extern void IMU_FAILURE_HANDLER (void);
u8 ASCIIdatlen;
u8 z[80]={0};
extern void ConvertToASCII(s16 * DataBuffer,u8 RawDataLength,u8 * ASCIIData,u8 * ASCIIdatalength,u8 FloatingPointPercision);
extern void InitializeArray(u8 * Array,u8 StartIndex,u8 EndIndex);
IMU IMU_Module ;
extern IMU_config_type IMU_Configuration;
extern delay( u32 max);




u8 u8_global_CALC_FLAG =0;
u8 u8_global_deltaTimeFactor =1;

void IMU_Requests_Error_Handler (void)
{


    if (IMU_Module.ModuleStatus == IMU_BUSY && u8_global_CALC_FLAG ==1 ) /*user request + calculate angles*/
    {
        IMU_FAILURE_HANDLER();
        u8_global_deltaTimeFactor ++;
        u8_global_CALC_FLAG =0;
    }

    else if (IMU_Module.ModuleStatus == IMU_BUSY)
    {
        IMU_FAILURE_HANDLER();
        IMU_Module.ModuleStatus = IMU_IDLE;
    }
    else
    {
        u8_global_deltaTimeFactor ++;
        u8_global_CALC_FLAG =0;
    }

}

void  IMU_Requests_Success_Handler (void) /*JOB_DONE CALL BACK FUNCTIONS HANDLS BOTH USER REQUESTS AND THE PERIODIC CALCULATE TASK*/
{

    u8 i;
    if ( IMU_Module.ModuleStatus == IMU_NOT_INITIALIZED )
    {
        u8_global_IMUStepNumber++;
    }
    else
    {
        IMU_privUpdateRawData();
        if (IMU_Module.ModuleStatus == IMU_BUSY && u8_global_CALC_FLAG == 1 ) /*user request + calculate angles*/
        {

            for (i=0;i<7;i++)
            {
                IMU_Module.UserBuffer[i] = ((f32) IMU_Module.RawDataBuffer[i] );
            }
            IMU_SUCCESS_HANDLER();
            IMU_Module.ModuleStatus = IMU_IDLE;

            /* CALCULATE ANGLES AND MAKE FLAG OF DELTA = 1 AFTER CALCULATION */

            IMU_privUpdateRawData();
            IMU_PrivConvertToDegPerSec();
            IMU_PrivConvertToAngle();
            IMU_PrivApplyCompelementaryFilter();

            u8_global_deltaTimeFactor =1;
            u8_global_CALC_FLAG =0;

        }
        else if (IMU_Module.ModuleStatus == IMU_BUSY) /*only user request */
        {

            for (i=0;i<7;i++)
            {
                IMU_Module.UserBuffer[i] = ((f32) IMU_Module.RawDataBuffer[i] );
            }
            IMU_SUCCESS_HANDLER();
            IMU_Module.ModuleStatus = IMU_IDLE;

        }

        else /* periodic  task only */
        {
            /* CALCULATE ANGLES AND MAKE FLAG OF DELTA = 1 AFTER CALCULATION */
            Semaphore = Semaphore_NotAvailable;
            IMU_privUpdateRawData();
            IMU_PrivConvertToDegPerSec();
            IMU_PrivConvertToAngle();
            IMU_PrivApplyCompelementaryFilter();
            //            UART_enumRequestToSend(UART1, 6, temp);
            //            UART_mainFunc ();
            Semaphore = Semaphore_Available;
            u8_global_deltaTimeFactor =1;
            u8_global_CALC_FLAG =0;

        }
    }

}



/****************************USEER_REQUEST_FUNC********************/

IMU_Req_Func_Return IMU_ReqToReceive(f32 * PtrToBuffer,enum_IMU_ReqDataType  RequestedDataType ) //just return most updated value
{
    IMU_Req_Func_Return Error_Validation = IMU_ReqOK ;
    u8 i;

    if (IMU_Module.ModuleStatus != IMU_NOT_INITIALIZED) /*any state rather than being not initialised*/
    {

        switch (RequestedDataType)
        {

        case Roll:

            * PtrToBuffer = IMU_Module.RollPitchYaw[0] ; /*return the most recent reading*/
            IMU_SUCCESS_HANDLER ();

            break;

        case Pitch:

            * PtrToBuffer =  IMU_Module.RollPitchYaw[1]; /*return the most recent reading*/
            IMU_SUCCESS_HANDLER();

            break;

        case Yaw:

            * PtrToBuffer =  IMU_Module.RollPitchYaw[2]; /*return the most recent reading*/
            IMU_SUCCESS_HANDLER();

            break;

        case RPY:

            for (i=0;i<3;i++)
            {
                PtrToBuffer [i] = IMU_Module.RollPitchYaw[i] ;
            }
            IMU_SUCCESS_HANDLER();

            break;


        case RawData:

            if (IMU_Module.ModuleStatus == IMU_IDLE) /*NO Requests are waiting to be handled*/
            {
                IMU_Module.ModuleStatus = IMU_SUSPENDED ;
                IMU_Module.UserBuffer = PtrToBuffer ;
            }
            else
            {
                Error_Validation = IMU_ReqNOT_OK ;
            }

            break;

        }
    }

    else
    {
        Error_Validation = IMU_ReqNOT_OK ;
    }

    return Error_Validation;
}




/****************************MAIN_FUNC********************/

void IMU_MainFunc(void)
{

    u32 a = 0;

    static u8 Periodic_TaskCounter =0;

    millis (Timer2,&m);
    a =m;

    switch (IMU_Module.ModuleStatus) /*handling user requests*/
    {


    case IMU_NOT_INITIALIZED :

        IMU_VidInit();
        break;

    case IMU_IDLE :

        /*No user requests to be handled*/
        break;

    case IMU_SUSPENDED : /* A USER REQUEST OF TYPE RAW DATA IS TO BE HANDLED ,, REQUEST FLAG = 1*/
        if ( I2C_MasterRequestToReceive( IMU_Module.I2C_ChannelID , IMU_Module.IMU_RegisterReadings , RAWDATALENGTH , ( (u8) IMU_Configuration.AD0_status + MODULE_ADRDRESS ) , ACCEL_XOUT_H ,Repeated_Start_Condition)== I2C_RequestAccepted )
        {
            IMU_Module.ModuleStatus = IMU_BUSY ;
        }
        else
        {
            IMU_Module.ModuleStatus = IMU_IDLE ;
            IMU_FAILURE_HANDLER ();
        }

        break ;


    default :

        break;
    }
    if ( IMU_Module.ModuleStatus != IMU_NOT_INITIALIZED )
    {
        Periodic_TaskCounter ++;

        if (Periodic_TaskCounter == IMU_UPDATE_LIMIT ) /* IN NEED TO PERIODICALLY UPDATE THE DATA TO CALCULATE THE ANGLES*/
        {
            if (IMU_Module.ModuleStatus == IMU_BUSY) /* request is being handled NO need for a new request now */
            {
                u8_global_CALC_FLAG =1; /*call calculate function if success*/
            }
            else
            {
                if (( I2C_MasterRequestToReceive( IMU_Module.I2C_ChannelID , IMU_Module.IMU_RegisterReadings , RAWDATALENGTH , ( (u8) IMU_Configuration.AD0_status + MODULE_ADRDRESS ) , ACCEL_XOUT_H ,Repeated_Start_Condition)== I2C_RequestAccepted )&&  ( u8_global_CALC_FLAG == 0 ) )
                {
                    u8_global_CALC_FLAG =1;
                }
                else
                {
                    u8_global_deltaTimeFactor ++ ; /*This factor will be multiplied by the periodicity of calculate angles */
                }

            }

            Periodic_TaskCounter =0;
        }
    }

    millis (Timer2,&m);
    a = m -a ; //the diffrence
    delay (10);


}

/*============================================= IMU Initialization Function =====================================================*/
enum_IMU_InitilizationfunctionStatus IMU_VidInit(void )
{
    u8 Local_u8_Data;
    enum_IMU_PrivCalibrtionResult CalibrationResult = CalibrationNotDone;
    enum_IMU_InitilizationfunctionStatus  InitializationStatus = InitializationNotDone;
    switch ( u8_global_IMUStepNumber )
    {
    /*===================================== to avoid the automatically selected sleep mode ================================ */
    case 0:
        Local_u8_Data = 0x00;
        IMU_Module.I2C_ChannelID = IMU_Configuration.I2C_Channel_Config;
        IMU_Module.ModuleStatus = IMU_NOT_INITIALIZED;
        I2C_MasterRequestToSend(IMU_Module.I2C_ChannelID,&Local_u8_Data,1,( (u8) IMU_Configuration.AD0_status + MODULE_ADRDRESS ),PWR_MGMT_1,Single_Start_Condition);
        break;

        /* ===================================== sample rate division for the internal adc ======================================*/
    case 1:
        Local_u8_Data = 0x07;
        I2C_MasterRequestToSend(IMU_Module.I2C_ChannelID,&Local_u8_Data,1,( (u8) IMU_Configuration.AD0_status + MODULE_ADRDRESS ),SMPLRT_DIV,Single_Start_Condition);
        break;
        /* ==================================== Select the X-gyroscope pll internal clock ======================================*/
    case 2:
        Local_u8_Data = IMU_Configuration.Clock;
        I2C_MasterRequestToSend(IMU_Module.I2C_ChannelID,&Local_u8_Data,1,( (u8) IMU_Configuration.AD0_status + MODULE_ADRDRESS ),PWR_MGMT_1,Single_Start_Condition);
        break;
        /* ==================================== Setting the DLPF to 0 to synch the gyro and accel readings ======================================*/
    case 3:
        Local_u8_Data = 0x00;
        I2C_MasterRequestToSend(IMU_Module.I2C_ChannelID,&Local_u8_Data,1,( (u8) IMU_Configuration.AD0_status + MODULE_ADRDRESS ),CONFIG,Single_Start_Condition);
        break;

        /* ==================================== Accelerometer full scale range set to +/- 2g ======================================*/
    case 4:
        Local_u8_Data = IMU_Configuration.Accelerometer_Range;
        I2C_MasterRequestToSend(IMU_Module.I2C_ChannelID,&Local_u8_Data,1,( (u8) IMU_Configuration.AD0_status + MODULE_ADRDRESS ),ACCEL_CONFIG,Single_Start_Condition);
        break;
        /* ==================================== Gyroscope full scale range set to 250 degree/sec ======================================*/

    case 5:
        Local_u8_Data = IMU_Configuration.Gyroscope_Range;
        I2C_MasterRequestToSend(IMU_Module.I2C_ChannelID,&Local_u8_Data,1,( (u8) IMU_Configuration.AD0_status + MODULE_ADRDRESS ),GYRO_CONFIG,Single_Start_Condition);
        break;

    default :
        CalibrationResult = IMU_PrivCalibrate();
        if ( CalibrationResult == CalibrationDone )
        {
            InitializationStatus = InitializationDone;
            IMU_Module.ModuleStatus = IMU_IDLE;

        }
        break;
    }
    return InitializationStatus;
}
