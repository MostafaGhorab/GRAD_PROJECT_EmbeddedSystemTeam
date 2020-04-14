/*
 * GPT_config.c
 *
 *  Created on: 24 Nov 2019
 *      Author: WEST
 */


#include "timer_priv.h"
#include "timer_config.h"



/*****************************************Configuration options*********************************/
/*
 1-Choose between (Enabled-Disabled) to either enable this timer ,set its configurations, enable its ISR

 2-Choose between (Timer_32bit-Timer_16bit) for timer resolution needed

 3-choose between(OneShotTimer,Timer_RTC, Timer_TimeCaptur, Timer_PWM , WaitForTrigger_16bit ,InputEdgeTime,PeriodicTimer) to set timer mode of operation

 4-choose the prescaler which is an 8 bit value between  (0 and 0xFF) to divide the frequency with a value = (prescaler +1)
 */


GPT_config config_array [NumberOfTimers] =
{
 {Enabled,Timer_32bit,PeriodicTimer,0x00}, //for timer 0
 {Enabled,Timer_32bit,PeriodicTimer,0xFF}, //for timer 1
 {Enabled,Timer_32bit,PeriodicTimer,NoPrescaler}, //for timer 2
 {Enabled,Timer_32bit,PeriodicTimer,NoPrescaler}, //for timer 3
 {Enabled,Timer_32bit,PeriodicTimer,NoPrescaler}, //for timer 4
 {Enabled,Timer_16bit,PeriodicTimer,NoPrescaler}, //for timer 5



};
