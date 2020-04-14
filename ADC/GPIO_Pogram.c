/*
 * GPIO_Pogram.c
 *
 *  Created on: Oct 12, 2019
 *      Author: AhmedShawky
 */
#include "GPIO_Utils.h"
#include "GPIO_Int.h"
#include "GPIO_configure.h"
#include "GPIO_Mem_Map.h"
#include "utils.h"
#define GPIO_UNLOCK_VAL                                   0x4C4F434B
#define VECTKEY                                           0x05FA


void (*PA_PIN0_ISR1)(void)='\0';
void (*PA_PIN1_ISR1)(void)='\0';
void (*PA_PIN2_ISR1)(void)='\0';
void (*PA_PIN3_ISR1)(void)='\0';
void (*PA_PIN4_ISR1)(void)='\0';
void (*PA_PIN5_ISR1)(void)='\0';
void (*PA_PIN6_ISR1)(void)='\0';
void (*PA_PIN7_ISR1)(void)='\0';

void (*PA_PIN0_ISR2)(void)='\0';
void (*PA_PIN1_ISR2)(void)='\0';
void (*PA_PIN2_ISR2)(void)='\0';
void (*PA_PIN3_ISR2)(void)='\0';
void (*PA_PIN4_ISR2)(void)='\0';
void (*PA_PIN5_ISR2)(void)='\0';
void (*PA_PIN6_ISR2)(void)='\0';
void (*PA_PIN7_ISR2)(void)='\0';




u32 PORTS_BASES[PORTS_NUMBER]={GPIO_PORTA_BASE,GPIO_PORTB_BASE,GPIO_PORTC_BASE,GPIO_PORTD_BASE,GPIO_PORTE_BASE,GPIO_PORTF_BASE};
extern u8 GPIODR2R_Val[PORTS_NUMBER];
extern u8 GPIODR4R_Val[PORTS_NUMBER];
extern u8 GPIODR8R_Val[PORTS_NUMBER];
extern u8 GPIOODR_Val[PORTS_NUMBER];
extern u8 GPIOPUR_Val[PORTS_NUMBER];
extern u8 GPIOPDR_Val[PORTS_NUMBER];
extern u8 GPIODEN_Val[PORTS_NUMBER];




extern u8 GPIOIBE_Val[PORTS_NUMBER];
extern u8 GPIOIS_Val[PORTS_NUMBER];
extern u8 GPIOIBE_Val[PORTS_NUMBER];
extern u8 GPIOIEV_Val[PORTS_NUMBER];



#define PORTA_DIREC              CONCBIT(PORTA_PIN7_DIREC,PORTA_PIN6_DIREC,PORTA_PIN5_DIREC,PORTA_PIN4_DIREC,PORTA_PIN3_DIREC,PORTA_PIN2_DIREC,PORTA_PIN1_DIREC,PORTA_PIN0_DIREC)
#define PORTB_DIREC              CONCBIT(PORTB_PIN7_DIREC,PORTB_PIN6_DIREC,PORTB_PIN5_DIREC,PORTB_PIN4_DIREC,PORTB_PIN3_DIREC,PORTB_PIN2_DIREC,PORTB_PIN1_DIREC,PORTB_PIN0_DIREC)
#define PORTC_DIREC              CONCBIT(PORTC_PIN7_DIREC,PORTC_PIN6_DIREC,PORTC_PIN5_DIREC,PORTC_PIN4_DIREC,PORTC_PIN3_DIREC,PORTC_PIN2_DIREC,PORTC_PIN1_DIREC,PORTC_PIN0_DIREC)
#define PORTD_DIREC              CONCBIT(PORTD_PIN7_DIREC,PORTD_PIN6_DIREC,PORTD_PIN5_DIREC,PORTD_PIN4_DIREC,PORTD_PIN3_DIREC,PORTD_PIN2_DIREC,PORTD_PIN1_DIREC,PORTD_PIN0_DIREC)
#define PORTE_DIREC              CONCBIT(0,0,PORTE_PIN5_DIREC,PORTE_PIN4_DIREC,PORTE_PIN3_DIREC,PORTE_PIN2_DIREC,PORTE_PIN1_DIREC,PORTE_PIN0_DIREC)
#define PORTF_DIREC              CONCBIT(0,0,0,PORTF_PIN4_DIREC,PORTF_PIN3_DIREC,PORTF_PIN2_DIREC,PORTF_PIN1_DIREC,PORTF_PIN0_DIREC)


