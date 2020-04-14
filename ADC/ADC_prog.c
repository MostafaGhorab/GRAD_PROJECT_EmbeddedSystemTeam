/*
 * ADC_prog.c
 *
 *  Created on: Oct 29, 2019
 *      Author: Yomna Ragab
 */

#include "std_types.h"
#include "ADC_memory_map.h"
#include "GPIO_Int.h"
#include "ADC_int.h"
#include "ADC_config.h"
#include "ADC_priv.h"
//#include "ADC_INTERRUPTS.h"

extern const strGroupInfo adc_groups[NUMOFGROUPS];
extern const strModuleInfo adc_confg[NUMOFMODULES];
strSS_pos SS_positions[NUMOFMODULES];

u32* Adc_resultBuffer[NUMOFGROUPS];
/*array of pointer to hold a buffer pointer for each gp*/
u8 finished_samples[NUMOFGROUPS]={0};
/*no of finished samples for each gp*/

extern const u32 ADC_FIFOn[NUMBER_OF_SS]={ADCSSFIFO0_OFFSET,ADCSSFIFO1_OFFSET,ADCSSFIFO2_OFFSET,ADCSSFIFO3_OFFSET};
/*FIFO registers offset*/
extern const u32 ADC_SSCTLn[NUMBER_OF_SS]={ADCSSCTL0_OFFSET,ADCSSCTL1_OFFSET,ADCSSCTL2_OFFSET,ADCSSCTL3_OFFSET};
/*SSCTL registers offset*/
extern const u32 ADC_SSMUXn[NUMBER_OF_SS]={ADCSSMUX0_OFFSET,ADCSSMUX1_OFFSET,ADCSSMUX2_OFFSET,ADCSSMUX3_OFFSET};
/*SSMUX registers offset*/

ADC_enumerr ADC_enuminit(void)
{
    u8 i,k;
    ADC_enumerr return_status;
    k=0;
    for (i=0; i<NUMOFMODULES; i++)
    {
        switch(adc_confg[i].sequencer_no)
        {
        case SS0:
            SS_positions[SS0].position=i;
            SS_positions[SS0].ss_no=SS0;
            SS_positions[SS0].gp_id=adc_confg[i].group_ID;
            break;
        case SS1:
            SS_positions[SS1].position=i;
            SS_positions[SS1].ss_no=SS1;
            SS_positions[SS1].gp_id=adc_confg[i].group_ID;
            break;
        case SS2:
            SS_positions[SS2].position=i;
            SS_positions[SS2].ss_no=SS2;
            SS_positions[SS2].gp_id=adc_confg[i].group_ID;
            break;
        case SS3:
            SS_positions[SS3].position=i;
            SS_positions[SS3].ss_no=SS3;
            SS_positions[SS3].gp_id=adc_confg[i].group_ID;
            break;
        default:
            return_status=ADC_NOTOKAY;
            break;
        }
    }
    for (i=0; i<NUMOFMODULES; i++)
    {
        /*ACTIVATE ADC0*/
        RCGC0|=0x00010000;
        /*configure clk speed "8th and 9th bits*/
        RCGC0 &= ~(adc_confg[i].clk << 7);
        /* SET samples priority*/
        ADC0SSPRI=CONCBIT_SEQ(SAMPLE_SEQ3_PRI,SAMPLE_SEQ2_PRI,SAMPLE_SEQ1_PRI,SAMPLE_SEQ0_PRI);

        /*disable sample sequencer which was chosen*/
        ADC_EnableDisableSeqSel(DISABLE,adc_confg[i].sequencer_no);

        /*SELECT THE trigger*/
        ADC_vidTrigger(adc_groups[adc_confg[i].group_ID].trigger,adc_confg[i].sequencer_no);

        /*Channels Selection*/
        while (k<adc_confg[i].no_of_inp_channels)
        {
            ADC0SSMUX(adc_confg[i].sequencer_no)= adc_groups[adc_confg[i].group_ID].ss_pins[k]<<(NIBBLE_SHIFT*i);
            k++;
        }

        /*Choose the end sample and enable it's interrupt*/
        ADC_vidSSCTL(adc_confg[i].sequencer_no,adc_confg[i].no_of_inp_channels);

        /*Enable the sample sequencer*/
        ADC_EnableDisableSeqSel(ENABLE,adc_confg[i].sequencer_no);
    }
    return_status=ADC_OKAY;
    return(return_status);
}
ADC_enumerr ADC_enumInterrupt_Enable(u8 module_id)
{
    ADC_enumerr status;
    if (module_id < NUMOFMODULES)
    {
        ADC0IM=(1<<adc_confg[module_id].sequencer_no);
        /*Enable the interrupt mask of the module*/
        SET_INT_PRI((adc_confg[module_id].sequencer_no)+INT_NO_SHIFT,0);
        /*Set the interrupt priority by calling set interrupt priority and give it the SS no added to 14 to
         * matched the number in the interrupt vector table, and 0 it's priority*/
        EN_INT((adc_confg[module_id].sequencer_no+INT_NO_SHIFT));
        /**
         * Enable the interrupt by giving the macors func the interrupt no in the vector table
         */
        status=ADC_OKAY;
    }
    else
    {
        status=ADC_NOTOKAY;
    }
    return(status);
}
void ADC_vidStartConv(u8 module_id)
{
    if(adc_groups[adc_confg[module_id].group_ID].trigger ==SW)
    {
        ADC0PSSI = 1 << (adc_confg[module_id].sequencer_no);
    }

}
ADC_enumerr ADC_enumSetBuffer(u8 GP_ID, u32 * BufferPtr)
{
    ADC_enumerr status;
    u8 i=0;
    for (i=0; i<NUMOFGROUPS; i++)
    {
        if(GP_ID == adc_groups[i].group_ID)
        {
            Adc_resultBuffer[GP_ID]=BufferPtr;
            /*set the buffer pointer for each gp*/
            status=ADC_OKAY;
            break;
        }
        else
        {
            status=ADC_NOTOKAY;

        }
    }

    return(status);
}



