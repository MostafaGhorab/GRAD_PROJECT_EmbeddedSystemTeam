/*
 * ADC_config.c
 *
 *  Created on: Dec 6, 2019
 *      Author: Yomna
 */

#include "ADC_int.h"
#include "ADC_config.h"


const strGroupInfo adc_groups[NUMOFGROUPS]={
                                                   {0,CONV_ONE_SHOT,SINGLE_ACCESS,SW,NONE,1,{AIN0}}
};
/*
 * array of structure called strGroupInfo  its size configurable in config.h, it contains in order:
 *
    -group id                        -->The group id ,started from 0.
    -conversion mode                 -->one shot or continuous.
    -access mode                     -->single access or streaming)
    -trigger                         -->software or hardware.
    -HW_trigger_source               --> in case of hardware trigger , the source is PWM or timer or comparator or GPIO pins
                                        in case of software trigger , write none.
    -no_of_streaming_samples         -->number of streaming samples , started from 1.
    -ss_pins[MAX_CHANNELS_PER_GROUP] --> array of the pins in the group.
 *
 * */
const strModuleInfo adc_confg[NUMOFMODULES]={
                                                    {0,SS3,SS_125k,1,0}
};
/*
 * array of structure called strModuleInfo  its size configurable in config.h, it contains in order:
{
    - module_ID              ->The module id ,started from 0.
    - sequencer_no           -> sequencer no , it can be 0,1,2,3
    - clk                    -> the sampling rate , it can be 125k , 250k, 500k, 1M.
    - no_of_inp_channels     -> Number of input channels of the group which will parsed to this module.
    - group_ID               -> The group ID which will be parsed to this module with those specification.
*/


