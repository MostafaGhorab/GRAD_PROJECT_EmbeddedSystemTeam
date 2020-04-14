/*
 * timer_priv.h
 *
 *  Created on: 2 Nov 2019
 *      Author: Ghorab
 */

#ifndef TIMER_PRIV_H_
#define TIMER_PRIV_H_

#include "std_types.h"

#define GPT_GEN_REG(Timer_NUM,OFFSET)                  Timer_Bases[Timer_NUM]+OFFSET

#define GPT_GEN_EnableREG(Num,Base)                    Timer_EnableOffsets[Num]+Base

#define NumberOfTimers 6

/**************************************************Timers' Labels*******************************************/

#define Timer0 0
#define Timer1 1
#define Timer2 2
#define Timer3 3
#define Timer4 4
#define Timer5 5

/**************************************************Interrupt Vector Numbers*******************************************/
#define  Timer0_InterruptVectorNum 19
#define  Timer1_InterruptVectorNum 21
#define  Timer2_InterruptVectorNum 23
#define  Timer3_InterruptVectorNum 35
#define  Timer4_InterruptVectorNum 70
#define  Timer5_InterruptVectorNum 92

/************************************************************************/
#define Timer_CountUP   0
#define Timer_CountDOWN 1
#define NoPrescaler 0x00

/**************************************************Timer status*************************************************/

typedef enum
{
    Disabled =0,
    Enabled
}Timer_Status;

/**************************************************Timer Resolution***********************************************/

typedef enum
{
    Timer_16bit =16,
    Timer_32bit=32
}Timer_Resolution;

/**************************************************OPeration modes**************************************************/

typedef enum
{
    PeriodicTimer =       0,

    OneShotTimer=         1,

    Timer_RTC   =         2,

    Timer_TimeCapture=    3,

    Timer_PWM   =         4,

    WaitForTrigger_16bit= 5,

    InputEdgeTime  =      6
}Timer_OperationMode;


typedef struct
{
    Timer_Status Status ;
    Timer_Resolution Resolution;
    Timer_OperationMode OperationMode;
    u8 Prescaler ;

}GPT_config;










#endif /* TIMER_PRIV_H_ */