ADC_enumconv ADC_enumGetData(u8 group_ID)
{
    u8 j=0;
    ADC_enumconv status=CONV_NOT_YET;
    for (j=0; j<NUMOFMODULES; j++)
    {
        if (SS_positions[j].gp_id==group_ID)
        {
            break;
        }
        else
        {
            status=error;
        }
    }
    if(status!=error)
    {

        u8 noOfChannels=adc_confg[j].no_of_inp_channels;
        u8 gp_id=adc_confg[j].group_ID;
        u8 SS=adc_confg[j].sequencer_no;
        u8 module_id=adc_confg[j].sequencer_no;
        u8 streaming_samp_no=adc_groups[gp_id].no_of_streaming_samples;
        u8 i=0;
        if((ADC0RIS & (1<<(module_id))) != 0)
        {

            SET_ISC(module_id);
            for (i=finished_samples[gp_id]*noOfChannels; i<((noOfChannels*(finished_samples[gp_id])+1)); i++)
            {
                *(Adc_resultBuffer[gp_id]+i)=ADC0_SSFIFO(SS);
            }
            finished_samples[gp_id]++;
            if(finished_samples[gp_id]==streaming_samp_no)
            {
                finished_samples[gp_id]=0;
            }
            status=CONV_DONE;
        }
        else
        {
            status=CONV_NOT_YET;
        }
    }
    return(status);
}


/************************Test polling ***************************/
void main (void)
{
    u32 data=0;
    volatile u32 i=0;
    u32 k=0;
    GPIO_Init();
    ADC_enuminit();
    ADC_enumSetBuffer(0,&data);
    while(1)
    {
        ADC_vidStartConv(0);
        u8 h=ADC_enumGetData(0);
        if(data<200)
        {
            GPIO_enumSetPinVal(PORTF_PIN1,1);

        }
        else if(data<500)
        {
            GPIO_enumSetPinVal(PORTF_PIN2,1);
        }
        else
        {
            GPIO_enumSetPinVal(PORTF_PIN3,1);

        }
        for(i=0; i<88888; i++)
        {
            k++;
        }
        GPIO_enumSetPinVal(PORTF_PIN3,0);
        GPIO_enumSetPinVal(PORTF_PIN2,0);
        GPIO_enumSetPinVal(PORTF_PIN1,0);



    }

}
/*****************************interrupt test****************/
//void main (void)
//{
//    u32 data=0;
//    volatile u32 i=0;
//    u32 k=0;
//    GPIO_Init();
//    ADC_enuminit();
//    ADC_enumSetBuffer(0,&data);
//    ADC_enumInterrupt_Enable(0);
//    while(1)
//    {
//        ADC_vidStartConv(0);
////        for(i=0; i<88888; i++)
////        {
////            k++;
////        }
//
//        if(data<200)
//        {
//            GPIO_enumSetPinVal(PORTF_PIN1,1);
//
//        }
//        else if(data<500)
//        {
//            GPIO_enumSetPinVal(PORTF_PIN2,1);
//        }
//        else
//        {
//            GPIO_enumSetPinVal(PORTF_PIN3,1);
//
//        }
//        for(i=0; i<88888; i++)
//                {
//                    k++;
//                }
//        GPIO_enumSetPinVal(PORTF_PIN3,0);
//        GPIO_enumSetPinVal(PORTF_PIN2,0);
//        GPIO_enumSetPinVal(PORTF_PIN1,0);
//
//
//
//    }
//
//}
