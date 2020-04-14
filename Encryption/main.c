#include "std_types.h"
#include "AES_priv.h"
#include "AES_int.h"
#define KEY_SIZE 16
#define SBOX_SIZE 256
#define EXPANDED_KEY_SIZE 176
#define RCON 256
#define MUL 257
#define MAX_SIZE_OF_A_MESSAGE 24
#define MAX_SIZE_OF_A_CIPHEREDDATA 32
int main(void)
{
    u8 Data[MAX_SIZE_OF_A_CIPHEREDDATA];
    AES_vidEncInit();
    AES_vidDecInit();
    u32 Message_Size=24;
    u8 Message[150]="Look out There is an accident on the second";
    u8 Decrypted_Message[MAX_SIZE_OF_A_MESSAGE];
    AES_RequestToEnc(Message,Message_Size);
    AES_Encryption();
    AES_Encryption();
    AES_Encryption();
    AES_Encryption();
    AES_Encryption();
    AES_Encryption();
    AES_Encryption();
    AES_Encryption();
    AES_Encryption();
    if (enc_done_flag==1)
    {
        AES_EncDataGetter(Data);
    }
AES_RequestToDec(Data,Message_Size);
    AES_Decryption();
    AES_Decryption();
    AES_Decryption();
    AES_Decryption();
    AES_Decryption();
    AES_Decryption();
    AES_Decryption();
    AES_Decryption();
    AES_Decryption();
 return 0;
}
