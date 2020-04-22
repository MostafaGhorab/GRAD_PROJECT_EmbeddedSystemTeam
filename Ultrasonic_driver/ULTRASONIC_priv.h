/*
 * ULTRASONIC_priv.h
 *
 *  Created on: 30 Mar 2020
 *      Author: Ghorab
 */

#ifndef ULTRASONIC_PRIV_H_
#define ULTRASONIC_PRIV_H_

#include "std_types.h"




typedef enum
{

    ULTRASONIC_IDLE,
    ULTRASONIC_SUSPENDED,
    ULTRASONIC_BUSY

}ULTRASONIC_STATE;

typedef struct
{
    u8 Timer_Number ;

  void  (* User_Call_Back_Handler ) (void);

  void  (* User_Call_Back_No_surrounding ) (void);

}ULTRASONIC_Config ;

extern void ULTRASONIC_STARTUP(void);



#endif /* ULTRASONIC_PRIV_H_ */
