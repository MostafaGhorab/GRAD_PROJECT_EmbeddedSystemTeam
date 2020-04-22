/*
 * timer_prog.c
 *
 *  Created on: 2 Nov 2019
 *      Author: Ghorab
 */

#include "std_types.h"
#include "utils.h"
#include "DIO.h"
#include "DIO_Priv.h"
#include "Timer0_MemMap.h"
#include "timer_priv.h"
#include "timer_int.h"
#include "timer_config.h"



/*
 * some calculations : for 16 bit mode : to get a 1 second delay : 4ms parameter for start function + a static counter of 250 no prescaler
 * for 16 bit mode with 0xFF prescaler put 1000ms to get 1 second delay preferably to be count down
 * for 32 bit mode just put 1000ms to start function no prescaler
 *
 */

#define NumOfInterruptBitsPerRegister 32

#define NULL (void*)0
#define GPT_16BIT 0x4
#define GPT_32BIT 0x00
#define SECONDS_TO_US (u32)1000000

 const u32 Timer_Bases[NUMBER_OF_TIMERS]={Timer0_BASE,Timer1_BASE,Timer2_BASE,Timer3_BASE,Timer4_BASE,Timer5_BASE};

const u32 Timer_EnableOffsets[3]={EN0_offset,EN1_offset,EN2_offset};

static u8 Timer_u8LookUpArray [NUMBER_OF_ENABLED_TIMERS] = {0};

static void (*GPT_voidIsrs[NUMBER_OF_TIMERS]) (void) = {NULL,NULL,NULL,NULL,NULL,NULL};

extern GPT_config config_array [NUMBER_OF_ENABLED_TIMERS] ; /*array of configurations initialised for all timers*/

u8 Timer_InterruptVectorNumbers[NUMBER_OF_TIMERS] = {Timer0_InterruptVectorNum,Timer1_InterruptVectorNum,Timer2_InterruptVectorNum,Timer3_InterruptVectorNum,Timer4_InterruptVectorNum,Timer5_InterruptVectorNum};

static u32 Timer_u32TicksRemaining[NUMBER_OF_TIMERS]={0} ;

static u32 Timer_u32TimeOutTicks[NUMBER_OF_TIMERS] ={0};

 u32 Number_of_overflows[NUMBER_OF_TIMERS]={0} ; //for millis function

 u32 * diffrence[NUMBER_OF_TIMERS];

 u8 JobDoneflag =0;

 u8  first_Call_flag =0;

 u8 uscount_u8first_time_flag=0;

  u32 temp_data[2];
   u8 counter =0;






 void GPT_Init(void)
 {

     u8 Local_LoopIndex;
     u8 Local_InterruptEnableNum=0;
     u8 Local_vectorNum;

     for (Local_LoopIndex=0;Local_LoopIndex<NUMBER_OF_ENABLED_TIMERS;Local_LoopIndex++)  /*as the fist element in configuration is not for sure timer 0 so lookup table is used */
     {
         Timer_u8LookUpArray[Local_LoopIndex] = config_array[Local_LoopIndex].TimerNumber ;
     }


     for (Local_LoopIndex=0;Local_LoopIndex<NUMBER_OF_ENABLED_TIMERS;Local_LoopIndex++) /*loop index indicates the timer number*/
     {

         GPT_voidIsrs[ Timer_u8LookUpArray[Local_LoopIndex]] = config_array[ Timer_u8LookUpArray[Local_LoopIndex]].HandlerPtr;

         SET_BIT(RCGCTIMER, Timer_u8LookUpArray[Local_LoopIndex]); //enables GPT clock in Free Run Mode for the timer number
         CLR_BIT(GPTMCTL( Timer_u8LookUpArray[Local_LoopIndex] ),0);            //disable Timer

         if (config_array[ Timer_u8LookUpArray[Local_LoopIndex]].Resolution == Timer_16bit)
         {
             GPTMCFG( Timer_u8LookUpArray[Local_LoopIndex]) = GPT_16BIT;                 //setup 16 bit resolution
         }
         else
         {
             GPTMCFG( Timer_u8LookUpArray[Local_LoopIndex] ) = GPT_32BIT;         //setup 32 bit resolution
         }

         GPTMTAPR( Timer_u8LookUpArray[Local_LoopIndex] ) = config_array[ Timer_u8LookUpArray[Local_LoopIndex]].Prescaler ;     //set the prescaler

         if (config_array[ Timer_u8LookUpArray[Local_LoopIndex]].OperationMode == PeriodicTimer)
         {

             SET_BIT(GPTMTAMR( Timer_u8LookUpArray[Local_LoopIndex] ),1);     //two bits for choosing periodic mode
             CLR_BIT(GPTMTAMR( Timer_u8LookUpArray[Local_LoopIndex] ),0);

             GPTMTAMR(Timer_u8LookUpArray[Local_LoopIndex] ) |= CountingIncrement<<4 ; //count up or down


         }
         else if (config_array[Timer_u8LookUpArray[Local_LoopIndex]].OperationMode == OneShotTimer)
         {

         }
         else if (config_array[Timer_u8LookUpArray[Local_LoopIndex]].OperationMode == Timer_RTC)
         {

         }
         else if (config_array[Timer_u8LookUpArray[Local_LoopIndex]].OperationMode == Timer_TimeCapture)
         {
             GPTMCFG( Timer_u8LookUpArray[Local_LoopIndex]) = GPT_16BIT;      //setup 16 bit resolution

             GPTMTAMR(Timer_u8LookUpArray[Local_LoopIndex] ) |= ( (COUNT_UP<<4) | (EDGE_TIME_MODE<<2) | (CAPTURE_MODE<<0) ) ; //0x17 //count up + time edge mode in capture mode

             GPTMTAPR( Timer_u8LookUpArray[Local_LoopIndex] ) = 0xFFFF;
             GPTMCTL( Timer_u8LookUpArray[Local_LoopIndex] ) |= (TRIGGER_EVENT<<2) ; //set bit g2 and 3




         }
         else if (config_array[Timer_u8LookUpArray[Local_LoopIndex]].OperationMode == Timer_PWM)
         {

         }
         else if (config_array[Timer_u8LookUpArray[Local_LoopIndex]].OperationMode == WaitForTrigger_16bit)
         {

         }
         else if (config_array[Timer_u8LookUpArray[Local_LoopIndex] ].OperationMode == InputEdgeTime)
         {

         }

         Local_vectorNum = Timer_InterruptVectorNumbers[ Timer_u8LookUpArray[Local_LoopIndex]]; //to get the interrupt vector number of the timer
         Local_InterruptEnableNum=0;
         while (Local_vectorNum >= NumOfInterruptBitsPerRegister)        //to get the interrupt bit mask corresponding to this timer
         {
             Local_vectorNum = Local_vectorNum -NumOfInterruptBitsPerRegister; //to get the needed enable bit for this timer as timers' vectors numbers are not contiguous
             Local_InterruptEnableNum ++; //to get the number of enable register (0 or 1 or 2) for corresponding timer

         }
         SET_BIT(EN(Local_InterruptEnableNum),Local_vectorNum );         //enable the global interrupt

         SET_BIT(GPTMIMR( Timer_u8LookUpArray[Local_LoopIndex]),0);      //enable the time out interrupt for timer
         SET_BIT(GPTMIMR( Timer_u8LookUpArray[Local_LoopIndex]),2);      //enable the time out interrupt for timer

     }


 }



