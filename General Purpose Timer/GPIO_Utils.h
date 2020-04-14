/*
 * GPIO_Utils.h
 *
 *  Created on: Oct 6, 2019
 *      Author: AhmedShawky
 */

#ifndef GPIO_UTILS_H_
#define GPIO_UTILS_H_
#include "Mem_Map.h"
#include "std_types.h"

#define GPIO_WRITE_DATA(PORT_NUM,MASK,DATA)    *((volatile u32*)(GPIODATA(PORT_NUM)+((MASK)<<2)))=DATA
#define GPIO_READ_DATA(PORT_NUM,MASK)          *((volatile u32*)(GPIODATA(PORT_NUM)+((MASK)<<2)))
#define SET_BIT(REG,PIN)                       REG|=(1<<PIN)
#define CLR_BIT(VAR,BITNO)                     (VAR) &= ~(1 << (BITNO))
#define GET_BIT(Reg,NO)                        ( (Reg>>NO) &1 )

#define CONCBIT(B7,B6,B5,B4,B3,B2,B1,B0) CONCBIT_HELPER(B7,B6,B5,B4,B3,B2,B1,B0)
#define CONCBIT_HELPER(B7,B6,B5,B4,B3,B2,B1,B0)  (0b##B7##B6##B5##B4##B3##B2##B1##B0)

#endif /* GPIO_UTILS_H_ */
