/*
 * timer_int.h
 *
 *  Created on: 2 Nov 2019
 *      Author: WEST
 */

#ifndef TIMER_INT_H_
#define TIMER_INT_H_

#include "std_types.h"
#include "timer_priv.h"

typedef enum
{
    GPT_OKAY =0,
    GPT_NOKAY

}Check_type;

/*
 * Description: The initialisation function initialise the enabled timers with the values in the config.c and config.h files
 * Parameters: It takes the array of structure in the config.c file
 * Return: void
 */

void GPT_Init(GPT_config copy_GPT_configArray[]);

/*
 * Description: The Start function enables the timer to start counting since its called
 * Parameters: It takes two parameters ,parameter one is the timer number which will start counting,parameter two is the time to take certain periodic action in ms
 * Return: Check_type check that the entered timer number and the time to be measured are within range
 */

Check_type GPT_Start (u8 copy_u8TimerNumber, u32 copy_u32TimeOut_ms); //with maximum of 53,000 ms -53 seconds- for 80 mhz clock while for 16 mhz 268 seconds

/*
 * Description: The Resume function enables the timer to continue counting after a timer stop was called it continues counting the remaining time for the action then continue normally
 * Parameters: It takes two parameters ,parameter one is the timer number which will start counting,two is the remaining ticks which is the return (The getter of stop function)
 * Return: Check_type check that the entered timer number is within range
 */

Check_type GPT_Resume (u8 copy_u8TimerID,u32 copy_u32TicksRemaining);

/*
 * Description: The Stop function stops the counter after certain number of counts
 * Parameters: It takes two parameters,the first is counter number, the second is a getter to the remaining number of ticks for the action to be counted
 * Return: Check_type check that the entered timer number is within range
 */

Check_type GPT_Stop (u8 copy_u8TimerNumber, u32 * getter_u32ptrTicksRemaining);

void Timer0A_LocalISR(void (*Local_ISR)(void));
void Timer1A_LocalISR(void (*Local_ISR)(void));
void Timer2A_LocalISR(void (*Local_ISR)(void));
void Timer3A_LocalISR(void (*Local_ISR)(void));
void Timer4A_LocalISR(void (*Local_ISR)(void));
void Timer5A_LocalISR(void (*Local_ISR)(void));




#endif /* TIMER_INT_H_ */