//=====================================================SET_BIT_VAL func============================================================================


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

//============================================SET_PIN_DIREC func=================================================================================

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


//===============================================INIT func========================================================================================


void GPIO_Init(void)
{
#if(PORTA_STATUS == ENABELED)
	SET_BIT(RCGCGPIO,PORTA); //ENABLING THE CLOCK
	GPIOLOCK(PORTA)=GPIO_UNLOCK_VAL;
	GPIOCR(PORTA)=0xff;
	GPIOAFSEL(PORTA)=0x0000003F; //enabling the alternate function for porta pin1 and pin0 as they are locked to uart AND FROM [5:2]
	GPIOPCTL(PORTA)=0x00000000;   //selecting the alternate function for these pins as GPIO
	GPIODIR(PORTA)=PORTA_DIREC;    //selecting the direction of the port
	GPIODR2R(PORTA)=GPIODR2R_Val[PORTA];   //SETTING THE VALUES FOR THE REGISTERS IF THEY WERE CONFIGURED AS OUTPUT
	GPIODR4R(PORTA)=GPIODR4R_Val[PORTA];
	GPIODR8R(PORTA)=GPIODR8R_Val[PORTA];
	GPIODEN(PORTA)=GPIODEN_Val[PORTA];   //ENABLING THE DIGITALLY ENABELED PINS BEFORE OPEN DRAIN SELECTION
	GPIOODR(PORTA)=GPIOODR_Val[PORTA];
	GPIOPDR(PORTA)=GPIOPDR_Val[PORTA];    //SETTING I/P PINS AS EITHER PULLUP OR DOWN
	GPIOPUR(PORTA)=GPIOPUR_Val[PORTA];
	//CHECKING IF ANY PIN IS INTERRUPT ENABELED
	if(PORTA_PIN0_INT|PORTA_PIN1_INT|PORTA_PIN2_INT|PORTA_PIN3_INT|PORTA_PIN4_INT|PORTA_PIN5_INT|PORTA_PIN6_INT|PORTA_PIN7_INT)
	{
		//TO AVOID FALSE INTERRUPTS WE FOLLOW THE STEPS IN THE DATASHEET
		GPIOIM(PORTA)=0x00;
		GPIOIS(PORTA)=GPIOIS_Val[PORTA];
		GPIOIBE(PORTA)=GPIOIBE_Val[PORTA];
		GPIOIEV(PORTA)=GPIOIEV_Val[PORTA];
		GPIOICR(PORTA)=0xFF;  //CLEARNING THE GPIORIS BY SETTING THE GPIOICR
		PRI0=PORTA_GP_PRI<<5;                 //SETTING THE PRI OF THE INT,5 IS THE NUMBER OF RESERVED BITS BEFORE THE 3 PRI BITS OF INTA
		GPIOIM(PORTA)=CONCBIT(PORTA_PIN7_INT,PORTA_PIN6_INT,PORTA_PIN5_INT,PORTA_PIN4_INT,PORTA_PIN3_INT,PORTA_PIN2_INT,PORTA_PIN1_INT,PORTA_PIN0_INT);
		//ENABELE PORTA'S INTERRUPT
		SET_BIT(EN0,PORTA_INT_NUM);
	}
	else
	{

	}
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
	if(PORTB_PIN0_INT|PORTB_PIN1_INT|PORTB_PIN2_INT|PORTB_PIN3_INT|PORTB_PIN4_INT|PORTB_PIN5_INT|PORTB_PIN6_INT|PORTB_PIN7_INT)
		{
			//TO AVOID FALSE INTERRUPTS WE FOLLOW THE STEPS IN THE DATASHEET
			GPIOIM(PORTB)=0x00;
			GPIOIS(PORTB)=GPIOIS_Val[PORTB];
			GPIOIBE(PORTB)=GPIOIBE_Val[PORTB];
			GPIOIEV(PORTB)=GPIOIEV_Val[PORTB];
			GPIOICR(PORTB)=0xFF;
			PRI0=PORTB_GP_PRI<<13;                 //13 IS THE NUM OF RESERVED BITS BEFORE INTB
			GPIOIM(PORTB)=CONCBIT(PORTB_PIN7_INT,PORTB_PIN6_INT,PORTB_PIN5_INT,PORTB_PIN4_INT,PORTB_PIN3_INT,PORTB_PIN2_INT,PORTB_PIN1_INT,PORTB_PIN0_INT);
			//ENABELE PORTB'S INTERRUPT
			SET_BIT(EN0,PORTB_INT_NUM);
		}
		else
		{

		}
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
	if(PORTC_PIN0_INT|PORTC_PIN1_INT|PORTC_PIN2_INT|PORTC_PIN3_INT|PORTC_PIN4_INT|PORTC_PIN5_INT|PORTC_PIN6_INT|PORTC_PIN7_INT)
		{
			//TO AVOID FALSE INTERRUPTS WE FOLLOW THE STEPS IN THE DATASHEET
			GPIOIM(PORTC)=0x00;
			GPIOIS(PORTC)=GPIOIS_Val[PORTC];
			GPIOIBE(PORTC)=GPIOIBE_Val[PORTC];
			GPIOIEV(PORTC)=GPIOIEV_Val[PORTC];
			GPIOICR(PORTC)=0xFF;
			PRI0=PORTC_GP_PRI<<21;                 //21 IS THE NUM OF RESERVED BITS BEFORE INTC
			GPIOIM(PORTC)=CONCBIT(PORTC_PIN7_INT,PORTC_PIN6_INT,PORTC_PIN5_INT,PORTC_PIN4_INT,PORTC_PIN3_INT,PORTC_PIN2_INT,PORTC_PIN1_INT,PORTC_PIN0_INT);
			//ENABELE PORTC'S INTERRUPT
			SET_BIT(EN0,PORTC_INT_NUM);
		}
		else
		{

		}
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
	if(PORTD_PIN0_INT|PORTD_PIN1_INT|PORTD_PIN2_INT|PORTD_PIN3_INT|PORTD_PIN4_INT|PORTD_PIN5_INT|PORTD_PIN6_INT|PORTD_PIN7_INT)
			{
				//TO AVOID FALSE INTERRUPTS WE FOLLOW THE STEPS IN THE DATASHEET
				GPIOIM(PORTD)=0x00;
				GPIOIS(PORTD)=GPIOIS_Val[PORTD];
				GPIOIBE(PORTD)=GPIOIBE_Val[PORTD];
				GPIOIEV(PORTD)=GPIOIEV_Val[PORTD];
				GPIOICR(PORTD)=0xFF;
				PRI0=PORTD_GP_PRI<<29;                 //29 IS THE NUM OF RESERVED BITS BEFORE INTD
				GPIOIM(PORTD)=CONCBIT(PORTD_PIN7_INT,PORTD_PIN6_INT,PORTD_PIN5_INT,PORTD_PIN4_INT,PORTD_PIN3_INT,PORTD_PIN2_INT,PORTD_PIN1_INT,PORTD_PIN0_INT);
				//ENABELE PORTD'S INTERRUPT
				SET_BIT(EN0,PORTD_INT_NUM);
			}
			else
			{

			}
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
	if(PORTE_PIN0_INT|PORTE_PIN1_INT|PORTE_PIN2_INT|PORTE_PIN3_INT|PORTE_PIN4_INT|PORTE_PIN5_INT)
			{
				//TO AVOID FALSE INTERRUPTS WE FOLLOW THE STEPS IN THE DATASHEET
				GPIOIM(PORTE)=0x00;
				GPIOIS(PORTE)=GPIOIS_Val[PORTE];
				GPIOIBE(PORTE)=GPIOIBE_Val[PORTE];
				GPIOIEV(PORTE)=GPIOIEV_Val[PORTE];
				GPIOICR(PORTE)=0xFF;
				PRI1=PORTE_GP_PRI<<5;                 //5 IS THE NUM OF RESERVED BITS BEFORE INTD IN PRI1
				GPIOIM(PORTE)=CONCBIT(0,0,PORTE_PIN5_INT,PORTE_PIN4_INT,PORTE_PIN3_INT,PORTE_PIN2_INT,PORTE_PIN1_INT,PORTE_PIN0_INT);
				//ENABELE PORTE'S INTERRUPT
				SET_BIT(EN0,PORTE_INT_NUM);
			}
			else
			{

			}
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
	if(PORTF_PIN0_INT|PORTF_PIN1_INT|PORTF_PIN2_INT|PORTF_PIN3_INT|PORTF_PIN4_INT)
			{
				//TO AVOID FALSE INTERRUPTS WE FOLLOW THE STEPS IN THE DATASHEET
				GPIOIM(PORTF)=0x00;
				GPIOIS(PORTF)=GPIOIS_Val[PORTF];
				GPIOIBE(PORTF)=GPIOIBE_Val[PORTF];
				GPIOIEV(PORTF)=GPIOIEV_Val[PORTF];
				GPIOICR(PORTF)=0xFF;
				PRI7=PORTF_GP_PRI<<21;                 //21 IS THE NUM OF RESERVED BITS BEFORE INTC IN PRI7
				GPIOIM(PORTF)=CONCBIT(0,0,0,PORTF_PIN4_INT,PORTF_PIN3_INT,PORTF_PIN2_INT,PORTF_PIN1_INT,PORTF_PIN0_INT);
				//ENABELE PORTF'S INTERRUPT
				SET_BIT(EN0,PORTF_INT_NUM);
			}
			else
			{

			}
#endif
#if(SUBPRIORITY_STATUS==ENABELED)
{
	APINT=VECTKEY<<16;                   //THIS VAL SHOULD BE SET IN THE UPPER 16 BITS OF THE APINT TO ACCESS THE REGISTER
		#if(SUBPRIORITY_LEVEL_NUM==1)
		{
			APINT=0x4<<8;
		}
		#elif(SUBPRIORITY_LEVEL_NUM==2)
		{
			APINT=0x5<<8;
		}
		#elif(SUBPRIORITY_LEVEL_NUM==4)
		{
			APINT=0x6<<8;
		}
		#elif(SUBPRIORITY_LEVEL_NUM==8)
		{
			APINT=0x7<<8;
		}
		#endif
}
#endif
}

