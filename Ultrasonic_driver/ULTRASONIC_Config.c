/*
 * ULTRASONIC_Config.c
 *
 *  Created on: 31 Mar 2020
 *      Author: Ghorab
 *
 *
 *
 */
#include "timer_priv.h"
#include "ULTRASONIC_priv.h"

//   if (check return dummy value)
//   {
//       no sorrunding
//       request again
//   }
//   {
//       print value or make certain action action
//   }
/*
 *
 * Extra call back is added to handle the case of no sorroundings instead of having a dumy variable to check in main app
 */


void user_call_back (void)
{


}
void user_no_sor_handler(void)
{


}




ULTRASONIC_Config UltrasonicConfiguration  = { Timer0 , user_call_back , user_no_sor_handler };


