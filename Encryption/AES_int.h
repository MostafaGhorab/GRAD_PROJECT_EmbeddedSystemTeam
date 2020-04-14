#ifndef AES_INT_H_INCLUDED
#define AES_INT_H_INCLUDED
typedef enum
{
  AES_request_accepted=0,
  AES_request_denied
}enumAES_ReqStatus;
typedef enum
{
    data_not_available=0,
    data_available

}enumAES_DataStaus;
void AES_vidEncInit(void);
void AES_vidDecInit(void);
void AES_Encryption(void);
void AES_Decryption(void);
/**
        Function name:
        Return:None.
        Arguments: CipheredData , hygufofh
*/
extern u8 enc_done_flag;
extern u8 Dec_done_flag;
enumAES_ReqStatus AES_RequestToEnc(u8*message,u32 Message_Size);

enumAES_DataStaus AES_EncDataGetter(u8 * data);

enumAES_ReqStatus AES_RequestToDec(u8 *encr_data,u32 msgDe_Size);
#endif // AES_INT_H_INCLUDED
