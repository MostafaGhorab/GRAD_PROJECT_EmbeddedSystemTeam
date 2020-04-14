/*
 * GPIO_Pogram.c
 *
 *  Created on: Oct 12, 2019
 *      Author: AhmedShawky
 */
#include "GPIO_Utils.h"
#include "GPIO_Int.h"
#include "GPIO_configure.h"
#include "Mem_Map.h"

u32 PORTS_BASES[PORTS_NUMBER]={GPIO_PORTA_BASE,GPIO_PORTB_BASE,GPIO_PORTC_BASE,GPIO_PORTD_BASE,GPIO_PORTE_BASE,GPIO_PORTF_BASE};
extern u8 GPIODR2R_Val[PORTS_NUMBER];
extern u8 GPIODR4R_Val[PORTS_NUMBER];
extern u8 GPIODR8R_Val[PORTS_NUMBER];
extern u8 GPIOODR_Val[PORTS_NUMBER];
extern u8 GPIOPUR_Val[PORTS_NUMBER];
extern u8 GPIOPDR_Val[PORTS_NUMBER];
extern u8 GPIODEN_Val[PORTS_NUMBER];

#define PORTA_DIREC              CONCBIT(PORTA_PIN7_DIREC,PORTA_PIN6_DIREC,PORTA_PIN5_DIREC,PORTA_PIN4_DIREC,PORTA_PIN3_DIREC,PORTA_PIN2_DIREC,PORTA_PIN1_DIREC,PORTA_PIN0_DIREC)
#define PORTB_DIREC              CONCBIT(PORTB_PIN7_DIREC,PORTB_PIN6_DIREC,PORTB_PIN5_DIREC,PORTB_PIN4_DIREC,PORTB_PIN3_DIREC,PORTB_PIN2_DIREC,PORTB_PIN1_DIREC,PORTB_PIN0_DIREC)
#define PORTC_DIREC              CONCBIT(PORTC_PIN7_DIREC,PORTC_PIN6_DIREC,PORTC_PIN5_DIREC,PORTC_PIN4_DIREC,PORTC_PIN3_DIREC,PORTC_PIN2_DIREC,PORTC_PIN1_DIREC,PORTC_PIN0_DIREC)
#define PORTD_DIREC              CONCBIT(PORTD_PIN7_DIREC,PORTD_PIN6_DIREC,PORTD_PIN5_DIREC,PORTD_PIN4_DIREC,PORTD_PIN3_DIREC,PORTD_PIN2_DIREC,PORTD_PIN1_DIREC,PORTD_PIN0_DIREC)
#define PORTE_DIREC              CONCBIT(0,0,PORTE_PIN5_DIREC,PORTE_PIN4_DIREC,PORTE_PIN3_DIREC,PORTE_PIN2_DIREC,PORTE_PIN1_DIREC,PORTE_PIN0_DIREC)
#define PORTF_DIREC              CONCBIT(0,0,0,PORTF_PIN4_DIREC,PORTF_PIN3_DIREC,PORTF_PIN2_DIREC,PORTF_PIN1_DIREC,PORTF_PIN0_DIREC)

enumerr GPIO_enumSetPinVal(u8 PinNum,u8 val)
{
	enumerr enumStateValidation;
	if(PORTA_PIN0<=PinNum&&PinNum<=PORTF_PIN4)
	{
		enumStateValidation=OKAY;
		if(PORTA_PIN0<=PinNum&&PinNum<=PORTA_PIN7)
		{
			GPIO_WRITE_DATA(PORTA,1<<PinNum,val<<PinNum);
		}
		else if (PORTB_PIN0<=PinNum&&PinNum<=PORTB_PIN7)
		{
			GPIO_WRITE_DATA(PORTB,1<<(PinNum-PORTA_PINS_NUM),val<<(PinNum-PORTA_PINS_NUM));
		}
		else if (PORTC_PIN0<=PinNum&&PinNum<=PORTC_PIN7)
		{
			GPIO_WRITE_DATA(PORTC,1<<(PinNum-PORTA_PINS_NUM-PORTB_PINS_NUM),val<<(PinNum-PORTA_PINS_NUM-PORTB_PINS_NUM));
		}
		else if (PORTD_PIN0<=PinNum&&PinNum<=PORTD_PIN7)
		{
			GPIO_WRITE_DATA(PORTD,1<<(PinNum-PORTA_PINS_NUM-PORTB_PINS_NUM-PORTC_PINS_NUM),val<<(PinNum-PORTA_PINS_NUM-PORTB_PINS_NUM-PORTC_PINS_NUM));
		}
		else if (PORTE_PIN0<=PinNum&&PinNum<=PORTE_PIN5)
		{
			GPIO_WRITE_DATA(PORTE,1<<(PinNum-PORTA_PINS_NUM-PORTB_PINS_NUM-PORTC_PINS_NUM-PORTD_PINS_NUM),val<<(PinNum-PORTA_PINS_NUM-PORTB_PINS_NUM-PORTC_PINS_NUM-PORTD_PINS_NUM));
		}
		else if (PORTF_PIN0<=PinNum&&PinNum<=PORTF_PIN4)
		{
			GPIO_WRITE_DATA(PORTF,1<<(PinNum-PORTA_PINS_NUM-PORTB_PINS_NUM-PORTC_PINS_NUM-PORTD_PINS_NUM-PORTE_PINS_NUM),val<<(PinNum-PORTA_PINS_NUM-PORTB_PINS_NUM-PORTC_PINS_NUM-PORTD_PINS_NUM-PORTE_PINS_NUM));
		}
	}

	else
	{
		enumStateValidation=NOKAY;
	}
	return enumStateValidation;
}

