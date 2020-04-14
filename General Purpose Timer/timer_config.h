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


#define ClockFrequency (u32)16000000             /*configure according to the system's clock frequency*/


#define CountingIncrement          Timer_CountDOWN /*Choose whether the gpt will count up or down*/



#endif /* TIMER_CONFIG_H_ */
