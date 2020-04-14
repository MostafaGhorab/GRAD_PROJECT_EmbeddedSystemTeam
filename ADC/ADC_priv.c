/*
 * ADC_utils.c
 *
 *  Created on: Oct 29, 2019
 *      Author: Yomna Ragab
 */
#include "std_types.h"
#include "GPIO_Mem_Map.h"
#include "GPIO_Int.h"
#include "ADC_memory_map.h"
#include "ADC_int.h"
#include "ADC_priv.h"
#include "ADC_config.h"

extern u32 PORTS_BASES[PORTS_NUMBER];



void ADC_vidSetAlternateFunc(u8 pin,u8 desired_port)
{
    if ((pin== AIN8)||(pin== AIN11)) GPIOAFSEL(desired_port) |=0x20;
    else if ((pin== AIN9)||(pin== AIN10)) GPIOAFSEL(desired_port) |=0x10;
    else if ((pin== AIN0)||(pin== AIN4)) GPIOAFSEL(desired_port) |=0x08;
    else if ((pin== AIN1)||(pin== AIN5)) GPIOAFSEL(desired_port) |=0x04;
    else if ((pin== AIN2)||(pin== AIN6)) GPIOAFSEL(desired_port) |=0x02;
    else if ((pin== AIN3)||(pin== AIN7)) GPIOAFSEL(desired_port) |=0x01;
}
void ADC_vidDisableDEN(u8 pin,u8 desired_port)
{
    if ((pin== AIN8)||(pin== AIN11)) GPIODEN(desired_port) &= ~0x20;
    else if ((pin== AIN9)||(pin== AIN10)) GPIODEN(desired_port) &= ~0x10;
    else if ((pin== AIN0)||(pin== AIN4)) GPIODEN(desired_port)  &= ~0x08;
    else if ((pin== AIN1)||(pin== AIN5)) GPIODEN(desired_port)  &= ~0x04;
    else if ((pin== AIN2)||(pin== AIN6)) GPIODEN(desired_port)  &= ~0x02;
    else if ((pin== AIN3)||(pin== AIN7)) GPIODEN(desired_port)  &= ~0x01;
}
void ADC_vidEnAnalogFunc(u8 pin,u8 desired_port)
{
    if ((pin== AIN8)||(pin== AIN11)) GPIOAMSEL(desired_port)|=0x20;
    else if ((pin== AIN9)||(pin== AIN10)) GPIOAMSEL(desired_port)|=0x10;
    else if ((pin== AIN0)||(pin== AIN4)) GPIOAMSEL(desired_port)|=0x08;
    else if ((pin== AIN1)||(pin== AIN5)) GPIOAMSEL(desired_port)|=0x04;
    else if ((pin== AIN2)||(pin== AIN6)) GPIOAMSEL(desired_port)|=0x02;
    else if ((pin== AIN3)||(pin== AIN7)) GPIOAMSEL(desired_port)|=0x01;
}


void ADC_EnableDisableSeqSel(u8 control,u8 sample_seq)
{

    if (control==DISABLE)       ADC0ACTSS &= ~ (1<<sample_seq);
    else if(control==ENABLE)    ADC0ACTSS|= 1<<sample_seq;

}
void ADC_vidTrigger(u8 trigger,u8 sample_seq)
{
    if(trigger==SW) //ADCPSSI to set the trigger
    {
        ADC0EMUX &= ~ (0xF << NIBBLE_SHIFT*sample_seq);
    }
    else if (trigger==CONT)
    {
        ADC0EMUX &= ~ (0x0 << NIBBLE_SHIFT*sample_seq);
    }

}

void ADC_vidSSCTL(u8 seq,u8 no_of_channels)
{
    //SINGLE_ACCESS
    ADC0SSCTL(seq)= LAST_SAMPLE_EN_AND_INTERRUPT << ((no_of_channels-1)<<NIBBLE_SHIFT);
    /*Configure the last sample and enable its interrupt*/

}