enumerr GPIO_enumSetPinDir(u8 copy_u8pinNum,u8 copy_u8direction)
{
	enumerr enumStateValidation = OKAY;
	u32* PtrReqReg;
	if(PORTA_PIN0<=copy_u8pinNum && copy_u8pinNum<=PORTF_PIN4)
	{

		if(PORTA_PIN0<=copy_u8pinNum && copy_u8pinNum<=PORTA_PIN7)
		{
			PtrReqReg = &(GPIODIR(PORTA)) ;
		}
		else if (PORTB_PIN0<=copy_u8pinNum && copy_u8pinNum<=PORTB_PIN7)
		{
			PtrReqReg = &(GPIODIR(PORTB)) ;
			copy_u8pinNum = copy_u8pinNum - PORTA_PINS_NUM ;
		}
		else if (PORTC_PIN0<=copy_u8pinNum && copy_u8pinNum<=PORTC_PIN7)
		{
			PtrReqReg = &(GPIODIR(PORTC)) ;
			copy_u8pinNum = copy_u8pinNum - PORTA_PINS_NUM -PORTB_PINS_NUM ;
		}
		else if (PORTD_PIN0<=copy_u8pinNum && copy_u8pinNum<=PORTD_PIN7)
		{
			PtrReqReg = &(GPIODIR(PORTD)) ;
			copy_u8pinNum = copy_u8pinNum - PORTA_PINS_NUM -PORTB_PINS_NUM - PORTC_PINS_NUM;
		}
		else if (PORTE_PIN0<=copy_u8pinNum && copy_u8pinNum<=PORTE_PIN5)
		{
			PtrReqReg = &(GPIODIR(PORTE)) ;
			copy_u8pinNum = copy_u8pinNum - PORTA_PINS_NUM -PORTB_PINS_NUM - PORTC_PINS_NUM - PORTD_PINS_NUM;

		}
		else if (PORTF_PIN0<=copy_u8pinNum && copy_u8pinNum<=PORTF_PIN4)
		{
			PtrReqReg = &(GPIODIR(PORTF)) ;
			copy_u8pinNum = copy_u8pinNum - PORTA_PINS_NUM -PORTB_PINS_NUM - PORTC_PINS_NUM - PORTD_PINS_NUM -PORTE_PINS_NUM;

		}
	}

	else
	{
		return NOKAY;
	}

	if (copy_u8direction == IN)
			{
			CLR_BIT(*PtrReqReg,copy_u8pinNum);
			}
		else if ( copy_u8direction == OUT)

			{
			SET_BIT( *PtrReqReg,copy_u8pinNum );
			}
		else
	        {
			return NOKAY;

			}
	return  enumStateValidation;

}

