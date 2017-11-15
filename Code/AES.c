//AES

#include <stdio.h>
#include <stdint.h>
#include "aes.h"
//#include "galois.h"

void addRoundKey(uint8_t*, uint32_t*);
void byteSubstitution(uint8_t*);
void encryptBlock(uint8_t*, uint32_t*);
uint32_t g(uint32_t, uint8_t);
void keyExspansion(uint32_t*);
void mixColumn(uint8_t*);
void shiftRows(uint8_t*);

//Substitution table
uint8_t sBox[256] =
{0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
 0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
 0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
 0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
 0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
 0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
 0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
 0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
 0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
 0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
 0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
 0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
 0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
 0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
 0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
 0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16};

uint8_t   roundCoeffients[10] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36};
uint32_t  subKeys[44];

void addRoundKey(uint8_t* state, uint32_t* roundKey)
{
  for(uint8_t i = 0; i < 4; i++) //This can be unrolled
  {
    uint8_t statePostion = i << 2;
    state[statePostion]     = state[statePostion]     ^ (*(roundKey + i) >> 24);
    state[statePostion + 1] = state[statePostion + 1] ^ ((*(roundKey + i) >> 16)&0xFF);
    state[statePostion + 2] = state[statePostion + 2] ^ ((*(roundKey + i) >> 8)&0xFF);
    state[statePostion + 3] = state[statePostion + 3] ^ (*(roundKey + i)&0xFF);
  }
}

void byteSubstitution(uint8_t* state)
{
  for(int8_t i = 0; i < 16; i++) //This can be unrolled
  {
    state[i] = sBox[state[i]];
  }
}

void encryptBlock(uint8_t* input, uint32_t* key)
{
  keyExspansion(key);

  addRoundKey(input, key);

  for(uint8_t i = 1; i < 10; i++)
  {
    byteSubstitution(input);
    mixColumn(input);
    addRoundKey(input, key + (i << 2));
  }

  byteSubstitution(input);
  shiftRows(input);
  addRoundKey(input, key + 40);
  for(uint16_t i = 0; i < 16; i++)
  {
    printf("%X", input[i]);
    if(((i+1)&0x3) == 0)
    {
      printf("\n");
    }
  }

}

uint32_t g(uint32_t subKey, uint8_t roundCoeffient)
{
  uint32_t gSubKey = ((sBox[(subKey >> 16)&0xFF] ^ roundCoeffient) << 24) | (sBox[(subKey >> 8)&0xFF] << 16) | (sBox[subKey&0xFF] << 8) | sBox[(subKey >> 24)&0xFF];
  return gSubKey;
}

void keyExspansion(uint32_t* subKeys)
{
  for(uint8_t i = 1; i < 11; i++)
  {
    uint8_t keyPosition = i << 2;
    subKeys[keyPosition]     = subKeys[keyPosition - 4] ^ g(subKeys[keyPosition - 1], roundCoeffients[i - 1]);
    subKeys[keyPosition + 1] = subKeys[keyPosition]     ^ subKeys[keyPosition - 3];
    subKeys[keyPosition + 2] = subKeys[keyPosition + 1] ^ subKeys[keyPosition - 2];
    subKeys[keyPosition + 3] = subKeys[keyPosition + 2] ^ subKeys[keyPosition - 1];
  }
}

void mixColumn(uint8_t* state)
{
  uint8_t tempState[16];
  uint8_t Bx2[16];
  uint8_t Bx3[16];

  for(int16_t i = 0; i < 16; i++)
  {
    Bx2[i] = state[i] << 1;
    Bx3[i] = state[i] << 1 ^ state[i];

    if((state[i] & 0x80) != 0)
    {
      Bx2[i] ^= 0x1B;
      Bx3[i] ^= 0x1B;
    }
  }

  tempState[0]  =    Bx2[0] ^    Bx3[5] ^ state[10] ^ state[15];
  tempState[1]  =  state[0] ^    Bx2[5] ^   Bx3[10] ^ state[15];
  tempState[2]  =  state[0] ^  state[5] ^   Bx2[10] ^   Bx3[15];
  tempState[3]  =    Bx3[0] ^  state[5] ^ state[10] ^   Bx2[15];
  tempState[4]  =    Bx2[4] ^    Bx3[9] ^ state[14] ^  state[3];
  tempState[5]  =  state[4] ^    Bx2[9] ^   Bx3[14] ^  state[3];
  tempState[6]  =  state[4] ^  state[9] ^   Bx2[14] ^    Bx3[3];
  tempState[7]  =    Bx3[4] ^  state[9] ^ state[14] ^    Bx2[3];
  tempState[8]  =    Bx2[8] ^   Bx3[13] ^  state[2] ^  state[7];
  tempState[9]  =  state[8] ^   Bx2[13] ^    Bx3[2] ^  state[7];
  tempState[10] =  state[8] ^ state[13] ^    Bx2[2] ^    Bx3[7];
  tempState[11] =    Bx3[8] ^ state[13] ^  state[2] ^    Bx2[7];
  tempState[12] =   Bx2[12] ^    Bx3[1] ^  state[6] ^  state[11];
  tempState[13] = state[12] ^    Bx2[1] ^    Bx3[6] ^  state[11];
  tempState[14] = state[12] ^  state[1] ^    Bx2[6] ^    Bx3[11];
  tempState[15] =   Bx3[12] ^  state[1] ^  state[6] ^    Bx2[11];

  for(int16_t i = 0; i < 16; i++)
  {
    state[i] = tempState[i];
  }
}

void shiftRows(uint8_t* state)
{
  uint8_t tempState[16];

  tempState[0]  = state[0];
  tempState[1]  = state[5];
  tempState[2]  = state[10];
  tempState[3]  = state[15];
  tempState[4]  = state[4];
  tempState[5]  = state[9];
  tempState[6]  = state[14];
  tempState[7]  = state[3];
  tempState[8]  = state[8];
  tempState[9]  = state[13];
  tempState[10] = state[2];
  tempState[11] = state[7];
  tempState[12] = state[12];
  tempState[13] = state[1];
  tempState[14] = state[6];
  tempState[15] = state[11];

  for(int16_t i = 0; i < 16; i++)
  {
    state[i] = tempState[i];
  }
}
