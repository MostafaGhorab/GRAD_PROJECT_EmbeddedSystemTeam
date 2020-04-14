/*
 * GPIO_configure.h
 *
 *  Created on: Oct 12, 2019
 *      Author: AhmedShawky
 */

#ifndef GPIO_CONFIGURE_H_
#define GPIO_CONFIGURE_H_
#include "GPIO_Priv.h"
//============================================PINS ARE CONFIGURED AS INPUT OR OUTPUT===================================================================
#define PORTA_PIN0_DIREC             INPUT
#define PORTA_PIN1_DIREC             INPUT
#define PORTA_PIN2_DIREC             INPUT
#define PORTA_PIN3_DIREC             INPUT
#define PORTA_PIN4_DIREC             INPUT
#define PORTA_PIN5_DIREC             INPUT
#define PORTA_PIN6_DIREC             INPUT
#define PORTA_PIN7_DIREC             INPUT

#define PORTB_PIN0_DIREC             OUTPUT
#define PORTB_PIN1_DIREC             OUTPUT
#define PORTB_PIN2_DIREC             OUTPUT
#define PORTB_PIN3_DIREC             OUTPUT
#define PORTB_PIN4_DIREC             OUTPUT
#define PORTB_PIN5_DIREC             OUTPUT
#define PORTB_PIN6_DIREC             OUTPUT
#define PORTB_PIN7_DIREC             OUTPUT

#define PORTC_PIN0_DIREC             OUTPUT
#define PORTC_PIN1_DIREC             OUTPUT
#define PORTC_PIN2_DIREC             OUTPUT
#define PORTC_PIN3_DIREC             OUTPUT
#define PORTC_PIN4_DIREC             OUTPUT
#define PORTC_PIN5_DIREC             OUTPUT
#define PORTC_PIN6_DIREC             OUTPUT
#define PORTC_PIN7_DIREC             OUTPUT

#define PORTD_PIN0_DIREC             INPUT
#define PORTD_PIN1_DIREC             INPUT
#define PORTD_PIN2_DIREC             INPUT
#define PORTD_PIN3_DIREC             INPUT
#define PORTD_PIN4_DIREC             INPUT
#define PORTD_PIN5_DIREC             INPUT
#define PORTD_PIN6_DIREC             INPUT
#define PORTD_PIN7_DIREC             INPUT

#define PORTE_PIN0_DIREC             INPUT
#define PORTE_PIN1_DIREC             INPUT
#define PORTE_PIN2_DIREC             INPUT
#define PORTE_PIN3_DIREC             INPUT
#define PORTE_PIN4_DIREC             INPUT
#define PORTE_PIN5_DIREC             INPUT

#define PORTF_PIN0_DIREC             OUTPUT
#define PORTF_PIN1_DIREC             OUTPUT
#define PORTF_PIN2_DIREC             OUTPUT
#define PORTF_PIN3_DIREC             OUTPUT
#define PORTF_PIN4_DIREC             OUTPUT

//============================================PINS ARE CONFIGURED AS INTERRUPT EN/DIS===================================================================

//*****************************************  MAKE SURE TO MAKE THE ENABELED AS INPUTS FIRST   *******************************************

#define PORTA_PIN0_INT               DISABELED
#define PORTA_PIN1_INT               DISABELED
#define PORTA_PIN2_INT               DISABELED
#define PORTA_PIN3_INT               DISABELED
#define PORTA_PIN4_INT               DISABELED
#define PORTA_PIN5_INT               ENABELED
#define PORTA_PIN6_INT               ENABELED
#define PORTA_PIN7_INT               ENABELED

#define PORTB_PIN0_INT               DISABELED
#define PORTB_PIN1_INT               DISABELED
#define PORTB_PIN2_INT               DISABELED
#define PORTB_PIN3_INT               DISABELED
#define PORTB_PIN4_INT               DISABELED
#define PORTB_PIN5_INT               DISABELED
#define PORTB_PIN6_INT               DISABELED
#define PORTB_PIN7_INT               DISABELED

