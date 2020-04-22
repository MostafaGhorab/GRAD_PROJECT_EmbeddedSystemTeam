/*
 * main.c
 *
 *  Created on: Apr 2, 2020
 *      Author: dodo_
 */

#include "Port.h"
#include "utils.h"
#include "std_types.h"
#include "DIO.h"
#include "timer_priv.h"
#include "timer_int.h"
#include "ULTRASONIC_Interface.h"

//void toggle (void)
//{
//    static u32 countr =0;
//    if (countr %2 ==0)
//    {
//        Dio_WriteChannel( PORTB_PIN0, 1 );
//    }
//    else
//    {
//        Dio_WriteChannel( PORTB_PIN0, 0);
//    }
//    countr ++;
//
//}
 u32 gettter;
 u32 buffereco ;
 void delay (u32 a)
 {
     volatile u32 i;

     for (i=0;i<a;i++)
     {

     }
 }
void main(void)
{

    Port_Init();
    Port_SetPinMode(PORTB_PIN0,0x00); //Trigger
    Port_SetPinMode(PORTF_PIN0,0x07); //ECHO

    GPT_Init();
    ULTRASONIC_Init();

    enum_Ultrasonic_UserRequest(&buffereco);


    GPT_Start(Timer1, 5);






       while(1)
       {

           enum_Ultrasonic_UserRequest(&buffereco);
       }
}
