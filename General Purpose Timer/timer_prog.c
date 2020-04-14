/*
 * timer_prog.c
 *
 *  Created on: 2 Nov 2019
 *      Author: Ghorab
 */

#include "std_types.h"
#include "GPIO_Utils.h"
#include "GPIO_Int.h"
#include "GPIO_Priv.h"
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
#define NULL 0

void (*Timer0A_Periodic_ISR)(void)  = NULL;
void (*Timer1A_Periodic_ISR)(void)  = NULL;
void (*Timer2A_Periodic_ISR)(void)  = NULL;
void (*Timer3A_Periodic_ISR)(void)  = NULL;
void (*Timer4A_Periodic_ISR)(void)  = NULL;
void (*Timer5A_Periodic_ISR)(void)  = NULL;

u32 Timer_Bases[NumberOfTimers]={Timer0_BASE,Timer1_BASE,Timer2_BASE,Timer3_BASE,Timer4_BASE,Timer5_BASE};

u32 Timer_EnableOffsets[3]={EN0_offset,EN1_offset,EN2_offset};

extern GPT_config config_array [NumberOfTimers] ; //array of configurations initialised for all timers

u8 Timer_InterruptVectorNumbers[NumberOfTimers] = {Timer0_InterruptVectorNum,Timer1_InterruptVectorNum,Timer2_InterruptVectorNum,Timer3_InterruptVectorNum,Timer4_InterruptVectorNum,Timer5_InterruptVectorNum};







void GPT_Init(GPT_config copy_GPT_configArray[])
{
//    Timer0_Config.Status  = Enabled;
//    Timer0_Config.Resolution = Timer_32bit;
//    Timer0_Config.OperationMode = PeriodicTimer;
    u8 Local_LoopIndex;
    u8 Local_InterruptEnableNum=0;
    u8 Local_vectorNum;
    for (Local_LoopIndex=0;Local_LoopIndex<NumberOfTimers;Local_LoopIndex++) //loop index indicates the timer number
    {

        if (copy_GPT_configArray[Local_LoopIndex].Status == Enabled)
        {

            SET_BIT(RCGCTIMER,Local_LoopIndex); //enables GPT clock in Free Run Mode for the timer number

            if (copy_GPT_configArray[Local_LoopIndex].Resolution == Timer_16bit)
            {
                CLR_BIT(GPTMCTL(Local_LoopIndex),0);            //disable Timer
                GPTMCFG(Local_LoopIndex) = 0x4;                 //setup 16 bit resolution


            }
            else if (copy_GPT_configArray[Local_LoopIndex].Resolution == Timer_32bit)
            {
                CLR_BIT(GPTMCTL(Local_LoopIndex),0);            //disable Timer
                GPTMCFG(Local_LoopIndex) = 0x00000000;         //setup 32 bit resolution
            }

            GPTMTAPR(Local_LoopIndex) = copy_GPT_configArray[Local_LoopIndex].Prescaler ;     //set the prescaler

            if (copy_GPT_configArray[Local_LoopIndex].OperationMode == PeriodicTimer)
            {
                SET_BIT(GPTMTAMR(Local_LoopIndex),1);     //two bits for choosing periodic mode
                CLR_BIT(GPTMTAMR(Local_LoopIndex),0);

                if (CountingIncrement == Timer_CountUP) //its by default counting down so condition only checking if up count
                {
                    SET_BIT(GPTMTAMR(Local_LoopIndex),4);
                }


            }
            else if (copy_GPT_configArray[Local_LoopIndex].OperationMode == OneShotTimer)
            {

            }
            else if (copy_GPT_configArray[Local_LoopIndex].OperationMode == Timer_RTC)
            {

            }
            else if (copy_GPT_configArray[Local_LoopIndex].OperationMode == Timer_TimeCapture)
            {

            }
            else if (copy_GPT_configArray[Local_LoopIndex].OperationMode == Timer_PWM)
            {

            }
            else if (copy_GPT_configArray[Local_LoopIndex].OperationMode == WaitForTrigger_16bit)
            {

            }
            else if (copy_GPT_configArray[Local_LoopIndex].OperationMode == InputEdgeTime)
            {

            }

            Local_vectorNum = Timer_InterruptVectorNumbers[Local_LoopIndex]; //to get the interrupt vector number of the timer
            Local_InterruptEnableNum=0;
            while (Local_vectorNum >= NumOfInterruptBitsPerRegister)        //to get the interrupt bit mask corresponding to this timer
            {
                Local_vectorNum = Local_vectorNum -NumOfInterruptBitsPerRegister; //to get the needed enable bit for this timer as timers' vectors numbers are not contiguous
                Local_InterruptEnableNum ++; //to get the number of enable register (0 or 1 or 2) for corresponding timer

            }
            SET_BIT(EN(Local_InterruptEnableNum),Local_vectorNum );  //enable the global interrupt

            SET_BIT(GPTMIMR(Local_LoopIndex),0);      //enable the time out interrupt for timer

        }


    }








}



