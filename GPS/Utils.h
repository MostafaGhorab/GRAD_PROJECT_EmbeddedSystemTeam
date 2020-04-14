/*
 * GPIO_Utils.h
 *
 *  Created on: Oct 6, 2019
 *      Author: AhmedShawky
 */

#ifndef UTILS_H_
#define UTILS_H_

#include "std_types.h"


#define SET_BIT(REG,PIN)                       REG|=(1<<PIN)
#define CLR_BIT(VAR,BITNO)                     (VAR) &= ~(1 << (BITNO))
#define GET_BIT(Reg,NO)                        ( (Reg>>NO) &1 )

#define CONCBIT(B7,B6,B5,B4,B3,B2,B1,B0) CONCBIT_HELPER(B7,B6,B5,B4,B3,B2,B1,B0)
#define CONCBIT_HELPER(B7,B6,B5,B4,B3,B2,B1,B0)  (0b##B7##B6##B5##B4##B3##B2##B1##B0)

#endif /* UTILS_H_ */
