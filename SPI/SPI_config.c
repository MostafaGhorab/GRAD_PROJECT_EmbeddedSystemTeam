/*
 * SPI_config.c
 *
 *  Created on: Jan 20, 2020
 *      Author: Dell
 */

#include "SPI_int.h"
#include "SPI_config.h"
#include "std_types.h"
#include "Port.h"
void test(void);
void test1(void);
Spi_Sequence ArrOfSequences[NumOfModules]=
{
 {
  SSI0,
  SlaveWithOP,
  NULL
 },
 {
  SSI3,
  Master,
  NULL
 }
};
//typedef struct{
//    u8 JobID;
//    u8 AssignedChipSelectPin;
//    enum_SpiClkPolarity ClkPol;    /*Idle Clock state*/
//    enum_SpiClkphase ClkPhase;
//    void(*JobEndNotificationPtr)(void);
//    Spi_Sequence * AssignedSeqPtr;
//    u8 SpiClk;
//}Spi_Job;
Spi_Job ArrOfJobs[NumOfJobs]=
{
 {
  1,
  PORTA_PIN3,
  steady_state_low,
  first_clk_edge_transition,
  NULL,
  ArrOfSequences,
  2000000
 },
 {
  2,
  PORTD_PIN1,
  steady_state_low,
  first_clk_edge_transition,
  NULL,
  &ArrOfSequences[1],
  2000000
 }
};

Spi_Channel ArrOfChannels[NumOfChannels]=
{
 {
  1,
  5,
  EightBitFrame,
  ArrOfJobs,
  NULL
 },
 {
  2,
  5,
  EightBitFrame,
  &ArrOfJobs[1],
  NULL
 }
};

