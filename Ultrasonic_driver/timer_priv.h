/*
 * timer_priv.h
 *
 *  Created on: 2 Nov 2019
 *      Author: Ghorab
 */

#ifndef TIMER_PRIV_H_
#define TIMER_PRIV_H_

#include "std_types.h"
#include "timer_int.h"

#define GPT_GEN_REG(Timer_NUM,OFFSET)                  Timer_Bases[Timer_NUM]+OFFSET

#define GPT_GEN_EnableREG(Num,Base)                    Timer_EnableOffsets[Num]+Base

#define NUMBER_OF_TIMERS 6
#define COUNT_UP 1
#define CAPTURE_MODE 0x3
#define EDGE_TIME_MODE 0x1

/**************************************************Timers' Numbers*******************************************/

#define Timer0 (u8)0
#define Timer1 (u8)1
#define Timer2 (u8)2
#define Timer3 (u8)3
#define Timer4 (u8)4
#define Timer5 (u8)5

/**************************************************Interrupt Vector Numbers*******************************************/
#define  Timer0_InterruptVectorNum 19
#define  Timer1_InterruptVectorNum 21
#define  Timer2_InterruptVectorNum 23
#define  Timer3_InterruptVectorNum 35
#define  Timer4_InterruptVectorNum 70
#define  Timer5_InterruptVectorNum 92

/************************************************************************/
#define Timer_CountUP   1
#define Timer_CountDOWN 0
#define NO_PRESCALER 0x00





typedef struct
{
    u8 TimerNumber ;
    Timer_Resolution Resolution;
    Timer_OperationMode OperationMode;
    u8 Prescaler ;
    void (*HandlerPtr)(void) ;


}GPT_config;










#endif /* TIMER_PRIV_H_ */
