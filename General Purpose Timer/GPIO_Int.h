/*
 * GPIO_Int.H
 *
 *  Created on: Oct 6, 2019
 *      Author: AhmedShawky
 */

#ifndef GPIO_INT_H_
#define GPIO_INT_H_

#include "std_types.h"
#include "Mem_Map.h"

#define PORTS_NUMBER 6

typedef enum{
	OKAY,
	NOKAY
}enumerr;
typedef enum{
	PORTA_PINS_NUM=8,
	PORTB_PINS_NUM=8,
	PORTC_PINS_NUM=8,
	PORTD_PINS_NUM=8,
	PORTE_PINS_NUM=6,
	PORTF_PINS_NUM=5
}enumPinsPerPorts;

#define GPIO_GEN_REG(PORT_NUM,OFFSET)                    PORTS_BASES[PORT_NUM]+OFFSET

#define GPIO_UNLOCK_VAL                                   0x4C4F434B




//===============================================PORTA PINS=================================================================================
#define PORTA_PIN0                0
#define PORTA_PIN1                1
#define PORTA_PIN2                2
#define PORTA_PIN3                3
#define PORTA_PIN4                4
#define PORTA_PIN5                5
#define PORTA_PIN6                6
#define PORTA_PIN7                7
//===============================================PORTB PINS=================================================================================
#define PORTB_PIN0                8
#define PORTB_PIN1                9
#define PORTB_PIN2                10
#define PORTB_PIN3                11
#define PORTB_PIN4                12
#define PORTB_PIN5                13
#define PORTB_PIN6                14
#define PORTB_PIN7                15
//===============================================PORTC PINS=================================================================================
#define PORTC_PIN0                16
#define PORTC_PIN1                17
#define PORTC_PIN2                18
#define PORTC_PIN3                19
#define PORTC_PIN4                20
#define PORTC_PIN5                21
#define PORTC_PIN6                22
#define PORTC_PIN7                23
//===============================================PORTD PINS=================================================================================
#define PORTD_PIN0                24
#define PORTD_PIN1                25
#define PORTD_PIN2                26
#define PORTD_PIN3                27
#define PORTD_PIN4                28
#define PORTD_PIN5                29
#define PORTD_PIN6                30
#define PORTD_PIN7                31
//===============================================PORTE PINS=================================================================================
#define PORTE_PIN0                32
#define PORTE_PIN1                33
#define PORTE_PIN2                34
#define PORTE_PIN3                35
#define PORTE_PIN4                36
#define PORTE_PIN5                37
//===============================================PORTF PINS=================================================================================
#define PORTF_PIN0                38
#define PORTF_PIN1                39
#define PORTF_PIN2                40
#define PORTF_PIN3                41
#define PORTF_PIN4                42
//=====================================================PORTS===================================================================================
#define PORTA   0
#define PORTB   1
#define PORTC   2
#define PORTD   3
#define PORTE   4
#define PORTF   5




void GPIO_Init(void);
enumerr GPIO_enumSetPinVal(u8 PinNum,u8 val); // to set a specific pin data to 1 or 0
enumerr GPIO_enumSetPinDir(u8 copy_u8pinNum,u8 copy_u8direction);
enumerr GPIO_enumGetPortVal(u8 copy_u8portNum,u8* u8Ptr_PortValue);
enumerr GPIO_enumSetPortVal(u8 copy_u8portNum,u8 val);
enumerr GPIO_enumGetPinVal(u8 PinNum,u8* u8Ptr_PinValue);








#endif /* GPIO_INT_H_ */
