/*
 * GPS_int.h
 *
 *  Created on: Feb 3, 2020
 *      Author: Dell
 */

#ifndef GPS_INT_H_
#define GPS_INT_H_
typedef enum{
    Done=0,
    Error=1
}enumcheckerror;
/*Check error enum type*/
typedef enum{
    Req_accepted,
    Req_notaccepted
}enumRequestStatus;
/*Check error for user requests*/
extern void (*GPS_ptr)(void);
/*Pointer to function which must be called after reception is finished*/
void GPS_init(void);
/*
 * GPS_init: To initialize the GPS parameters.
 * Return:None.
 * Arguments:None.
 * */
void GPS_main(void);
/*
 * GPS_main: It's a periodic function called by the RTOS , it checks if there's a request to get the frame or not, if yes ,It calls the UART to request to recieve.
 * Return:None.
 * Arguments:None.
 * */
enumRequestStatus GPS_RequestToHandle(void);
/*
 * GPS_RequestToHandle: It's a request from the user to ask to read and handle the frame.
 * Return : enumRequestStatus , it's an enum type , it returns wheather the request is accepted or not.
 * Arguments:None.
 */

void GPS_enumGetData(u8 * Latitude , u8 * Longitude , u8 * TIME , u8 * speed ,u8 * DATE);
/*
 * GPS_enumGetData: It gives the frame information to user , which are latitude , longitude , time , speed and date.
 * Return : None.
 * Arguments: Latitude pointer to u8 , it must be 11 bytes , and its form is ddmm.mmmmm
 *            Longitude pointer to u8 , it must be 12 bytes , and its form is dddmm.mmmmm
 *            Time pointer to u8 , it must be 9 bytes , and its form is hhmmss.ss
 *            Speed pointer to u8 , it must be 8 bytes , and its form is 000.000 in units of knots
 *            Date pointer to u8 , it must be 7 bytes , and its form is ddmmyy
 */
#endif /* GPS_INT_H_ */