Check_type GPT_Start (u8 copy_u8TimerNumber, u32 copy_u32TimeOut_ms)
{
    Check_type Local_u8ErrorFuncValidation = GPT_OKAY;
    u64 Local_u64MaxNumberOfticks =0;
    Local_u64MaxNumberOfticks|=(((u64)1)<<config_array[copy_u8TimerNumber].Resolution );  //to get 2 ^ resolution -Max number of ticks

                                                                //This equation gets the maximum time in ms for certain clock frequency and resolution
    if ( (copy_u8TimerNumber> Timer5) || (copy_u32TimeOut_ms > ((Local_u64MaxNumberOfticks * 1000 * (config_array[copy_u8TimerNumber].Prescaler+1) )/CLOCK_FREQUENCY) ) )
    {
        Local_u8ErrorFuncValidation =  GPT_NOKAY ;
    }

    else
    {
        GPTMTAILR(copy_u8TimerNumber) =  ( (CLOCK_FREQUENCY / (1000 *(config_array[copy_u8TimerNumber].Prescaler +1) ) ) * copy_u32TimeOut_ms ) ;
                                                                /*this equation gets the number of ticks needed for the desired time using desired time in seconds and clock frequency*/
        Timer_u32TimeOutTicks[copy_u8TimerNumber] =  ( (CLOCK_FREQUENCY / (1000 *(config_array[copy_u8TimerNumber].Prescaler +1) ) ) * copy_u32TimeOut_ms ) ; //copy the value in global variable

        SET_BIT(GPTMCTL(copy_u8TimerNumber),0);                //enable timer to start counting
    }

    return Local_u8ErrorFuncValidation ;

}

Check_type GPT_Stop (u8 copy_u8TimerNumber)
{
    Check_type Local_u8ErrorFuncValidation = GPT_OKAY;

    if (copy_u8TimerNumber > Timer5)
    {
        Local_u8ErrorFuncValidation = GPT_NOKAY;
    }

    else
    {
        Timer_u32TicksRemaining[copy_u8TimerNumber] = (GPTMTAILR(copy_u8TimerNumber) - GPTMTAR(copy_u8TimerNumber)) ; /*returns the remaining ticks for time out
                                                             (overflow no of ticks) - The current no of ticks in the counter register(Number of ticks elapsed in the register)
                                                             This  value is passed to the resume function*/

        CLR_BIT(GPTMCTL(copy_u8TimerNumber),0);            /*disable timerA*/
    }

    return Local_u8ErrorFuncValidation ;

}


