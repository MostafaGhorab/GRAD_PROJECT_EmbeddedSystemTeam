/*
 * I2C_MemMap.h
 *
 *  Created on: Feb 12, 2020
 *      Author: dodo_
 */
#include"std_types.h"
#ifndef I2C_MEMMAP_H_
#define I2C_MEMMAP_H_

#define I2C_BaseGen(Module_Num)      0x40020000+(Module_Num<<12)
#define RCGCI2C                      *((volatile u32*)(0x400FE620))
/* ====================================== I2C Master Registers ================================================*/

#define I2CMSA_OFFSET               0x000
#define I2CMCS_OFFSET               0x004
#define I2CMDR_OFFSET               0x008
#define I2CMTPR_OFFSET              0x00c
#define I2CMIMR_OFFSET              0x010
#define I2CMRIS_OFFSET              0x014
#define I2CMMIS_OFFSET              0x018
#define I2CMICR_OFFSET              0x01c
#define I2CMCR_OFFSET               0x020
#define I2CMCLKOCNT_OFFSET          0x024
#define I2CMBMON_OFFSET             0x02c
#define I2CMCR2_OFFSET              0x038

#define I2CMSA(Module_Num)         *((volatile u32*)(I2C_BaseGen(Module_Num) + I2CMSA_OFFSET  ))
#define I2CMCS(Module_Num)         *((volatile u32*)(I2C_BaseGen(Module_Num) + I2CMCS_OFFSET  ))
#define I2CMDR(Module_Num)         *((volatile u32*)(I2C_BaseGen(Module_Num) + I2CMDR_OFFSET  ))
#define I2CMTPR(Module_Num)        *((volatile u32*)(I2C_BaseGen(Module_Num) + I2CMTPR_OFFSET  ))
#define I2CMIMR(Module_Num)        *((volatile u32*)(I2C_BaseGen(Module_Num) + I2CMIMR_OFFSET  ))
#define I2CMRIS(Module_Num)        *((volatile u32*)(I2C_BaseGen(Module_Num) + I2CMRIS_OFFSET  ))
#define I2CMMIS(Module_Num)        *((volatile u32*)(I2C_BaseGen(Module_Num) + I2CMMIS_OFFSET  ))
#define I2CMICR(Module_Num)        *((volatile u32*)(I2C_BaseGen(Module_Num) + I2CMICR_OFFSET  ))
#define I2CMCR(Module_Num)         *((volatile u32*)(I2C_BaseGen(Module_Num) + I2CMCR_OFFSET  ))
#define I2CMCLKOCNT(Module_Num)    *((volatile u32*)(I2C_BaseGen(Module_Num) + I2CMCLKOCNT_OFFSET  ))
#define I2CMBMON(Module_Num)       *((volatile u32*)(I2C_BaseGen(Module_Num) + I2CMBMON_OFFSET  ))
#define I2CMCR2(Module_Num)        *((volatile u32*)(I2C_BaseGen(Module_Num) + I2CMCR2_OFFSET  ))


/* ====================================== I2C Slave Registers ================================================*/
#define I2CSOAR_OFFSET              0x800
#define I2CSCSR_OFFSET              0x804
#define I2CSDR_OFFSET               0x808
#define I2CSIMR_OFFSET              0x80C
#define I2CSRIS_OFFSET              0x810
#define I2CSMIS_OFFSET              0x814
#define I2CSICR_OFFSET              0x818
#define I2CSOAR2_OFFSET             0x81C
#define I2CSACKCTL_OFFSET           0x820

#define I2CSOAR(Module_Num)        *((volatile u32*)(I2C_BaseGen(Module_Num) + I2CSOAR_OFFSET  ))
#define I2CSCSR(Module_Num)        *((volatile u32*)(I2C_BaseGen(Module_Num) + I2CSCSR_OFFSET  ))
#define I2CSDR(Module_Num)         *((volatile u32*)(I2C_BaseGen(Module_Num) + I2CSDR_OFFSET  ))
#define I2CSIMR(Module_Num)        *((volatile u32*)(I2C_BaseGen(Module_Num) + I2CSIMR_OFFSET  ))
#define I2CSRIS(Module_Num)        *((volatile u32*)(I2C_BaseGen(Module_Num) + I2CSRIS_OFFSET  ))
#define I2CSMIS(Module_Num)        *((volatile u32*)(I2C_BaseGen(Module_Num) + I2CSMIS_OFFSET  ))
#define I2CSICR(Module_Num)        *((volatile u32*)(I2C_BaseGen(Module_Num) + I2CSICR_OFFSET  ))
#define I2CSOAR2(Module_Num)       *((volatile u32*)(I2C_BaseGen(Module_Num) + I2CSOAR2_OFFSET  ))
#define I2CSACKCTL(Module_Num)     *((volatile u32*)(I2C_BaseGen(Module_Num) + I2CSACKCTL_OFFSET  ))


#endif /* I2C_MEMMAP_H_ */
