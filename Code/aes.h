#ifndef AES_H
#define AES_H

void addRoundKey(uint8_t*, uint32_t*);
void byteSubstitution(uint8_t*);
void decryptBlock(uint8_t*, uint32_t*);
void encryptBlock(uint8_t*, uint32_t*);
uint32_t g(uint32_t, uint8_t);
void keyExspansion(uint32_t*);
void mixColumn(uint8_t*);
void shiftRows(uint8_t*);
void inverseByteSubstitution(uint8_t*);
void inverseMixColumn(uint8_t*);
void inverseShiftRows(uint8_t*);

//Substitution table
uint8_t sBox[256];

 // uint32_t rotl32c (uint32_t x, uint32_t n)
 // {
 //   assert (n<32);
 //   return (x<<n) | (x>>(-n&31));
 // }

#endif
