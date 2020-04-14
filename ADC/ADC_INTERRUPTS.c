/*
 * ADC_INTERRUPTS.c
 *
 *  Created on: Dec 1, 2019
 *      Author: Yomna Ragab
 */

#include "std_types.h"
#include "ADC_INTERRUPTS.h"
#include "ADC_memory_map.h"
#include "ADC_config.h"
#include "ADC_int.h"
#include "ADC_priv.h"



extern strSS_pos SS_positions[NUMOFMODULES];
/*Extern the array which hold each sequencer position in the config array*/
extern u8 finished_samples[NUMOFGROUPS];
/*Number of finished samples for each group*/
extern u32* Adc_resultBuffer[NUMOFGROUPS];
/*Array of pointer to buffer pointers , one for each group*/

extern const strGroupInfo adc_groups[NUMOFGROUPS];
/*The array holds the info for each gp*/
extern const strModuleInfo adc_confg[NUMOFMODULES];
/*The array holds the info for each module*/

/*pointers to functions , used to implement the ISR for each sequencer*/
void (*SS0_interrupt)(void)  = (void*)0;
void (*SS1_interrupt)(void)  = (void*)0;
void (*SS2_interrupt)(void)  = (void*)0;
void (*SS3_interrupt)(void)   = (void*)0;

/*Func takes the func pointer and assign it to the sample sequencer 0 pointer to be called in the ISR of sample seq 0 */
void SS0_ISR(void(*ptr)(void))
{
    SS0_interrupt=ptr;
}
/*Func takes the func pointer and assign it to the sample sequencer 1 pointer to be called in the ISR of sample seq 1 */
void SS1_ISR(void(*ptr)(void))
{
    SS1_interrupt=ptr;
}
/*Func takes the func pointer and assign it to the sample sequencer 2 pointer to be called in the ISR of sample seq 2 */
void SS2_ISR(void(*ptr)(void))
{
    SS2_interrupt=ptr;
}
/*Func takes the func pointer and assign it to the sample sequencer 3 pointer to be called in the ISR of sample seq 3 */
void SS3_ISR(void(*ptr)(void))
{
    SS3_interrupt=ptr;
}

/*The ISR of sequencer 3 , will be called after the conversion is finished , if the SEQ INTERRUPT is enabled */
void ADC_Sequence3_Handler(void)
{
    /*Assign the number of input channels in a variable,by index the adc_config array by the sequencer position which is held in the SS_positions array*/
    u8 noOfChannels=adc_confg[SS_positions[SS3].position].no_of_inp_channels;
    /*Assign the corresponding gp in a variable,by index the adc_config array by the sequencer position which is held in the SS_positions array*/
    u8 gp_id=adc_confg[SS_positions[SS3].position].group_ID;
    /*Assign the number streaming samples in a variable,by index the adc_groups array by group id from the previous assignment*/
    u8 streaming_samp_no=adc_groups[gp_id].no_of_streaming_samples;
    /*Counter*/
    u8 i=0;
    /*In case of streaming conversion , it has to start indexing the buffer from the last indexing to avoid overwriting */
    for (i=finished_samples[gp_id]; i<((noOfChannels*(finished_samples[gp_id])+1)); i++)
    {
        *(Adc_resultBuffer[gp_id]+i)=ADC0_SSFIFO(SS3);
        /*
         *put the converted result on the buffer and each time started from the finished samples*no of channels to
         *avoid overwriting data
         */
    }
    /*increment the number of converted samples of the group */
    finished_samples[gp_id]++;

    /*Sent an acknowledge by set the interrupt and status for ss3*/
    SET_ISC(SS3);

    /*In case the number of converted samples is equal of streaming samples,then the finished samples will be reset to 0*/
    if(finished_samples[gp_id]==streaming_samp_no)
    {
        finished_samples[gp_id]=0;
    }
    /*Call the User ISR of Sample sequencer 3*/
    SS3_interrupt();
}
void ADC_Sequence2_Handler(void)
{
    u8 noOfChannels=adc_confg[SS_positions[SS2].position].no_of_inp_channels;
    u8 gp_id=adc_confg[SS_positions[SS2].position].group_ID;
    u8 streaming_samp_no=adc_groups[gp_id].no_of_streaming_samples;
    u8 i=0;
    for (i=finished_samples[gp_id]; i<((noOfChannels*(finished_samples[gp_id])+1)); i++)
    {
        *(Adc_resultBuffer[gp_id]+i)=ADC0_SSFIFO(SS2);
    }
    finished_samples[gp_id]++;
    SET_ISC(SS2);
    /*Sent an acknowledge by set the interrupt and status for ss2*/
    if(finished_samples[gp_id]==streaming_samp_no)
    {
        finished_samples[gp_id]=0;
    }
    /*Call the User ISR of Sample sequencer 2*/
    SS2_interrupt();
}
void ADC_Sequence1_Handler(void)
{
    u8 noOfChannels=adc_confg[SS_positions[SS1].position].no_of_inp_channels;
    u8 gp_id=adc_confg[SS_positions[SS1].position].group_ID;
    u8 streaming_samp_no=adc_groups[gp_id].no_of_streaming_samples;
    u8 i=0;
    for (i=finished_samples[gp_id]; i<((noOfChannels*(finished_samples[gp_id])+1)); i++)
    {
        *(Adc_resultBuffer[gp_id]+i)=ADC0_SSFIFO(SS1);
    }
    finished_samples[gp_id]++;
    SET_ISC(SS1);
    /*Sent an acknowledge by set the interrupt and status for ss1*/
    if(finished_samples[gp_id]==streaming_samp_no)
    {
        finished_samples[gp_id]=0;
    }
    /*Call the User ISR of Sample sequencer 1*/
    SS1_interrupt();
}
void ADC_Sequence0_Handler(void)
{
    u8 noOfChannels=adc_confg[SS_positions[SS0].position].no_of_inp_channels;
    u8 gp_id=adc_confg[SS_positions[SS0].position].group_ID;
    u8 streaming_samp_no=adc_groups[gp_id].no_of_streaming_samples;
    u8 i=0;
    for (i=finished_samples[gp_id]*noOfChannels; i<((noOfChannels*(finished_samples[gp_id])+1)); i++)
    {
        *(Adc_resultBuffer[gp_id]+i)=ADC0_SSFIFO(SS0);
    }
    finished_samples[gp_id]++;
    SET_ISC(SS0);
    /*Sent an acknowledge by set the interrupt and status for ss0*/
    if(finished_samples[gp_id]==streaming_samp_no)
    {
        finished_samples[gp_id]=0;
    }
    /*Call the User ISR of Sample sequencer 0s*/
    SS0_interrupt();
}


