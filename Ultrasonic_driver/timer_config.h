/*
 * timer_config.h
 *
 *  Created on: 2 Nov 2019
 *      Author: WEST
 */

#ifndef TIMER_CONFIG_H_
#define TIMER_CONFIG_H_
#include "timer_priv.h"

/* Note the prescaler can yield proper division only when the timer is configured as a down counter. the clock is divided by prescaler value + 1 */


#define CLOCK_FREQUENCY (u32)16000000             /*configure according to the system's clock frequency*/


#define CountingIncrement   Timer_CountUP /*Choose whether the gpt will count up or down*/ //always up for  input capture unit//

#define TRIGGER_EVENT Both_edges

#define NUMBER_OF_ENABLED_TIMERS 3



#endif /* TIMER_CONFIG_H_ */
