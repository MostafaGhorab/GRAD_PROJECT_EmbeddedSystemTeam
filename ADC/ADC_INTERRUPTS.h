/*
 * ADC_INTERRUPTS.h
 *
 *  Created on: Dec 1, 2019
 *      Author: Yomna Ragab
 */

#ifndef ADC_INTERRUPTS_H_
#define ADC_INTERRUPTS_H_

/*****User ISR functions , it takes pointer to function which will be called when interrupt occurs*****/

/*for seq 0*/
void SS0_ISR(void(*ptr)(void));
/*for seq 1*/
void SS1_ISR(void(*ptr)(void));
/*for seq 2*/
void SS2_ISR(void(*ptr)(void));
/*for seq 3*/
void SS3_ISR(void(*ptr)(void));


void ADC_Sequence0_Handler(void);
void ADC_Sequence1_Handler(void);
void ADC_Sequence2_Handler(void);
void ADC_Sequence3_Handler(void);
/*
 * Func: ADC_SequenceN_Handler , they are the ISR for each sequencer , After each conversion it's called by default
 * Arguments: None.
 * Return :None
 * note: it can't be called by user
 * */


#endif /* ADC_INTERRUPTS_H_ */
