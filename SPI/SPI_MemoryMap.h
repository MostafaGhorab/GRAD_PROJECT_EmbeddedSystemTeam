/*
 * SPI_MemoryMap.h
 *
 *  Created on: Jan 20, 2020
 *      Author: Dell
 */

#ifndef SPI_MEMORYMAP_H_
#define SPI_MEMORYMAP_H_

#include "std_types.h"

#define SPI_BASE (u32)0x40008000
#define SPIn_BASE(SPINUM)                  ((SPINUM<<(u16)12)+SPI_BASE)
#define SPI_GEN_REG(SPINUM,OFFSET)         (SPIn_BASE(SPINUM) + OFFSET )
#define RCGCSSI_OFFSET          0x61C
#define RCGCSSI                 *((volatile u32*)(0x400FE000+RCGCSSI_OFFSET) )
#define SSICR0_OFFSET           0x000
#define SSICR1_OFFSET           0x004
#define SSIDR_OFFSET            0x008
#define SSISR_OFFSET            0x00C
#define SSICPSR_OFFSET          0x010
#define SSIIM_OFFSET            0x014
#define SSIRIS_OFFSET           0x018
#define SSIMIS_OFFSET           0x01C
#define SSIICR_OFFSET           0x020
#define SSIDMACTL_OFFSET        0x024
#define SSICC_OFFSET            0xFC8
#define SSIPeriphID4_OFFSET     0xFD0
#define SSIPeriphID5_OFFSET     0xFD4
#define SSIPeriphID6_OFFSET     0xFD8
#define SSIPeriphID7_OFFSET     0xFDC
#define SSIPeriphID0_OFFSET     0xFE0
#define SSIPeriphID1_OFFSET     0xFE4
#define SSIPeriphID2_OFFSET     0xFE8
#define SSIPeriphID3_OFFSET     0xFEC
#define SSIPCellID0_OFFSET      0xFF0
#define SSIPCellID1_OFFSET      0xFF4
#define SSIPCellID2_OFFSET      0xFF8
#define SSIPCellID3_OFFSET      0xFFC

#define SSICR0(SPINUM)        *((volatile u32*) SPI_GEN_REG(SPINUM,SSICR0_OFFSET) )
#define SPH 7
#define SPO 6
#define FRF 4
#define DSS 0
#define SSICR1(SPINUM)        *((volatile u32*) SPI_GEN_REG(SPINUM,SSICR1_OFFSET) )
#define SSE 1
#define SSIDR(SPINUM)         *((volatile u32*) SPI_GEN_REG(SPINUM,SSIDR_OFFSET) )
#define SSISR(SPINUM)         *((volatile u32*) SPI_GEN_REG(SPINUM,SSISR_OFFSET) )
#define TNF 1
#define RNE 2
#define SSICPSR(SPINUM)         *((volatile u32*) SPI_GEN_REG(SPINUM,SSICPSR_OFFSET) )
#define SSIIM(SPINUM)         *((volatile u32*) SPI_GEN_REG(SPINUM,SSIIM_OFFSET) )
#define SSIRIS(SPINUM)         *((volatile u32*) SPI_GEN_REG(SPINUM,SSIRIS_OFFSET) )

#define SSIMIS(SPINUM)         *((volatile u32*) SPI_GEN_REG(SPINUM,SSIMIS_OFFSET) )

#define SSIICR(SPINUM)         *((volatile u32*) SPI_GEN_REG(SPINUM,SSIICR_OFFSET) )

#define SSIDMACTL(SPINUM)         *((volatile u32*) SPI_GEN_REG(SPINUM,SSIDMACTL_OFFSET) )

#define SSICC(SPINUM)         *((volatile u32*) SPI_GEN_REG(SPINUM,SSICC_OFFSET) )


#endif /* SPI_MEMORYMAP_H_ */
