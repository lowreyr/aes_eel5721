#ifndef AES_H
#define AES_H

//typedef uchar uint8_t;

void addRoundKey(__global uchar*, __global uchar*);
void byteSubstitution(__global uchar*);
//void decryptBlock(uint8_t*, uint32_t*);
//void encryptBlock(uint8_t*, uint32_t*);
//uint32_t g(uint32_t, cl_uchar);
//void keyExspansion(uint32_t*);
//void mixColumn(uint8_t*);
//void shiftRows(uint8_t*);
//void inverseByteSubstitution(uint8_t*);
//void inverseMixColumn(uint8_t*);
//void inverseShiftRows(uint8_t*);

//Substitution table
//uchar sBox[256];

 // uint32_t rotl32c (uint32_t x, uint32_t n)
 // {
 //   assert (n<32);
 //   return (x<<n) | (x>>(-n&31));
 // }

#endif