//==========================================GET_PORT_VAL func===================================================================================

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

//====================================== SET_PORT_VAL func====================================================================================

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


//=======================================GET_PIN_VAL func==========================================================================


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


//================================================PORTA _ISR=================================================================================
void PORTA_ISR(void)
{
	u8 local_INT_PIN,local_bit_num,local_GPIOIBE_status;
	static u8 local_arr_counter[PORTA_PINS_NUM]={0};
	for(local_bit_num=0;local_bit_num<PORTA_PINS_NUM;local_bit_num++)
	{
		local_GPIOIBE_status=(GPIOIBE_Val[PORTA]&(1<<local_bit_num));
		local_INT_PIN=(GPIORIS(PORTA)&(1<<local_bit_num));
		if(local_INT_PIN==1) //then an event from porta pin0 has occured
		{
			/*
			 * write the ISR then don't forget to SET the corresponding pin in the GPIOICR in case of edge triggered
			 */
			if(local_GPIOIBE_status==1) //then the GPIOIBE is enabeled for this pin
			{
				if(local_arr_counter[local_bit_num]==0)
				{
					PA_PIN0_ISR1();
					local_arr_counter[local_bit_num]=1;
					SET_BIT(GPIOICR(PORTA),local_bit_num);// CLEARING THE GPIORIS AFTER EXECUTING THE
				}
				else if(local_arr_counter[local_bit_num]==1)
				{

					PA_PIN0_ISR2();
					local_arr_counter[local_bit_num]=0;
					SET_BIT(GPIOICR(PORTA),local_bit_num);// CLEARING THE GPIORIS AFTER EXECUTING THE
				}
			}
			else
			{

				PA_PIN0_ISR1();
				SET_BIT(GPIOICR(PORTA),local_bit_num);// CLEARING THE GPIORIS AFTER EXECUTING THE

			}
		}
		if(local_INT_PIN==2) //then an event from porta pin1 has occured
		{
			/*
			 * write the code here then don't forget to set the corresponding pin in the GPIOICR in case of edge triggered
			 */
		}
		if(local_INT_PIN==4) //then an event from porta pin2 has occured
		{
			/*
			 * write the code here then don't forget to set the corresponding pin in the GPIOICR in case of edge triggered
			 */
		}
		if(local_INT_PIN==8) //then an event from porta pin3 has occured
		{
			/*
			 * write the code here then don't forget to set the corresponding in the GPIOICR in case of edge triggered
			 */
		}
		if(local_INT_PIN==16) //then an event from porta pin4 has occured
		{
			/*
			 * write the code here then don't forget to set the corresponding in the GPIOICR in case of edge triggered
			 */
		}
		if(local_INT_PIN==32) //then an event from porta pin5 has occured
		{
			/*
			 * write the code here then don't forget to set the corresponding in the GPIOICR in case of edge triggered
			 */
			if(local_GPIOIBE_status==32) //then the GPIOIBE is enabeled for this pin
			{
				if(local_arr_counter[local_bit_num]==0)
				{
					PA_PIN5_ISR1();
					local_arr_counter[local_bit_num]++;
					SET_BIT(GPIOICR(PORTA),local_bit_num);// CLEARING THE GPIORIS AFTER EXECUTING THE
				}
				else if(local_arr_counter[local_bit_num]==1)
				{
					PA_PIN5_ISR2();
					local_arr_counter[local_bit_num]=0;
					SET_BIT(GPIOICR(PORTA),local_bit_num);// CLEARING THE GPIORIS AFTER EXECUTING THE
				}
			}
			else //THEN IBE IS NOT ENABELED FOR THIS PIN
			{

				PA_PIN5_ISR1();
				SET_BIT(GPIOICR(PORTA),local_bit_num);// CLEARING THE GPIORIS AFTER EXECUTING THE

			}
		}
		if(local_INT_PIN==64) //then an event from porta pin6 has occured
		{
			/*
			 * write the code here then don't forget to SET the corresponding in the GPIOICR in case of edge triggered
			 */
			if(local_GPIOIBE_status==64) //then the GPIOIBE is enabeled for this pin
			{
				if(local_arr_counter[local_bit_num]==0)
				{
					PA_PIN6_ISR1();
					local_arr_counter[local_bit_num]++;
					SET_BIT(GPIOICR(PORTA),local_bit_num);// CLEARING THE GPIORIS AFTER EXECUTING THE
				}
				else if(local_arr_counter[local_bit_num]==1)
				{
					PA_PIN6_ISR2();
					local_arr_counter[local_bit_num]=0;
					SET_BIT(GPIOICR(PORTA),local_bit_num);// CLEARING THE GPIORIS AFTER EXECUTING THE
				}
			}
			else //THEN NO IBE IS ENABELED FOR THIS PIN
			{

				PA_PIN6_ISR1();
				SET_BIT(GPIOICR(PORTA),local_bit_num);// CLEARING THE GPIORIS AFTER EXECUTING THE

			}
		}
		if(local_INT_PIN==128) //then an event from porta pin7 has occured
		{
			/*
			 * write the code here then don't forget to set the corresponding in the GPIOICR in case of edge triggered
			 */
			if(local_GPIOIBE_status==128) //then the GPIOIBE is enabeled for this pin
			{
				if(local_arr_counter[local_bit_num]==0)
				{
					PA_PIN7_ISR1();
					local_arr_counter[local_bit_num]++;
					SET_BIT(GPIOICR(PORTA),local_bit_num);// CLEARING THE GPIORIS AFTER EXECUTING THE
				}
				else if(local_arr_counter[local_bit_num]==1)
				{
					PA_PIN7_ISR2();
					local_arr_counter[local_bit_num]=0;
					SET_BIT(GPIOICR(PORTA),local_bit_num);// CLEARING THE GPIORIS AFTER EXECUTING THE
				}
			}
			else //THEN NO IBE IS ENABELED FOR THIS PIN
			{

				PA_PIN7_ISR1();
				SET_BIT(GPIOICR(PORTA),local_bit_num);// CLEARING THE GPIORIS AFTER EXECUTING THE

			}
		}
	}
}


