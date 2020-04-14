/*
 * IMU_Config.c
 *
 *  Created on: 11 Mar 2020
 *      Author: WEST
 */

#include "IMU_int.h"

void IMU_SUCCESS_HANDLER (void) /*call back called when user data requested is available*/
{

}

void IMU_FAILURE_HANDLER (void) /*failed to handle user request*/
{

}
IMU_config_type IMU_Configuration = {ACCEL_2g,GYRO_250,FIFO_DISABLED,Internal_8MHz_oscillator,AD0_LOW,0,IMU_SUCCESS_HANDLER,IMU_FAILURE_HANDLER};








