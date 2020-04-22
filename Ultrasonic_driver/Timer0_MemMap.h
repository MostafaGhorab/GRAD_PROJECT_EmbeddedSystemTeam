/*
 * Timer0_MemMap.h
 *
 *  Created on: 2 Nov 2019
 *      Author: WEST
 */

#ifndef TIMER0_MEMMAP_H_
#define TIMER0_MEMMAP_H_
//***************************************************Timers Bases***************************************************************************//
#define Timer0_BASE 0x40030000
#define Timer1_BASE 0x40031000
#define Timer2_BASE 0x40032000
#define Timer3_BASE 0x40033000
#define Timer4_BASE 0x40034000
#define Timer5_BASE 0x40035000




//**************************************************Register Offsets*************************************************************************//

#define GPTMCTL_OFFSET              0x00C

#define RCGC_Timer_OFFSET           0x604

#define GPTMCFG_OFFSET              0x000

#define GPTMTAMR_OFFSET             0x004

#define GPTMTAILR_OFFSET            0x028

#define GPTMTAPR_OFFSET             0x038

#define GPTMIMR_OFFSET              0x018

#define GPTMRIS_OFFSET              0x01C

#define GPTMICR_OFFSET              0x024

#define GPTMTAMATCHR_OFFSET         0x030

#define GPTMTAR_OFFSET              0x048

#define GPTMTAV_OFFSET              0x050




//=====================================================CLOCK GATING CONTROL REGISTER BASES==================================================//
#define RCGC_BASE             0x400FE000




//***********************************************Enable_REGISTERS*****************************************************************************

#define RCGCTIMER               *((volatile u32*)(RCGC_BASE+RCGC_Timer_OFFSET ))
#define EN0_offset              0x100
#define EN1_offset              0x104
#define EN2_offset              0x108
#define EN_BASE                 0xE000E000
#define EN(Num)                 *((volatile u32*)(GPT_GEN_EnableREG(Num,EN_BASE)))






#define GPTMCTL(Timer_NUM)       *((volatile u32*)(GPT_GEN_REG(Timer_NUM,GPTMCTL_OFFSET)))

#define GPTMCFG(Timer_NUM)       *((volatile u32*)(GPT_GEN_REG(Timer_NUM,GPTMCFG_OFFSET)))

#define GPTMTAMR(Timer_NUM)      *((volatile u32*)(GPT_GEN_REG(Timer_NUM,GPTMTAMR_OFFSET)))

#define GPTMTAILR(Timer_NUM)     *((volatile u32*)(GPT_GEN_REG(Timer_NUM,GPTMTAILR_OFFSET)))

#define GPTMTAPR(Timer_NUM)      *((volatile u32*)(GPT_GEN_REG(Timer_NUM,GPTMTAPR_OFFSET)))

#define GPTMIMR(Timer_NUM)       *((volatile u32*)(GPT_GEN_REG(Timer_NUM,GPTMIMR_OFFSET)))

#define GPTMRIS(Timer_NUM)       *((volatile u32*)(GPT_GEN_REG(Timer_NUM,GPTMRIS_OFFSET)))

#define GPTMICR(Timer_NUM)       *((volatile u32*)(GPT_GEN_REG(Timer_NUM,GPTMICR_OFFSET)))

#define GPTMTAMATCHR(Timer_NUM)  *((volatile u32*)(GPT_GEN_REG(Timer_NUM,GPTMTAMATCHR_OFFSET)))

#define GPTMTAR(Timer_NUM)       *((volatile u32*)(GPT_GEN_REG(Timer_NUM,GPTMTAR_OFFSET)))

#define GPTMTAV(Timer_NUM)       *((volatile u32*)(GPT_GEN_REG(Timer_NUM,GPTMTAV_OFFSET)))




#endif /* TIMER0_MEMMAP_H_ */
