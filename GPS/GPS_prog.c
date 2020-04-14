/*
 * GPS_ptog.c
 *
 *  Created on: Feb 3, 2020
 *      Author: Dell
 */


#include "std_types.h"
#include "GPS_int.h"
#include "UART_int.h"
#include "UART_priv.h"
#include "GPS_config.h"
#include "GPS_priv.h"
/*
 * Frame form:
 *  $xxRMC,time,module_status,lat,NS,lon,EW,spd,cog,date,mv,mvEW,posMode,navStatus*cs<CR><LF>     */
#define INFO_BYTES 70
#define TIME_POS 7
#define LOC_BYTES 40
#define LAT_POS 19
#define LONG_POS 32
#define SPD_POS 46
#define DATE_POS 58
#define LAT_SIZE 11
#define LONG_SIZE 12
#define DATA_SIZE 1000
#define TIME_SIZE 10
#define SPD_SIZE 8
#define DATE_SIZE 7
#define TIME_OUT 20
static u8 Lati[LAT_SIZE]={0};
static u8 Long[LONG_SIZE]={0};
static u8 data[DATA_SIZE]={0};
static u8 time[TIME_SIZE]={0};
static u8 spd[SPD_SIZE]={0};
static u8 date[DATE_SIZE]={0};
static u8 reception_flag=0;
static u8 request_flag=0;
static GPS_status module_status;
static u16 comm_pos=0;
void (*GPS_ptr)(void)=GPS_vidprivframehandling;
/*ptr to func which is must be called after uart reception is finished,it's a parameter of uart channel configuration*/
void GPS_init(void)
{
    u16 counter=0;
    for (counter = 0; counter < LAT_SIZE; counter++)
    {
        Lati[counter] = 0;
    }
    for (counter = 0; counter < LONG_SIZE; counter++)
    {
        Long[counter] = 0;
    }
    for (counter = 0; counter < DATA_SIZE; counter++)
    {
        data[counter] = 0;
    }
    for (counter = 0; counter < TIME_SIZE; counter++)
    {
        time[counter] = 0;
    }
    for (counter = 0; counter < SPD_SIZE; counter++)
    {
        spd[counter] = 0;
    }
    for (counter = 0; counter < DATE_SIZE; counter++)
    {
        date[counter] = 0;
    }


    module_status=idle;
}

/************************************************************/
enumRequestStatus GPS_RequestToHandle(void)
{
    enumRequestStatus request_status=Req_notaccepted;
    if(module_status==idle) /*if gps is in idle module_status , accept the request and rise the request_flag.*/
    {
        request_flag=1; /*Rise the request_flag*/
        module_status=suspended; /*Change the GPS module_status*/
        request_status=Req_accepted; /*return that the request is accepted*/
    }
    return(request_status);

}
/************************************************************/
void GPS_main(void)
{
    u8 i=0;
    u8 receiption_flag=0;
    if (request_flag==1) /*If there's a request to handle the frame , then request to receive a frame from UART*/
    {
        receiption_flag=UART_enumRequestToRecieve (GPS_CH, DATA_SIZE, data);
        while( (!receiption_flag) && (i<TIME_OUT) ) /* i is a counter to avoid stucking , as long as the request is denied and i is less than 20
                                                      , keep trying. it calls the reception function of UART and gives it the GPS channel ,
                                                        length of data , and pointer to the buffer*/
        {
            receiption_flag=UART_enumRequestToRecieve (GPS_CH, DATA_SIZE, data);
            i++;
        }
        if(i != TIME_OUT)
        {
            module_status=busy;
            request_flag=0;
        }

    }
    if (reception_flag==1) //reception is finished
    {
        module_status=processing;
        GPS_vidprivframehandling();
        module_status=idle;
    }
}

