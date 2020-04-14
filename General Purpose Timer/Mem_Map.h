/*
 * MemMap.h
 *
 *  Created on: Oct 5, 2019
 *      Author: AhmedShawky
 */

#ifndef MEM_MAP_H_
#define MEM_MAP_H_
#include "std_types.h"

#define GPIO_PORTA_BASE                        0x40004000
#define GPIO_PORTB_BASE                        0x40005000
#define GPIO_PORTC_BASE                        0x40006000
#define GPIO_PORTD_BASE                        0x40007000
#define GPIO_PORTE_BASE                        0x40024000
#define GPIO_PORTF_BASE                        0x40025000

#define GPIODATA_OFFSET                        0x000
#define GPIODIR_OFFSET                         0x400
#define GPIOIS_OFFSET                          0x404
#define GPIOIBE_OFFSET                         0x408
#define GPIOIEV_OFFSET                         0x40C
#define GPIOIM_OFFSET                          0x410
#define GPIORIS_OFFSET                         0x414
#define GPIOMIS_OFFSET                         0x418
#define GPIOICR_OFFSET                         0x41C
#define GPIOAFSEL_OFFSET                       0x420
#define GPIODR2R_OFFSET                        0x500
#define GPIODR4R_OFFSET                        0x504
#define GPIODR8R_OFFSET                        0x508
#define GPIOODR_OFFSET                         0x50C
#define GPIOPUR_OFFSET                         0x510
#define GPIOPDR_OFFSET                         0x514
#define GPIOSLR_OFFSET                         0x518
#define GPIODEN_OFFSET                         0x51C
#define GPIOLOCK_OFFSET 					   0x520
#define GPIOCR_OFFSET                          0x524
#define GPIOAMSEL_OFFSET 					   0x528
#define GPIOPCTL_OFFSET 					   0x52C
#define GPIOADCCTL_OFFSET                      0x530
#define GPIODMACTL_OFFSET                      0x534
#define GPIOPeriphID4_OFFSET                   0xFD0
#define GPIOPeriphID5_OFFSET 				   0xFD4
#define GPIOPeriphID6_OFFSET                   0xFD8
#define GPIOPeriphID7_OFFSET                   0xFDC
#define GPIOPeriphID0_OFFSET                   0xFE0
#define GPIOPeriphID1_OFFSET                   0xFE4
#define GPIOPeriphID2_OFFSET                   0xFE8
#define GPIOPeriphID3_OFFSET                   0xFEC
#define GPIOPCellID0_OFFSET                    0xFF0
#define GPIOPCellID1_OFFSET                    0xFF4
#define GPIOPCellID2_OFFSET                    0xFF8
#define GPIOPCellID3_OFFSET                    0xFFC





//===========================================================REGISTERS============================================================


