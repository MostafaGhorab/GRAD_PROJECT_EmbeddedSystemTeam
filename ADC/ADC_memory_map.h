/*
 * adc_memory_map.h
 *
 *  Created on: Oct 27, 2019
 *      Author: Yomna Ragab
 */

#ifndef ADC_MEMORY_MAP_H_
#define ADC_MEMORY_MAP_H_
#include "std_types.h"
#define NUMBER_OF_SS 4
extern const u32 ADC_FIFOn[NUMBER_OF_SS];
extern const u32 ADC_SSCTLn[NUMBER_OF_SS];
extern const u32 ADC_SSMUXn[NUMBER_OF_SS];
/*Reg generation*/
#define RegGen(module_num,offset)  *((volatile u32*)(module_num+offset))
/*ADC BASIS*/
#define ADC0_BASE            0x40038000
#define ADC1_BASE            0x40039000
/*ADC CLK*/
#define RCGCADC_OFFSET       0x638
#define RCGCADC_BASE         0x400FE000
#define RCGCADC              *((volatile u32*)(RCGCADC_BASE+RCGCADC_OFFSET))
/*Run Mode Clock Gating Control Register 0*/
#define RCGC0_BASE          0x400FE000
#define RCGC0_OFFSET        0x100
#define RCGC0               *((volatile u32*)(RCGC0_BASE+RCGC0_OFFSET))
/*ADC Sample Sequencer Priority */
#define ADCSSPRI_OFFSET     0x020
#define ADC0SSPRI           RegGen(ADC0_BASE,ADCSSPRI_OFFSET)
#define ADC1SSPRI           RegGen(ADC1_BASE,ADCSSPRI_OFFSET)
/*ADC Active Sample Sequencer*/
#define ADCACTSS_OFFSET     0x000
#define ADC0ACTSS           RegGen(ADC0_BASE,ADCACTSS_OFFSET)
#define ADC1ACTSS           RegGen(ADC1_BASE,ADCACTSS_OFFSET)
/*ADC Event Multiplexer Select*/
#define ADCEMUX_OFFSET      0x014
#define ADC0EMUX            RegGen(ADC0_BASE,ADCEMUX_OFFSET)
#define ADC1EMUX            RegGen(ADC1_BASE,ADCEMUX_OFFSET)
/*ADC Sample Sequence Input Multiplexer Select 3,2,1,0*/
#define ADCSSMUX3_OFFSET      0x0A0
#define ADCSSMUX2_OFFSET      0x080
#define ADCSSMUX1_OFFSET      0x060
#define ADCSSMUX0_OFFSET      0x040
#define ADC0SSMUX(SEQ)         RegGen(ADC0_BASE,ADC_SSMUXn[SEQ])
#define ADC1SSMUX(SEQ)         RegGen(ADC1_BASE,ADC_SSMUXn[SEQ])

/*ADC Sample Sequence Control 3,2,1,0*/
#define ADCSSCTL3_OFFSET      0x0A4
#define ADCSSCTL2_OFFSET      0x084
#define ADCSSCTL1_OFFSET      0x064
#define ADCSSCTL0_OFFSET      0x044
#define ADC0SSCTL(SEQ)        RegGen(ADC0_BASE,ADC_SSCTLn[SEQ])

/*ADC Sample Sequence Input Multiplexer Select 0 */
#define ADCSSMUX0_OFFSET    0x040
#define ADC0SSMUX0           RegGen(ADC0_BASE,ADCSSMUX0_OFFSET)
#define ADC1SSMUX0           RegGen(ADC1_BASE,ADCSSMUX0_OFFSET)
/*ADC Processor Sample Sequence Initiate*/
#define ADCPSSI_OFFSET        0x028
#define ADC0PSSI            RegGen(ADC0_BASE,ADCPSSI_OFFSET)
#define ADC1PSSI            RegGen(ADC1_BASE,ADCPSSI_OFFSET)
/*ADC Raw Interrupt Status*/
#define ADCRIS_OFFSET       0x004
#define ADC0RIS            RegGen(ADC0_BASE,ADCRIS_OFFSET)
#define ADC1RIS            RegGen(ADC1_BASE,ADCRIS_OFFSET)
/*ADC Sample Sequence Result FIFO 3,2,1,0*/
#define ADCSSFIFO3_OFFSET    0x0A8
#define ADCSSFIFO2_OFFSET    0x088
#define ADCSSFIFO1_OFFSET    0x068
#define ADCSSFIFO0_OFFSET    0x048
#define ADC0_SSFIFO(SEQ)    RegGen(ADC0_BASE,ADC_FIFOn[SEQ])

/*ADC Interrupt Status and Clear*/
#define ADCISC_OFFSET       0x00C
#define ADC0ISC           RegGen(ADC0_BASE,ADCISC_OFFSET)
#define SET_ISC(SEQ)      ADC0ISC|=(1<<SEQ)
#define ADC1ISC           RegGen(ADC1_BASE,ADCISC_OFFSET)
/*Analog Comparator Control 0*/
#define ACCTL0_OFFSET    0x024
#define ACCTL0_BASE      0x4003C000
#define ACCTL0           RegGen(ACCTL0_BASE,ACCTL0_OFFSET)
/*ADC Interrupt Mask*/
#define ADCIM_OFFSET     0x008
#define ADC0IM           RegGen(ADC0_BASE,ADCIM_OFFSET)
#define ADC1IM           RegGen(ADC1_BASE,ADCIM_OFFSET)
/*Interrupt Enable */
#define INTERRUPT_BASE  0xE000E000
#define EN_REG_BASE 0x100
#define EN_REG_NUM(INT_NUM) (INT_NUM/32) //to jump one reg after 32 interrupt according to datasheet
#define EN_REG(INT_NUM) *((volatile u32 *)((EN_REG_NUM(INT_NUM) * 4) + EN_REG_BASE + INTERRUPT_BASE))
#define EN_INT(INT_NUM)  EN_REG(INT_NUM) |= (1 << ((INT_NUM) % 32))
/*Interrupt priority*/
#define PRI_REG_BASE 0x400
#define PRI_REG_NUM(INT_NUM) (INT_NUM/4) //to get n which refers to PRIn and jump one reg after 4 n
#define PRI_REG(INT_NUM) *((volatile u32 *)((PRI_REG_NUM(INT_NUM) * 4) + PRI_REG_BASE + INTERRUPT_BASE))//addition of any number divided by 4
#define SET_INT_PRI(INT_NUM,PRI_NUM) PRI_REG(INT_NUM) &= ~(0x0F <<  (8*(INT_NUM%4)+5) );\
        PRI_REG(INT_NUM) |= ((PRI_NUM & 0X0F)<< (8*(INT_NUM%4)+5)) //clear then set INTA/B/C/D shifting by the remaining multiplied by 8 (to jump form 5th bit to 13th bit) added to 5



#endif /* ADC_MEMORY_MAP_H_ */
