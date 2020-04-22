/*
 * ULTRASONIC_Priv.c
 *
 *  Created on: 31 Mar 2020
 *      Author: Ghorab
 */
#include "port.h"
#include "DIO.h"
#include "GPIO_Int.h"
#include "ULTRASONIC_priv.h"
#include "ULTRASONIC_Config.h"
#include "timer_priv.h"
#include "Timer0_MemMap.h"
extern const u32 Timer_Bases[NUMBER_OF_TIMERS];
extern u32 gettter;
extern u32 Number_of_overflows[NUMBER_OF_TIMERS];

volatile u32 i;



void ULTRASONIC_STARTUP(void)
{
    //WRITE INPUT LOW TO TRIG
   Dio_WriteChannel( TRIGGER_PIN, 0 );
   delay_func_us(Timer2,32);

//   for (i=0;i<32;i++)
//   {
//       asm(" NOP ")  ;
//   }



    //WRITE INPUT HIGH TO TRIG
    Dio_WriteChannel( TRIGGER_PIN, 1 );
    delay_func_us(Timer2,11);

//    for (i=0;i<160;i++)
//    {
//        asm(" NOP ")  ;
//    }


    //WRITE INPUT LOW TO TRIG
    Dio_WriteChannel( TRIGGER_PIN, 0 );
    delay_func_us(Timer2,32);

//    for (i=0;i<32;i++)
//    {
//        asm(" NOP ")  ;
//    }

    us_count_stop(Timer2);

}