Check_type GPT_Start (u8 copy_u8TimerNumber, u32 copy_u32TimeOut_ms)
{
    if (copy_u8TimerNumber> Timer5)
       {
           return GPT_NOKAY ;
       }
    u64 Local_u64MaxNumberOfticks =0;
    Local_u64MaxNumberOfticks|=((u64)1<<config_array[copy_u8TimerNumber].Resolution );  //to get 2 ^ resolution -Max number of ticks

    //This equation gets the maximum time in ms for certain clock frequency and resolution
    if (copy_u32TimeOut_ms > ((Local_u64MaxNumberOfticks * 1000 * (config_array[copy_u8TimerNumber].Prescaler+1) )/ClockFrequency)  )
    {
        return GPT_NOKAY ;
    }


    GPTMTAILR(copy_u8TimerNumber) =  ( (ClockFrequency / (1000 *(config_array[copy_u8TimerNumber].Prescaler +1) ) ) * copy_u32TimeOut_ms ) ;      //count up until this value of ticks
                                              //this equation gets the number of ticks needed for the desired time using desired time in seconds and clock frequency
    SET_BIT(GPTMCTL(copy_u8TimerNumber),0);                //enable timer to start counting


    return GPT_OKAY ;

}

Check_type GPT_Stop (u8 copy_u8TimerNumber, u32 * getter_u32ptrTicksRemaining)
{
    if (copy_u8TimerNumber> Timer5)
    {
        return GPT_NOKAY ;
    }




        *getter_u32ptrTicksRemaining = (GPTMTAILR(copy_u8TimerNumber) - GPTMTAR(copy_u8TimerNumber)) ; /*returns the remaining ticks for time out
                                                             (overflow no of ticks) - The current no of ticks in the counter register(Number of ticks elapsed in the register)
                                                             This  value is passed to the resume function*/
        CLR_BIT(GPTMIMR(copy_u8TimerNumber),0);             //disable the time out for timer A interrupt
        CLR_BIT(GPTMCTL(copy_u8TimerNumber),0);            //disable timerA




    return GPT_OKAY ;

}


Check_type GPT_Resume (u8 copy_u8TimerNumber,u32 copy_u32TicksRemaining)
{
    if (copy_u8TimerNumber> Timer5)
    {
        return GPT_NOKAY ;
    }


        SET_BIT(GPTMIMR(copy_u8TimerNumber),4);   //enables Timer0A match interrupt
        SET_BIT(GPTMTAMR(copy_u8TimerNumber),5);   //timer A match interrupt enable
        GPTMTAMATCHR(copy_u8TimerNumber) = copy_u32TicksRemaining ;  //set the matching value
        SET_BIT(GPTMCTL(copy_u8TimerNumber),0);  //enables the timer







    return GPT_OKAY;

}








/**************************************************************TIMER 0 CALLBACK FUNCTIONS**********************************************************************/
void Timer0A_LocalISR(void (*Local_ISR)(void))
{

    Timer0A_Periodic_ISR =  Local_ISR;

}

void Timer0A_ISR (void)
{
    if (GET_BIT(GPTMRIS(Timer0),0) ==1 ) // check if the interrupt is driven by the time out
    {
        Timer0A_Periodic_ISR  ();
        SET_BIT(GPTMICR(Timer0),0);         //clear the interrupt flag by setting it to one
    }
    else if (GET_BIT(GPTMRIS(Timer0),4) ==1 ) // check if the interrupt is driven by match
    {
        Timer0A_Periodic_ISR  ();
        SET_BIT(GPTMICR(Timer0),4);        //clear the match interrupt flag by setting it to one

        CLR_BIT(GPTMCTL(Timer0),0);      //disable the timer
        SET_BIT(GPTMIMR(Timer0),0);     //enable the time out mode again
        CLR_BIT(GPTMIMR(Timer0),4);    //disable Timer0A match interrupt
        CLR_BIT(GPTMTAMR(Timer0),5);  //disable timer A match interrupt
        SET_BIT(GPTMCTL(Timer0),0);  //enables the timer

    }
}

