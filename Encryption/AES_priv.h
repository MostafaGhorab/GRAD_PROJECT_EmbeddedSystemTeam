#ifndef AES_PRIV_H_INCLUDED
#define AES_PRIV_H_INCLUDED
typedef enum{
  idle=0,
  busy,
  suspended
}enumModuleStatus;
void AES_Enc_Done();
void AES_Dec_Done();
void Inverse_Subbytes(unsigned char*state);
void Inverse_Mixcolumns( u8*State);
void Inverse_ShiftRows( u8*State);
void keyExpansioncore(u8*in , u8 i);
void keyExpansion(u8* inputkey,u8* Expanded_key);
void Subbytes(u8*State);
void Shiftrows( u8*State);
void Mixcolumns( u8*State);
void Addroundkey(u8*State, u8*roundkey);
void AES_Enc_done(void);
#endif // AES_PRIV_H_INCLUDED