Check_type GPT_Resume (u8 copy_u8TimerNumber)
{
    Check_type Local_u8ErrorFuncValidation = GPT_OKAY;

    if (copy_u8TimerNumber> Timer5)
    {
        Local_u8ErrorFuncValidation = GPT_NOKAY ;
    }
    else
    {

        GPTMTAV(copy_u8TimerNumber) =0;      /*reset the timer counter next cycle*/

        if (CountingIncrement == Timer_CountUP)
        {
            GPTMTAILR(copy_u8TimerNumber) = Timer_u32TicksRemaining[copy_u8TimerNumber]; //set the value with remaining ticks if up
        }
        else
        {
            GPTMTAILR(copy_u8TimerNumber) = Timer_u32TimeOutTicks[copy_u8TimerNumber] - Timer_u32TicksRemaining[copy_u8TimerNumber]; //set the value with remaining ticks if down
        }

        SET_BIT(GPTMCTL(copy_u8TimerNumber),0);     //enables the timer
    }

    return Local_u8ErrorFuncValidation;

}


Check_type millis (u8 copy_u8TimerNumber,u32 *ptr_u32_TimeGetter)
{

    static Check_type Local_u8ErrorFuncValidation  ;
    static u8 local_u8first_time_flag =0; //flag indicates whether its first time the function called or not
    if (local_u8first_time_flag  == 0 )
    {
        Local_u8ErrorFuncValidation =   GPT_Start(copy_u8TimerNumber, 1);
        local_u8first_time_flag ++;
    }

    *ptr_u32_TimeGetter = Number_of_overflows[copy_u8TimerNumber];


    return  Local_u8ErrorFuncValidation ;

}

void GPT_StopTimeCapture (u8 copy_u8TimerNumber)
{
    CLR_BIT(GPTMCTL(copy_u8TimerNumber),0);                //stop timer
    CLR_BIT(GPTMIMR( copy_u8TimerNumber),2); //STOP THE INTERRUPT


    first_Call_flag = 0;
}


u32 GPT_Time_Capture (u8 copy_u8TimerNumber) //after 268 seconds overflow occours
{

   // return( ( GPTMTAR(copy_u8TimerNumber) * SECONDSreturn( ( GPTMTAR(copy_u8TimerNumber)_TO_US)  / CLOCK_FREQUENCY ); //CAPTURES THE VALUE OF THE LAST EVENT OCCOURS in us
    return(  GPTMTAR(copy_u8TimerNumber) );
}

void GPT_DiffrenceBetweenTwoEvents(u8 copy_u8TimerNumber,u32 * buffer) //when counter value is 1 take the value
{
    //static u32 temp_data[2];
    //static u8 counter =0;

    if (first_Call_flag == 0)
    {
        diffrence[copy_u8TimerNumber] = buffer ;
        first_Call_flag ++;
        counter = 0;
        //  GPTMTAV(copy_u8TimerNumber) =0;      /*reset the timer counter next cycle*/
        // GPTMTAILR(copy_u8TimerNumber)  = 0;
        SET_BIT(GPTMIMR(copy_u8TimerNumber),2);                 //enable interrupt
        GPTMCTL( copy_u8TimerNumber ) |= (TRIGGER_EVENT<<2) ; //set bit g2 and 3
        SET_BIT(GPTMCTL(copy_u8TimerNumber),0);                //enable timer


    }
    else
    {


        if (counter % 2 != 0 )
        {

            *(diffrence[copy_u8TimerNumber]) =  temp_data[1] - temp_data[0] ;


            first_Call_flag =0;
            // CLR_BIT(GPTMIMR( copy_u8TimerNumber),2); //STOP THE INTERRUPT
             CLR_BIT(GPTMCTL(copy_u8TimerNumber),0);                //stop timer

            GPT_voidIsrs[copy_u8TimerNumber] (); //CALL BACK FUNC
        }
        counter ++;

    }



}




