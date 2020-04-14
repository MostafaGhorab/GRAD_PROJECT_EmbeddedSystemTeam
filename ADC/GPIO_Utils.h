/*
 * GPIO_Utils.h
 *
 *  Created on: Oct 6, 2019
 *      Author: AhmedShawky
 */

#ifndef GPIO_UTILS_H_
#define GPIO_UTILS_H_
#include "GPIO_Mem_Map.h"
#include "std_types.h"

#define GPIO_WRITE_DATA(PORT_NUM,MASK,DATA)    *((volatile u32*)(GPIODATA(PORT_NUM)+((MASK)<<2)))=DATA
#define GPIO_READ_DATA(PORT_NUM,MASK)          *((volatile u32*)(GPIODATA(PORT_NUM)+((MASK)<<2)))


#endif /* GPIO_UTILS_H_ */