void GPS_vidprivframehandling(void)
{

    u16 loopIndex=0;
    for(loopIndex=0; loopIndex<1000; loopIndex++)
    {
        if((data[loopIndex]=='$') && (data[loopIndex+1]=='G') && (data[loopIndex+2]=='N') && (data[loopIndex+3]=='R'))
        {
            GPS_vidPrivTime(loopIndex);
            GPS_vidPrivLatLong(loopIndex);
            GPS_vidPrivSpeed(loopIndex);
            GPS_vidPrivDate(loopIndex);
            reception_flag=0;
            break;
        }

    }

}
void GPS_vidPrivTime(u8 loopIndex)
{
    u8 i;
    u16 timeIndex=loopIndex+TIME_POS;

    for (i=0; i<TIME_SIZE-1; i++)
    {
        time[i]=data[timeIndex+i];

        /*assign the TIME by data array , which has the required frame ,  TIME is 9 char :hhmmss.ss*/
    }
}
void GPS_vidPrivLatLong(u8 loopIndex)
{
    u8 i;
    u16 latIndex=loopIndex+LAT_POS;
    u16 longIndex=loopIndex+LONG_POS;
    for (i=0; i<LAT_SIZE-1; i++)
    {
        Lati[i]=data[latIndex+i];
        /*assign the latitude by data array , which has the required frame ,  lat is 10 char :ddmm.mmmmm*/
        Long[i]=data[longIndex+i];
        /*assign the longitude by data array , which has the required frame ,  long is 11 char :dddmm.mmmmm*/
    }
    Long[i]=data[longIndex+i];
    /*to assign the last digit in longitude as it's greater by 1 thn latitude*/
}
void GPS_vidPrivSpeed(u8 loopIndex)
{
    u8 i=0;
    u16 spdIndex=loopIndex+SPD_POS;
    while(data[spdIndex+i] != ',') /*As speed form is not constant as previous parameters*/
    {
        spd[i]=data[spdIndex+i];
        /*assign the SPEED by data array , which has the required frame ,  SPEED is 7 numeric 000.000(almost)*/
        i++;
    }
    comm_pos=spdIndex+i; /*it helps to get the next data positions in the frame*/
}
void GPS_vidPrivDate(u8 loopIndex)
{
    u8 i=1;
    u16 dateindex=0;
    while (data[comm_pos+i] != ',') /*Search for the next comma to get th position of date*/
    {
        i++;
    }
    dateindex=comm_pos+i+1;
    for (i=0; i<DATE_SIZE-1; i++)
    {
        date[i]=data[dateindex+i];
        /* assign the date by data array , which has the required frame ,  date is 6 numeric ddmmyy*/
    }
}
void GPS_enumGetData(u8 * Latitude , u8 * Longitude , u8 * TIME , u8 * speed ,u8 * DATE)
{
    u8 i=0;
    while(Lati[i]!='\0')
    {
        Latitude[i]=Lati[i];
        i++;
    }
    i=0;
    while(Long[i]!='\0')
    {
        Longitude[i]=Long[i];
        i++;
    }
    i=0;
    while(time[i]!='\0')
    {
        TIME[i]=time[i];
        i++;
    }
    i=0;
    while(spd[i]!='\0')
    {
        speed[i]=spd[i];
        i++;
    }
    i=0;
    while(date[i]!='\0')
    {
        DATE[i]=date[i];
        i++;
    }
}
void GPS_ReceptionDone(void)
{
    reception_flag=1;
}

/*****************************************************************************************************************************/
/***************************************************************************************************************************/


#include "DIO.h"
#include "DIO_Config.h"
#include "DIO_types.h"
#include "DIO_Utils.h"
#include "DIO_Priv.H"
#include "Port_Config.h"
#include "Port_Priv.h"
#include "GPIO_MemMap.h"
#include "utils.h"
#include "std_types.h"
#include"Priv.h"
#include "UART_int.h"
#include "UART_priv.h"
#include "UART_Config.h"
#include "UART_MemMap.h"

void delay(u32 count);
extern u16 RecieveDataLength [NUMBER_OF_MODULES];

void main()
{

    Port_Init();
    Port_SetPinMode(9,0x01);
    Port_SetPinMode(8,0x01);
    UART_init () ;
    SET_BIT(UARTCTL(UART1),8);
    SET_BIT(UARTCTL(UART1),9);
    u8 Latitude[11];
    u8 Longitude[12];
    u8 TIME[10];
    u8 speed[8];
    u8 DATE[7];
    GPS_init();
   // UART_enumRequestToRecieve (1, 80 , data);
    GPS_RequestToHandle();
    while(1)
    {
        GPS_main();
        UART_mainFunc();
        GPS_main();
        GPS_enumGetData(Latitude , Longitude ,  TIME , speed ,DATE);

        // UART_enumRequestToSend(1,11,Latitude);
        // UART_enumRequestToSend(1,12,Longitude);
        //UART_enumRequestToSend(1,10,TIME);
        // UART_enumRequestToSend(1,8,speed);
        UART_enumRequestToSend(1,7,DATE);

    }

}



void delay(u32 count)
{
    u32 volatile  i=0;
    for(i=0; i<count; i++);
}