#define PORTC_PIN0_INT               DISABELED
#define PORTC_PIN1_INT               DISABELED
#define PORTC_PIN2_INT               DISABELED
#define PORTC_PIN3_INT               DISABELED
#define PORTC_PIN4_INT               DISABELED
#define PORTC_PIN5_INT               DISABELED
#define PORTC_PIN6_INT               DISABELED
#define PORTC_PIN7_INT               DISABELED

#define PORTD_PIN0_INT               DISABELED
#define PORTD_PIN1_INT               DISABELED
#define PORTD_PIN2_INT               DISABELED
#define PORTD_PIN3_INT               DISABELED
#define PORTD_PIN4_INT               DISABELED
#define PORTD_PIN5_INT               DISABELED
#define PORTD_PIN6_INT               DISABELED
#define PORTD_PIN7_INT               DISABELED

#define PORTE_PIN0_INT               DISABELED
#define PORTE_PIN1_INT               DISABELED
#define PORTE_PIN2_INT               DISABELED
#define PORTE_PIN3_INT               DISABELED
#define PORTE_PIN4_INT               DISABELED
#define PORTE_PIN5_INT               DISABELED


#define PORTF_PIN0_INT               DISABELED
#define PORTF_PIN1_INT               DISABELED
#define PORTF_PIN2_INT               DISABELED
#define PORTF_PIN3_INT               DISABELED
#define PORTF_PIN4_INT               DISABELED


#define SUBPRIORITY_STATUS            DISABELED
//THE SUBPRIORITY LEVELS NUMBER GOES FROM 1 TO 8,IT'S EITHER 1,2,4 OR 8
#define SUBPRIORITY_LEVEL_NUM             1


//THE GROUP PRIORITY IS FROM 0 T0 7 ,IF 2 PORTS HAVE THE SAME GP PRIORITIES AND BOTH ARE PENDING WE LOOK AT THE SUBPRIORITIES
//WHILE IF BOTH HAVE THE SAME GP PRIORITY THEY CAN NOT INTERRUPT EACH OTHER EVEN IF ONE HAVE A HIGHER SUB PRIORITY

// YOU CAN CHOOSE BETWEEN 8 PRIORITY LEVELS:{HIGHEST , VERY_HIGH , HIGH , MODERATE_HIGH , MODERATE , LOW,VERY_LOW , LOWEST }

#define PORTA_GP_PRI                     HIGHEST
#define PORTB_GP_PRI                     LOWEST
#define PORTC_GP_PRI                     LOWEST
#define PORTD_GP_PRI                     LOWEST
#define PORTE_GP_PRI                     LOWEST
#define PORTF_GP_PRI                     LOWEST

//PLEASE TAKE INTO CONCIDERATION THAT WHEN USING THE SUBPRIOITY_LEVEL_NUM AS 1 YOU DO USE THE FULL 3-BITS OF THE GP_PRI NUMBER ASSIGNED
//WHILE WHEN CHOOSING THE SUBPRIOITY_LEVEL_NUM AS 2 YOU TAKE THE HIGHEST 2 BITS AS GP_PRI AND THE LEAST AS THE SUB_PRI
//WHEN CHOOSING 4 YOU TAKE THE LOWEST 2 AS SUB_PRI AND THE HIGHEST AS GP_PRI


//=====================================================CONFIGURING PORTS AS ENABELED OR DISABELED======================================================================
#define PORTA_STATUS                 ENABELED
#define PORTB_STATUS                 ENABELED
#define PORTC_STATUS                 ENABELED
#define PORTD_STATUS                 ENABELED
#define PORTE_STATUS                 ENABELED
#define PORTF_STATUS                 ENABELED

#endif /* GPIO_CONFIGURE_H_ */