#define GPIODATA(PORT_ID)                GPIO_GEN_REG(PORT_ID,GPIODATA_OFFSET)
#define GPIODIR(PORT_ID)                 *((volatile u32*)(GPIO_GEN_REG(PORT_ID,GPIODIR_OFFSET)))
#define GPIOIS(PORT_ID)                  *((volatile u32*)(GPIO_GEN_REG(PORT_ID,GPIOIS_OFFSET)))
#define GPIOIBE(PORT_ID)                 *((volatile u32*)(GPIO_GEN_REG(PORT_ID,GPIOIBE_OFFSET)))
#define GPIOIEV(PORT_ID)                 *((volatile u32*)(GPIO_GEN_REG(PORT_ID,GPIOIEV_OFFSET)))
#define GPIOIM(PORT_ID)                  *((volatile u32*)(GPIO_GEN_REG(PORT_ID,GPIOIM_OFFSET)))
#define GPIORIS(PORT_ID)                 *((volatile u32*)(GPIO_GEN_REG(PORT_ID,GPIORIS_OFFSET)))
#define GPIOMIS(PORT_ID)                 *((volatile u32*)(GPIO_GEN_REG(PORT_ID,GPIOMIS_OFFSET)))
#define GPIOICR(PORT_ID)                 *((volatile u32*)(GPIO_GEN_REG(PORT_ID,GPIOICR_OFFSET)))
#define GPIOAFSEL(PORT_ID)               *((volatile u32*)(GPIO_GEN_REG(PORT_ID,GPIOAFSEL_OFFSET)))
#define GPIODR2R(PORT_ID)                *((volatile u32*)(GPIO_GEN_REG(PORT_ID,GPIODR2R_OFFSET)))
#define GPIODR4R(PORT_ID)                *((volatile u32*)(GPIO_GEN_REG(PORT_ID,GPIODR4R_OFFSET)))
#define GPIODR8R(PORT_ID)                *((volatile u32*)(GPIO_GEN_REG(PORT_ID,GPIODR8R_OFFSET)))
#define GPIOODR(PORT_ID)                 *((volatile u32*)(GPIO_GEN_REG(PORT_ID,GPIOODR_OFFSET)))
#define GPIOPUR(PORT_ID)                 *((volatile u32*)(GPIO_GEN_REG(PORT_ID,GPIOPUR_OFFSET)))
#define GPIOPDR(PORT_ID)                 *((volatile u32*)(GPIO_GEN_REG(PORT_ID,GPIOPDR_OFFSET)))
#define GPIOSLR(PORT_ID)                 *((volatile u32*)(GPIO_GEN_REG(PORT_ID,GPIOSLR_OFFSET)))
#define GPIODEN(PORT_ID)                 *((volatile u32*)(GPIO_GEN_REG(PORT_ID,GPIODEN_OFFSET)))
#define GPIOLOCK(PORT_ID)                *((volatile u32*)(GPIO_GEN_REG(PORT_ID,GPIOLOCK_OFFSET)))
#define GPIOCR(PORT_ID)                  *((volatile u32*)(GPIO_GEN_REG(PORT_ID,GPIOCR_OFFSET)))
#define GPIOAMSEL(PORT_ID)               *((volatile u32*)(GPIO_GEN_REG(PORT_ID,GPIOAMSEL_OFFSET)))
#define GPIOPCTL(PORT_ID)                *((volatile u32*)(GPIO_GEN_REG(PORT_ID,GPIOPCTL_OFFSET)))
#define GPIOADCCTL(PORT_ID)              *((volatile u32*)(GPIO_GEN_REG(PORT_ID,GPIOADCCTL_OFFSET)))
#define GPIODMACTL(PORT_ID)              *((volatile u32*)(GPIO_GEN_REG(PORT_ID,GPIODMACTL_OFFSET)))
#define GPIOPeriphID4(PORT_ID)           *((volatile u32*)(GPIO_GEN_REG(PORT_ID,GPIOPeriphID4_OFFSET)))
#define GPIOPeriphID5(PORT_ID)           *((volatile u32*)(GPIO_GEN_REG(PORT_ID,GPIOPeriphID5_OFFSET)))
#define GPIOPeriphID6(PORT_ID)           *((volatile u32*)(GPIO_GEN_REG(PORT_ID,GPIOPeriphID6_OFFSET)))
#define GPIOPeriphID7(PORT_ID)           *((volatile u32*)(GPIO_GEN_REG(PORT_ID,GPIOPeriphID7_OFFSET)))
#define GPIOPeriphID0(PORT_ID)           *((volatile u32*)(GPIO_GEN_REG(PORT_ID,GPIOPeriphID0_OFFSET)))
#define GPIOPeriphID1(PORT_ID)           *((volatile u32*)(GPIO_GEN_REG(PORT_ID,GPIOPeriphID1_OFFSET)))
#define GPIOPeriphID2(PORT_ID)           *((volatile u32*)(GPIO_GEN_REG(PORT_ID,GPIOPeriphID2_OFFSET)))
#define GPIOPeriphID3(PORT_ID)           *((volatile u32*)(GPIO_GEN_REG(PORT_ID,GPIOPeriphID3_OFFSET)))
#define GPIOPCellID0(PORT_ID)            *((volatile u32*)(GPIO_GEN_REG(PORT_ID,GPIOPCellID0_OFFSET)))
#define GPIOPCellID1(PORT_ID)            *((volatile u32*)(GPIO_GEN_REG(PORT_ID,GPIOPCellID1_OFFSET)))
#define GPIOPCellID2(PORT_ID)            *((volatile u32*)(GPIO_GEN_REG(PORT_ID,GPIOPCellID2_OFFSET)))
#define GPIOPCellID3(PORT_ID)            *((volatile u32*)(GPIO_GEN_REG(PORT_ID,GPIOPCellID3_OFFSET)))

//=====================================================CLOCK GATING CONTROL REGISTER BASES==================================================
#define RCGC_BASE             0x400FE000


//=====================================================CLOCK GATING CONTROL REGISTER OFFSETS==============================================
#define RCGC_GPIO_OFFSET           0x108



//===================================================CLOCK GATING CONTROL REGISTERS======================================================
#define RCGCGPIO                 *((volatile u32*)(RCGC_BASE+RCGC_GPIO_OFFSET))





#endif /* MEM_MAP_H_ */
