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

typedef enum
{
    Positive_edge =0,
    Negative_edge =1,
    Both_edges    =3

}Triggering_event;



/**************************************************Timer Resolution***********************************************/

typedef enum
{
    Timer_16bit = 16,
    Timer_32bit = 32
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


/*
 * Description: The initialisation function initialise the enabled timers with the values in the config.c and config.h files
 * Parameters: none
 * Return: void
 */

void GPT_Init(void);

/*
 * Description: The Start function enables the timer to start counting since its called
 * Parameters: It takes two parameters ,parameter one is the timer number which will start counting,parameter two is the time to take certain periodic action in ms
 * Return: Check_type check that the entered timer number and the time to be measured are within range
 */

Check_type GPT_Start (u8 copy_u8TimerNumber, u32 copy_u32TimeOut_ms); //with maximum of 53,000 ms -53 seconds- for 80 mhz clock while for 16 mhz 268 seconds

/*
 * Description: The Resume function enables the timer to continue counting after a timer stop was called it continues counting the remaining time for the action then continue normally
 * Parameters: It takes one parameter ,the timer number
 * Return: Check_type check that the entered timer number is within range
 */

Check_type GPT_Resume (u8 copy_u8TimerNumber);

/*
 * Description: The Stop function stops the counter after certain number of counts
 * Parameters: It takes one parameter,the timer number
 * Return: Check_type check that the entered timer number is within range
 */

Check_type GPT_Stop (u8 copy_u8TimerNumber);


/*
 * Description: millis function starts certain chosen timer and return the time since firstly called
 * Parameters: It takes the timer number used and the a getter to return time
 * Return: Check_type check that the entered timer number is within range
 */
Check_type millis (u8 copy_u8TimerNumber,u32 *ptr_u32_TimeGetter);




void GPT_DiffrenceBetweenTwoEvents(u8 copy_u8TimerNumber,u32 * buffer) ;

void GPT_StopTimeCapture (u8 copy_u8TimerNumber) ;

Check_type us_count (u8 copy_u8TimerNumber) ;

void us_count_stop (u8 copy_u8TimerNumber);

void delay_func_us (u8 timer_num, u32 delay);


#endif /* TIMER_INT_H_ */