Check_type us_count (u8 copy_u8TimerNumber)
{

    static Check_type Local_u8ErrorFuncValidation  ;
     //flag indicates whether its first time the function called or not
    if (uscount_u8first_time_flag  == 0 )
    {

        uscount_u8first_time_flag ++; //value inside GTMAILR SHOULD BE 16 in clock freq 16 mhz and no prescaller attached
        //GPTMTAILR(copy_u8TimerNumber) =  0xFFFFFFFF;//( (CLOCK_FREQUENCY / (1000000 *(config_array[copy_u8TimerNumber].Prescaler +1) ) ) * 1 ) ;
                                                                /*this equation gets the number of ticks needed for the desired time using desired time in seconds and clock frequency*/
        //Timer_u32TimeOutTicks[copy_u8TimerNumber] =  ( (CLOCK_FREQUENCY / (1000000 *(config_array[copy_u8TimerNumber].Prescaler +1) ) ) * 1 ) ; //copy the value in global variable
        CLR_BIT(GPTMIMR( copy_u8TimerNumber),0); //STOP THE INTERRUPT
        SET_BIT(GPTMCTL(copy_u8TimerNumber),0);                //enable timer to start counting

    }


   // *ptr_u32_TimeGetter = Number_of_overflows[copy_u8TimerNumber];


    return  Local_u8ErrorFuncValidation ;

}

void delay_func_us (u8 timer_num, u32 delay)
{

    us_count (timer_num); //start timer

    u32 snap_shot ;




        snap_shot =  GPTMTAR(timer_num) ;

    while( (( GPTMTAR(timer_num)- snap_shot) < (delay *( CLOCK_FREQUENCY/1000000)) ) );


}

void us_count_stop (u8 copy_u8TimerNumber)
{

    CLR_BIT(GPTMCTL(copy_u8TimerNumber),0);
    uscount_u8first_time_flag =0;

}











/**************************************************************TIMERs  CALLBACK FUNCTIONS**********************************************************************/

void Timer0A_ISR (void)
{
    u32 *NO;

//    if (  GET_BIT( GPTMRIS(Timer0),0 ) ==1  ) //periodic mode
//    {
//    Number_of_overflows[Timer0]++ ;     //accumulates the number of overflows since the program started used with millis function
//    GPT_voidIsrs[Timer0] ();
  // SET_BIT(GPTMICR(Timer0),0);         //clear the interrupt flag by setting it to one
//
//    }
//     if ( GET_BIT( GPTMRIS(Timer0),2 ) == 1) //input capture mode
//    {
    temp_data[counter] = GPT_Time_Capture (Timer0) ;
        GPT_DiffrenceBetweenTwoEvents(Timer0 , NO);
        SET_BIT(GPTMICR(Timer0),2);

  //  }

}


void Timer1A_ISR (void)
{
    Number_of_overflows[Timer1]++ ;
    GPT_voidIsrs[Timer1] ();
    SET_BIT(GPTMICR(Timer1),0);    //clear the interrupt flag by setting it to one
}

void Timer2A_ISR (void)
{
    Number_of_overflows[Timer2]++ ;
    GPT_voidIsrs[Timer2] ();
    SET_BIT(GPTMICR(Timer2),0);    //clear the interrupt flag by setting it to one

}
void Timer3A_ISR (void)
{
    Number_of_overflows[Timer3]++ ;
    GPT_voidIsrs[Timer3] ();
    SET_BIT(GPTMICR(Timer3),0);    //clear the interrupt flag by setting it to one
}
void Timer4A_ISR (void)
{
    Number_of_overflows[Timer4]++ ;
    GPT_voidIsrs[Timer4] ();
    SET_BIT(GPTMICR(Timer4),0);    //clear the interrupt flag by setting it to one
}
void Timer5A_ISR (void)
{
    Number_of_overflows[Timer5]++ ;
    GPT_voidIsrs[Timer5] ();
    SET_BIT(GPTMICR(Timer5),0);    //clear the interrupt flag by setting it to one
}














/*************************************************************************TESTING*****************************************************************************/

//void LedToggle (void)
//{
//
//
//}
//
//void main (void)
//
// {
//
//    u32 x=0;
//    u32 * getter;
//    getter = &x;
//    GPIO_Init ();
//     u32 i;
//
//
//
//
//     GPIO_enumSetPinVal(PORTF_PIN1,1);
//
//      GPT_Init();
//      millis(Timer1, getter) ;
//      GPT_Start(Timer0, 20000);
//
//    while ( Number_of_overflows[Timer0] ==0);
//    GPIO_enumSetPinVal(PORTF_PIN2,0);
//    millis(Timer1, getter) ;
//
//   // GPT_Start (Timer1, 1000);
////    for (i=0;i<9900000;i++) //8 seconds passed and 2 left
////    {
////
////    }
////        GPIO_enumSetPinVal(PORTF_PIN3, HIGH);
////        GPT_Stop (Timer1);
//    //for (i=0;i<9900000;i++); //8 seconds delay
////    {
////
////    }
//     //for (i=0;i<9900000;i++);
//    //millis(Timer0, getter) ;
//
////    GPIO_enumSetPinVal(PORTF_PIN3, LOW);
//
//
//
//   // GPT_Resume (Timer1);
//
//
//
//
//    while(1)
//    {
//
//    }
//
//
// }
//
