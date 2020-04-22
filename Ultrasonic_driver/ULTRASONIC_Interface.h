/*
 * ULTRASONIC.h
 *
 *  Created on: Oct 8, 2019
 *      Author: mohamed magdi
 */

#ifndef ULTRASONIC_INTERFACE_H_
#define ULTRASONIC_INTERFACE_H_

#include "GPIO_Int.h"

#define  OUT_OF_RANGE 0

typedef enum
{
    ULTRASONIC_ok ,
    ULTRASONIC_not_ok

}Ultrasonic_CheckType;

#define TRIG 			0				//TRIG PIN IS PIN0 IN MICROCONTROLLER'S PORT
#define ECHO 			1				//ECHO PIN IS PIN1 IN MICROCONTROLLER'S PORT


 extern void ULTRASONIC_Init(void);


 extern Ultrasonic_CheckType enum_Ultrasonic_UserRequest (u32 * Ptr_UserBuffer);

 extern void UltrasonicMainFunc (void);







#endif /* ULTRASONIC_INTERFACE_H_ */