void GPIO_Init(void)
{
#if(PORTA_STATUS == ENABELED)
	SET_BIT(RCGCGPIO,PORTA); //ENABLING THE CLOCK
	GPIOLOCK(PORTA)=GPIO_UNLOCK_VAL;
	GPIOCR(PORTA)=0xff;
	GPIOAFSEL(PORTA)=0x0000003F; //enabling the alternate function for porta pin1 and pin0 as they are locked to uart AND FROM [5:2]
	GPIOPCTL(PORTA)=0x00000000;   //selecting the alternate function for these pins as GPIO
	GPIODIR(PORTA)=PORTA_DIREC;    //selecting the direction of the port
	GPIODR2R(PORTA)=GPIODR2R_Val[PORTA];   //SETTING THE VALUES FOR THE REGISTERS IF THEY WERE CONFIGURED AS INPUT
	GPIODR4R(PORTA)=GPIODR4R_Val[PORTA];
	GPIODR8R(PORTA)=GPIODR8R_Val[PORTA];
	GPIODEN(PORTA)=GPIODEN_Val[PORTA];   //ENABLING THE DIGITALLY ENABELED PINS BEFORE OPEN DRAIN SELECTION
	GPIOODR(PORTA)=GPIOODR_Val[PORTA];
	GPIOPDR(PORTA)=GPIOPDR_Val[PORTA];    //SETTING O/P PINS AS EITHER PULLUP OR DOWN
	GPIOPUR(PORTA)=GPIOPUR_Val[PORTA];
#endif
#if(PORTB_STATUS == ENABELED)
	SET_BIT(RCGCGPIO,PORTB);
	GPIOAFSEL(PORTB)=0x0000000C;
	GPIOPCTL(PORTB)=0x00000000;
	GPIODIR(PORTB)=PORTB_DIREC;
	GPIODR2R(PORTB)=GPIODR2R_Val[PORTB];
	GPIODR4R(PORTB)=GPIODR4R_Val[PORTB];
	GPIODR8R(PORTB)=GPIODR8R_Val[PORTB];
	GPIODEN(PORTB)=GPIODEN_Val[PORTB];
	GPIOODR(PORTB)=GPIOODR_Val[PORTB];
	GPIOPDR(PORTB)=GPIOPDR_Val[PORTB];
	GPIOPUR(PORTB)=GPIOPUR_Val[PORTB];
#endif
#if(PORTC_STATUS == ENABELED)
	SET_BIT(RCGCGPIO,PORTC);
	GPIODIR(PORTC)=PORTC_DIREC;
	GPIODR2R(PORTC)=GPIODR2R_Val[PORTC];
	GPIODR4R(PORTC)=GPIODR4R_Val[PORTC];
	GPIODR8R(PORTC)=GPIODR8R_Val[PORTC];
	GPIODEN(PORTC)=GPIODEN_Val[PORTC];
	GPIOODR(PORTC)=GPIOODR_Val[PORTC];
	GPIOPDR(PORTC)=GPIOPDR_Val[PORTC];
	GPIOPUR(PORTC)=GPIOPUR_Val[PORTC];
#endif
#if(PORTD_STATUS == ENABELED)
	SET_BIT(RCGCGPIO,PORTD);
	GPIOLOCK(PORTD)=GPIO_UNLOCK_VAL;
	GPIOCR(PORTD)=0xff;
	GPIOPCTL(PORTD)=0x00000000;
	GPIODIR(PORTD)=PORTD_DIREC;
	GPIODR2R(PORTD)=GPIODR2R_Val[PORTD];
	GPIODR4R(PORTD)=GPIODR4R_Val[PORTD];
	GPIODR8R(PORTD)=GPIODR8R_Val[PORTD];
	GPIODEN(PORTD)=GPIODEN_Val[PORTD];
	GPIOODR(PORTD)=GPIOODR_Val[PORTD];
	GPIOPDR(PORTD)=GPIOPDR_Val[PORTD];
	GPIOPUR(PORTD)=GPIOPUR_Val[PORTD];
#endif
#if(PORTE_STATUS == ENABELED)
	SET_BIT(RCGCGPIO,PORTE);
	GPIOPCTL(PORTE)=0x00000000;
	GPIODIR(PORTE)=PORTE_DIREC;
	GPIODR2R(PORTE)=GPIODR2R_Val[PORTE];
	GPIODR4R(PORTE)=GPIODR4R_Val[PORTE];
	GPIODR8R(PORTE)=GPIODR8R_Val[PORTE];
	GPIODEN(PORTE)=GPIODEN_Val[PORTE];
	GPIOODR(PORTE)=GPIOODR_Val[PORTE];
	GPIOPDR(PORTE)=GPIOPDR_Val[PORTE];
	GPIOPUR(PORTE)=GPIOPUR_Val[PORTE];
#endif
#if(PORTF_STATUS == ENABELED)
	SET_BIT(RCGCGPIO,PORTF);
	GPIOLOCK(PORTD)=GPIO_UNLOCK_VAL;
	GPIOCR(PORTD)=0xff;
	GPIOPCTL(PORTF)=0x00000000;
	GPIODIR(PORTF)=PORTF_DIREC;
	GPIODR2R(PORTF)=GPIODR2R_Val[PORTF];
	GPIODR4R(PORTF)=GPIODR4R_Val[PORTF];
	GPIODR8R(PORTF)=GPIODR8R_Val[PORTF];
	GPIODEN(PORTF)=GPIODEN_Val[PORTF];
	GPIOODR(PORTF)=GPIOODR_Val[PORTF];
	GPIOPDR(PORTF)=GPIOPDR_Val[PORTF];
	GPIOPUR(PORTF)=GPIOPUR_Val[PORTF];
#endif

}

