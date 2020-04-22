/*
 * GPT_config.c
 *
 *  Created on: 24 Nov 2019
 *      Author: WEST
 */


#include "timer_priv.h"
#include "timer_config.h"
#define NULL (void *)0

extern void WaitingEventHandler (void);
extern void toggle (void);
void Tfunc (void)
{


}
extern void UltrasonicMainFunc (void);




/*********NOTES**********/
/*
 * MAXIMUM NUMBER OF TIMERS IS 6 TIMERS
 * CONFIGURE THE NUMBER OF ENABLED TIMERS FROM THE CONFIG.H FILE
 */

/*****************************************Configuration options*********************************/
/*
 1-Choose between (Timer0- Timer5)

 2-Choose between (Timer_32bit-Timer_16bit) for timer resolution needed

 3-choose between(OneShotTimer,Timer_RTC, Timer_TimeCaptur, Timer_PWM , WaitForTrigger_16bit ,InputEdgeTime,PeriodicTimer) to set timer mode of operation

 4-choose the prescaler which is an 8 bit value between  (0 and 0xFF) to divide the frequency with a value = (prescaler +1)

 5-Pointer to the function that will be executed each period
 */

//use timer zero for icu//


GPT_config config_array [NUMBER_OF_ENABLED_TIMERS] =
{
 {Timer0,Timer_16bit,Timer_TimeCapture,NO_PRESCALER,WaitingEventHandler},
 {Timer1,Timer_32bit,PeriodicTimer,NO_PRESCALER,UltrasonicMainFunc},
 {Timer2,Timer_32bit,PeriodicTimer,NO_PRESCALER,Tfunc}

};
