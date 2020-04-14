/*
 * ADC_priv.h
 *
 *  Created on: Oct 29, 2019
 *      Author: Yomna Ragab
 */

#ifndef ADC_PRIV_H_
#define ADC_PRIV_H_



#define INT_NO_SHIFT (u8)14


#define NIBBLE_SHIFT (u8)0x4
#define LAST_SAMPLE_EN_AND_INTERRUPT (u8)0x6
#define OUTPUT               1
#define INPUT                0
#define SET_BIT(REG,PIN)            REG|=(1<<PIN)
typedef struct
{
    u8 position;
    u8 ss_no;
    u8 gp_id;
}strSS_pos;

void ADC_vidSetAlternateFunc(u8 pin,u8 desired_port);
void ADC_vidDisableDEN(u8 pin,u8 desired_port);
void ADC_vidEnAnalogFunc(u8 pin,u8 desired_port);
void ADC_EnableDisableSeqSel(u8 control,u8 sample_seq);
void ADC_vidTrigger(u8,u8);
void ADC_vidSSCTL(u8 seq,u8 no_of_channels);
#define CONCBIT_SEQ(P3,P2,P1,P0) CONCBIT_HELPER_SEQ(P3,P2,P1,P0)
#define CONCBIT_HELPER_SEQ(P3,P2,P1,P0)  (0x##P3##P2##P1##P0)
#endif /* ADC_PRIV_H_ */