/**************************************************************REST OF TIMERs  CALLBACK FUNCTIONS**********************************************************************/
void Timer1A_LocalISR(void (*Local_ISR)(void))
{

    Timer1A_Periodic_ISR =  Local_ISR;

}
void Timer2A_LocalISR(void (*Local_ISR)(void))
{

    Timer2A_Periodic_ISR =  Local_ISR;

}
void Timer3A_LocalISR(void (*Local_ISR)(void))
{

    Timer3A_Periodic_ISR =  Local_ISR;

}
void Timer4A_LocalISR(void (*Local_ISR)(void))
{

    Timer4A_Periodic_ISR =  Local_ISR;

}
void Timer5A_LocalISR(void (*Local_ISR)(void))
{



    Timer5A_Periodic_ISR =  Local_ISR;

}

void Timer1A_ISR (void)
{
    if (GET_BIT(GPTMRIS(Timer1),0) ==1 ) // check if the interrupt is driven by the time out
    {
        Timer1A_Periodic_ISR  ();
        SET_BIT(GPTMICR(Timer1),0);    //clear the interrupt flag by setting it to one
    }
    else if (GET_BIT(GPTMRIS(Timer1),4) ==1 ) // check if the interrupt is driven by match
    {
        Timer1A_Periodic_ISR  ();
        SET_BIT(GPTMICR(Timer1),4);     //clear the match interrupt flag by setting it to one

        CLR_BIT(GPTMCTL(Timer1),0);      //disable the timer
        SET_BIT(GPTMIMR(Timer1),0);     //enable the time out mode again
        CLR_BIT(GPTMIMR(Timer1),4);    //disable Timer0A match interrupt
        CLR_BIT(GPTMTAMR(Timer1),5);  //disable timer A match interrupt
        SET_BIT(GPTMCTL(Timer1),0);  //enables the timer

    }
}

void Timer2A_ISR (void)
{
    if (GET_BIT(GPTMRIS(Timer2),0) ==1 ) // check if the interrupt is driven by the time out
    {
        Timer2A_Periodic_ISR  ();
        SET_BIT(GPTMICR(Timer2),0);    //clear the interrupt flag by setting it to one
    }
    else if (GET_BIT(GPTMRIS(Timer2),4) ==1 ) // check if the interrupt is driven by match
    {
        Timer2A_Periodic_ISR  ();
        SET_BIT(GPTMICR(Timer2),4);     //clear the match interrupt flag by setting it to one

        CLR_BIT(GPTMCTL(Timer2),0);      //disable the timer
        SET_BIT(GPTMIMR(Timer2),0);     //enable the time out mode again
        CLR_BIT(GPTMIMR(Timer2),4);    //disable Timer0A match interrupt
        CLR_BIT(GPTMTAMR(Timer2),5);  //disable timer A match interrupt
        SET_BIT(GPTMCTL(Timer2),0);  //enables the timer

    }
}
void Timer3A_ISR (void)
{
    if (GET_BIT(GPTMRIS(Timer3),0) ==1 ) // check if the interrupt is driven by the time out
    {
        Timer3A_Periodic_ISR  ();
        SET_BIT(GPTMICR(Timer3),0);    //clear the interrupt flag by setting it to one
    }
    else if (GET_BIT(GPTMRIS(Timer3),4) ==1 ) // check if the interrupt is driven by match
    {
        Timer3A_Periodic_ISR  ();
        SET_BIT(GPTMICR(Timer3),4);     //clear the match interrupt flag by setting it to one

        CLR_BIT(GPTMCTL(Timer3),0);      //disable the timer
        SET_BIT(GPTMIMR(Timer3),0);     //enable the time out mode again
        CLR_BIT(GPTMIMR(Timer3),4);    //disable Timer0A match interrupt
        CLR_BIT(GPTMTAMR(Timer3),5);  //disable timer A match interrupt
        SET_BIT(GPTMCTL(Timer3),0);  //enables the timer

    }
}
void Timer4A_ISR (void)
{
    if (GET_BIT(GPTMRIS(Timer4),0) ==1 ) // check if the interrupt is driven by the time out
    {
        Timer4A_Periodic_ISR  ();
        SET_BIT(GPTMICR(Timer4),0);    //clear the interrupt flag by setting it to one
    }
    else if (GET_BIT(GPTMRIS(Timer4),4) ==1 ) // check if the interrupt is driven by match
    {
        Timer4A_Periodic_ISR  ();
        SET_BIT(GPTMICR(Timer4),4);     //clear the match interrupt flag by setting it to one

        CLR_BIT(GPTMCTL(Timer4),0);      //disable the timer
        SET_BIT(GPTMIMR(Timer4),0);     //enable the time out mode again
        CLR_BIT(GPTMIMR(Timer4),4);    //disable Timer0A match interrupt
        CLR_BIT(GPTMTAMR(Timer4),5);  //disable timer A match interrupt
        SET_BIT(GPTMCTL(Timer4),0);  //enables the timer

    }
}
void Timer5A_ISR (void)
{
    if (GET_BIT(GPTMRIS(Timer5),0) ==1 ) // check if the interrupt is driven by the time out
    {
        Timer5A_Periodic_ISR  ();
        SET_BIT(GPTMICR(Timer5),0);    //clear the interrupt flag by setting it to one
    }
    else if (GET_BIT(GPTMRIS(Timer5),4) ==1 ) // check if the interrupt is driven by match
    {
        Timer5A_Periodic_ISR  ();
        SET_BIT(GPTMICR(Timer5),4);     //clear the match interrupt flag by setting it to one

        CLR_BIT(GPTMCTL(Timer5),0);      //disable the timer
        SET_BIT(GPTMIMR(Timer5),0);     //enable the time out mode again
        CLR_BIT(GPTMIMR(Timer5),4);    //disable Timer0A match interrupt
        CLR_BIT(GPTMTAMR(Timer5),5);  //disable timer A match interrupt
        SET_BIT(GPTMCTL(Timer5),0);  //enables the timer

    }
}


