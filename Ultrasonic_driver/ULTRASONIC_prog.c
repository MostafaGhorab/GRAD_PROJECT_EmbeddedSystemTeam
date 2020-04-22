/*
 * US.C
 *
 *  Created on: Mar 22, 2020
 *      Author: mohamed magdi
 */
#include "std_types.h"
#include "ULTRASONIC_Interface.h"
#include "ULTRASONIC_Config.h"
#include "ULTRASONIC_Interface.h"
#include "ULTRASONIC_priv.h"
#include "GPIO_Int.h"
#include "timer_priv.h"
#include "timer_int.h"


#define SOUND_SPEED_CM_US 0.0034
#define HALF_WAY 2
extern ULTRASONIC_Config UltrasonicConfiguration ;
u8 JobDoneFlag = 0;
extern u32 gettter;




typedef struct
{

    ULTRASONIC_STATE UltraSonic_State ;

    u32 *Data_Ptr;

    void (* ULTRASONIC_CallBackFunction) (void);

    void (*ULTRASONIC_NoSorrHandler) (void);



}Struct_ULTRASONIC_Module;

Struct_ULTRASONIC_Module ULTRASONIC_Module ;

void WaitingEventHandler (void)
{

    if (ULTRASONIC_Module.UltraSonic_State == ULTRASONIC_BUSY)
    {
        JobDoneFlag =1;
    }

}



void ULTRASONIC_Init(void)

{

    ULTRASONIC_Module.UltraSonic_State = ULTRASONIC_IDLE ;
    ULTRASONIC_Module.ULTRASONIC_CallBackFunction = UltrasonicConfiguration.User_Call_Back_Handler ;
    ULTRASONIC_Module.ULTRASONIC_NoSorrHandler = UltrasonicConfiguration.User_Call_Back_No_surrounding ;
}


Ultrasonic_CheckType enum_Ultrasonic_UserRequest (u32 * Ptr_UserBuffer)
{
    Ultrasonic_CheckType Error_FuncValidation = ULTRASONIC_not_ok;

    if (ULTRASONIC_Module.UltraSonic_State == ULTRASONIC_IDLE )
    {

        ULTRASONIC_Module.Data_Ptr =  Ptr_UserBuffer ;
        ULTRASONIC_Module.UltraSonic_State = ULTRASONIC_SUSPENDED ;
        Error_FuncValidation = ULTRASONIC_ok ;

    }

    return Error_FuncValidation ;
}



void UltrasonicMainFunc (void)
{

    static u8 counter = 0;



    switch ( ULTRASONIC_Module.UltraSonic_State)
    {

    case ULTRASONIC_IDLE :

        break;

    case ULTRASONIC_SUSPENDED :


        ULTRASONIC_Module.UltraSonic_State = ULTRASONIC_BUSY ;
        counter = 0;
        ULTRASONIC_STARTUP();
        GPT_DiffrenceBetweenTwoEvents(Timer0,ULTRASONIC_Module.Data_Ptr);

        break ;


    case ULTRASONIC_BUSY :
        counter ++;
        if (counter == 5) //periodicity should be 5ms
        {

            if (JobDoneFlag ==1) //perdiodicity should be more 24 ms the max time for a signal to go to 4 meters and return back if job is not done in this time so nothing here
            {


                *(ULTRASONIC_Module.Data_Ptr)  = ( (*(ULTRASONIC_Module.Data_Ptr) ) * SOUND_SPEED_CM_US) /2  ;//converrt time to distance,assign distance for the buffer pointer
                ULTRASONIC_Module.ULTRASONIC_CallBackFunction ();
                ULTRASONIC_Module.UltraSonic_State = ULTRASONIC_IDLE ;
                JobDoneFlag = 0 ;

            }
            else //NO SOURRONDINGS
            {

                GPT_StopTimeCapture(Timer0);
                //*(ULTRASONIC_Module.Data_Ptr) =  OUT_OF_RANGE ;  //a dummy value can be used in application for checking for body existance for the nearest 4 meters
                ULTRASONIC_Module.ULTRASONIC_NoSorrHandler ();
                ULTRASONIC_Module.UltraSonic_State = ULTRASONIC_IDLE ;

            }
            break;
        }

    }




}