enumerr GPIO_enumGetPortVal(u8 copy_u8portNum,u8* u8Ptr_PortValue) //where the pointer is a carrier for the pin value
{

	*u8Ptr_PortValue = 0x00; //initialization value

		if(copy_u8portNum<PORTA || copy_u8portNum>PORTF)
		{
			return NOKAY ;
		}
		else
		{
			switch (copy_u8portNum )
			{

			case PORTA :

				(*u8Ptr_PortValue) = ( GPIO_READ_DATA(PORTA,0xFF) ) ;
				break;

			case PORTB :

				(*u8Ptr_PortValue) = ( GPIO_READ_DATA(PORTB,0xFF) ) ;
				break;

			case PORTC :

				(*u8Ptr_PortValue) = ( GPIO_READ_DATA(PORTC,0xFF) ) ;
				break;


			case PORTD :

				(*u8Ptr_PortValue) = ( GPIO_READ_DATA(PORTD,0xFF) ) ;
				break;

			case PORTE :

				(*u8Ptr_PortValue) = ( GPIO_READ_DATA(PORTE,0xFF) ) ;
				break;

			case PORTF :

				(*u8Ptr_PortValue) = ( GPIO_READ_DATA(PORTF,0xFF) ) ;
				break;


			}

		}

		return OKAY;

}

enumerr GPIO_enumSetPortVal(u8 copy_u8portNum,u8 val)
{
			if(copy_u8portNum<PORTA || copy_u8portNum>PORTF)
			{
				return NOKAY ;
			}
			else
			{
				switch (copy_u8portNum )
				{

				case PORTA :

					GPIO_WRITE_DATA(PORTA,0xFF,val);
					break;

				case PORTB :

					GPIO_WRITE_DATA(PORTB,0xFF,val);
					break;

				case PORTC :

					GPIO_WRITE_DATA(PORTC,0xFF,val);
					break;


				case PORTD :

					GPIO_WRITE_DATA(PORTD,0xFF,val);
					break;

				case PORTE :

					GPIO_WRITE_DATA(PORTE,0xFF,val);
					break;

				case PORTF :

					GPIO_WRITE_DATA(PORTF,0xFF,val);
					break;


				}

			}

			return OKAY;

}



enumerr GPIO_enumGetPinVal(u8 PinNum,u8* u8Ptr_PinValue) //where the pointer is a carrier for the pin value
{

    *u8Ptr_PinValue = 0x00; //initialization value

    enumerr enumStateValidation;
        if(PORTA_PIN0<=PinNum&&PinNum<=PORTF_PIN4)
        {
            enumStateValidation=OKAY;
            if(PORTA_PIN0<=PinNum&&PinNum<=PORTA_PIN7)
            {
                *u8Ptr_PinValue = (GPIO_READ_DATA(PORTA,1<<PinNum))>>(PinNum);
            }
            else if (PORTB_PIN0<=PinNum&&PinNum<=PORTB_PIN7)
            {
                *u8Ptr_PinValue = (GPIO_READ_DATA(PORTB,1<<PinNum-PORTA_PINS_NUM))>>(PinNum-PORTA_PINS_NUM);
            }
            else if (PORTC_PIN0<=PinNum&&PinNum<=PORTC_PIN7)
            {
                *u8Ptr_PinValue = (GPIO_READ_DATA(PORTC,1<<PinNum-PORTA_PINS_NUM-PORTB_PINS_NUM))>>(PinNum-PORTA_PINS_NUM-PORTB_PINS_NUM);
            }
            else if (PORTD_PIN0<=PinNum&&PinNum<=PORTD_PIN7)
            {
                *u8Ptr_PinValue = (GPIO_READ_DATA(PORTD,1<<PinNum-PORTA_PINS_NUM-PORTB_PINS_NUM-PORTC_PINS_NUM))>>(PinNum-PORTA_PINS_NUM-PORTB_PINS_NUM-PORTC_PINS_NUM);
            }
            else if (PORTE_PIN0<=PinNum&&PinNum<=PORTE_PIN5)
            {
                *u8Ptr_PinValue = (GPIO_READ_DATA(PORTE,1<<PinNum-PORTA_PINS_NUM-PORTB_PINS_NUM-PORTC_PINS_NUM-PORTD_PINS_NUM))>>(PinNum-PORTA_PINS_NUM-PORTB_PINS_NUM-PORTC_PINS_NUM-PORTD_PINS_NUM);
            }
            else if (PORTF_PIN0<=PinNum&&PinNum<=PORTF_PIN4)
            {
                *u8Ptr_PinValue = (GPIO_READ_DATA(PORTF,1<<PinNum-PORTA_PINS_NUM-PORTB_PINS_NUM-PORTC_PINS_NUM-PORTD_PINS_NUM-PORTE_PINS_NUM))>>(PinNum-PORTA_PINS_NUM-PORTB_PINS_NUM-PORTC_PINS_NUM-PORTD_PINS_NUM-PORTE_PINS_NUM);
            }
        }

        else
        {
            enumStateValidation=NOKAY;
        }
        return enumStateValidation;
}