/*************************************************************************TESTING*****************************************************************************/

void LedToggle (void)
{
   //static u8 i =0;
    static u8 counter =1;
   // i++;
   // if (i == 250)
   // {
   // i=0;


    if ( (counter %2) == 0)
    {
        GPIO_enumSetPinVal(PORTF_PIN1, HIGH);
    }
    else
    {
        GPIO_enumSetPinVal(PORTF_PIN1, LOW);
    }
    counter ++;
    //SET_BIT(GPTMICR_Timer0,0); //clear the interrupt flag
   // }
}

void main (void)

 {

    u32 x=0;
    u32 * getter;
    getter = &x;
    GPIO_Init ();





    //Timer3A_LocalISR(LedToggle);
    Timer0A_LocalISR(LedToggle);
    GPT_Init(config_array);
    GPT_Start (Timer0, 1000);
    //    for (i=0;i<9900000;i++)
    //                  {
    //
    //                  }
    //    GPIO_enumSetPinVal(PORTF_PIN3, HIGH);
   // GPT_Stop (Timer5, getter);
    //    for (i=0;i<9900000;i++) //8.2 seconds delay
    //                     {
    //
    //                     }
    //
    //
    //    GPIO_enumSetPinVal(PORTF_PIN3, LOW);
    //
    //
    //
   //GPT_Resume (Timer5,*getter);





    while(1)
    {


        //     while (GET_BIT(GPTMRIS_Timer0,0) ==0 ); //pull until flag is set
        //
        //     SET_BIT(GPTMICR_Timer0,0); //clear the interrupt flag
        //
        //     GPIO_enumSetPinVal(PORTF_PIN1, HIGH);
        //     while (GET_BIT(GPTMRIS_Timer0,0) ==0 ); //pull until flag is set
        //
        //          SET_BIT(GPTMICR_Timer0,0); //clear the interrupt flag
        //
        //          GPIO_enumSetPinVal(PORTF_PIN1, LOW);






    }


 }
//#if (Timer0_Status == Enabled)
//
// SET_BIT(RCGCTIMER,0); //enables gptimer0 in Free Run Mode
//
//
//    #if (Timer0_Resolution ==Timer_16bit )
//
//
//
//
//
//
//    #elif (Timer0_Resolution ==Timer_32bit )
//
//    CLR_BIT(GPTMCTL_Timer0,0);            //disable Timer0
//    GPTMCFG_Timer0 = 0x00000000;         //setup 32 bit resolution
//
//        #if( OperationMode == PeriodicTimer)
//        SET_BIT(GPTMTAMR_Timer0,1);     //two bits for choosing periodic mode
//        CLR_BIT(GPTMTAMR_Timer0,0);
//
//
//            #if(CountingIncrement == Timer_CountUP) //its by default counting down
//            SET_BIT(GPTMTAMR_Timer0,4);
//
//
//
//            #elif (CountingIncrement == Timer_CountDOWN)  //by default counting down
//           // GPTMTAILR_Timer0 =   CountDownStartValue;     //count starting from this value decrementing to zero if reset value goes to 0xFFFF.FFFF by default
//
//            #endif
//
//
//        SET_BIT(EN0,Timer0_InterruptVectorNum );  //enable the global interrupt
//        SET_BIT(GPTMIMR_Timer0,0);             //enable the time out for timer A interrupt
//
//
//        #elif  ( OperationMode == Timer0_PWM)
//
//        #endif
//
//
//    #endif
//
//#endif
//
//
//#if (Timer1_Status == Enabled)
//#endif