//===================================================ISR func for each pin====================================================================================

void PORTA_PIN0_ISR(void(*Local_PIN0_ISR1)(void),void(*Local_PIN0_ISR2)(void))
{
	PA_PIN0_ISR1=Local_PIN0_ISR1;
	PA_PIN0_ISR2=Local_PIN0_ISR2;
}



void PORTA_PIN1_ISR(void(*Local_PIN1_ISR1)(void),void(*Local_PIN1_ISR2)(void))
{
	PA_PIN1_ISR1=Local_PIN1_ISR1;
	PA_PIN1_ISR2=Local_PIN1_ISR2;
}



void PORTA_PIN2_ISR(void(*Local_PIN2_ISR1)(void),void(*Local_PIN2_ISR2)(void))
{
	PA_PIN2_ISR1=Local_PIN2_ISR1;
	PA_PIN2_ISR2=Local_PIN2_ISR2;
}



void PORTA_PIN3_ISR(void(*Local_PIN3_ISR1)(void),void(*Local_PIN3_ISR2)(void))
{
	PA_PIN3_ISR1=Local_PIN3_ISR1;
	PA_PIN3_ISR2=Local_PIN3_ISR2;
}



void PORTA_PIN4_ISR(void(*Local_PIN4_ISR1)(void),void(*Local_PIN4_ISR2)(void))
{
	PA_PIN4_ISR1=Local_PIN4_ISR1;
	PA_PIN4_ISR2=Local_PIN4_ISR2;
}



void PORTA_PIN5_ISR(void(*Local_PIN5_ISR1)(void),void(*Local_PIN5_ISR2)(void))
{
	PA_PIN5_ISR1=Local_PIN5_ISR1;
	PA_PIN5_ISR2=Local_PIN5_ISR2;
}



void PORTA_PIN6_ISR(void(*Local_PIN6_ISR1)(void),void(*Local_PIN6_ISR2)(void))
{
	PA_PIN6_ISR1=Local_PIN6_ISR1;
	PA_PIN6_ISR2=Local_PIN6_ISR2;
}



void PORTA_PIN7_ISR(void(*Local_PIN7_ISR1)(void),void(*Local_PIN7_ISR2)(void))
{
	PA_PIN7_ISR1=Local_PIN7_ISR1;
	PA_PIN7_ISR2=Local_PIN7_ISR2;
}
