//AES

#include "aes.h"
//#include "galois.h"

#pragma OPENCL EXTENSION cl_intel_channels : enable

//Substitution table
constant uchar sBox[256] =
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

//Inverse substitution table
constant uchar sBoxInverse[256] = {
   	0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
   	0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
   	0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
   	0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
   	0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
   	0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
   	0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
   	0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
   	0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
   	0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
   	0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
   	0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
   	0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
   	0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
   	0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
   	0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d,
  };

constant uchar   roundCoeffients[10] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36};
//Add other round coeffiencients
//uint32_t  subKeys[44];

channel uchar chb0;
channel uchar chm0;
channel uchar cha1;

__kernel void addRoundKey0(__global uchar* state, __global uchar* roundKey)
{
  //for(int r = 0; r < 16; r++)
  //{
  //  state[r] = read_channel_intel(cha0);
  //}
  uchar data;
  uchar key;
  uchar result;
  printf("\naddRoundKey0: ");
  for(int i = 0; i < 16; i++)
  {
    //uchar statePosition = i << 2;
    //state[statePosition]     = state[statePosition]     ^ (*(roundKey + i) >> 24);
    //state[statePosition + 1] = state[statePosition + 1] ^ ((*(roundKey + i) >> 16)&0xFF);
    //state[statePosition + 2] = state[statePosition + 2] ^ ((*(roundKey + i) >> 8)&0xFF);
    //state[statePosition + 3] = state[statePosition + 3] ^ (*(roundKey + i)&0xFF);
    data = state[i];
    key = roundKey[i];
    result = data ^ key;
    //state[i] = state[i] ^ roundKey[i];
  //}
  //for(int w = 0; w < 16; w++)
  //{
    write_channel_intel(chb0, result);
    //printf("data: %x, key: %x, result: %x\n",data,key,result);
    printf("%x ",result);
  }
  write_channel_intel(chr0, roundKey);
}

// __kernel void byteSubstitution0()
// {
//   uchar temp;
//   printf("\nbyteSubstition0: ");
//   for(int i = 0; i < 16; i++)
//   {
//     temp = read_channel_intel(chb0);
//     temp = sBox[temp];
//     printf("%x ",temp);
//     write_channel_intel(chm0, temp);
//   }
// }

// __kernel void mixColumn0()//__global uchar* out)
// {
//   uchar state[16];
//   uchar tempState[16];
//   uchar Bx2[16];
//   uchar Bx3[16];
//
//   printf("\nmixColumn0: ");
//
//   for(int i = 0; i < 16; i++)
//   {
//     state[i] = read_channel_intel(chm0);
//     Bx2[i] = state[i] << 1;
//     Bx3[i] = state[i] << 1 ^ state[i];
//
//     if((state[i] & 0x80) != 0)
//     {
//       Bx2[i] ^= 0x1B;
//       Bx3[i] ^= 0x1B;
//     }
//   }
//
//   tempState[0]  =    Bx2[0] ^    Bx3[5] ^ state[10] ^ state[15];
//   tempState[1]  =  state[0] ^    Bx2[5] ^   Bx3[10] ^ state[15];
//   tempState[2]  =  state[0] ^  state[5] ^   Bx2[10] ^   Bx3[15];
//   tempState[3]  =    Bx3[0] ^  state[5] ^ state[10] ^   Bx2[15];
//   tempState[4]  =    Bx2[4] ^    Bx3[9] ^ state[14] ^  state[3];
//   tempState[5]  =  state[4] ^    Bx2[9] ^   Bx3[14] ^  state[3];
//   tempState[6]  =  state[4] ^  state[9] ^   Bx2[14] ^    Bx3[3];
//   tempState[7]  =    Bx3[4] ^  state[9] ^ state[14] ^    Bx2[3];
//   tempState[8]  =    Bx2[8] ^   Bx3[13] ^  state[2] ^  state[7];
//   tempState[9]  =  state[8] ^   Bx2[13] ^    Bx3[2] ^  state[7];
//   tempState[10] =  state[8] ^ state[13] ^    Bx2[2] ^    Bx3[7];
//   tempState[11] =    Bx3[8] ^ state[13] ^  state[2] ^    Bx2[7];
//   tempState[12] =   Bx2[12] ^    Bx3[1] ^  state[6] ^ state[11];
//   tempState[13] = state[12] ^    Bx2[1] ^    Bx3[6] ^ state[11];
//   tempState[14] = state[12] ^  state[1] ^    Bx2[6] ^   Bx3[11];
//   tempState[15] =   Bx3[12] ^  state[1] ^  state[6] ^   Bx2[11];
//
//   for(int i = 0; i < 16; i++)
//   {
//     out[i] = tempState[i];
//     //write_channel_intel(cha1, state[i]);
//     printf("%x ",tempState[i]);
//   }
// }

//figuring this out:
// __kernel void keyExpansion()//uint32_t* subKeys)
// {
//   uchar key[16];
//   key = read_channel_intel(chr0);
//   uint32 subKeys[44];
//   for(int i = 1; i < 11; i++)
//   {
//     uchar keyPosition = i << 2;
//     uint32 subKey = subKeys[keyPosition - 1];
//     uint32 gSubKey = ((sBox[(subKey >> 16)&0xFF] ^ roundCoefficients[i - 1]) << 24) | (sBox[(subKey >> 8)&0xFF] << 16) | (sBox[subKey&0xFF] << 8) | sBox[(subKey >> 24)&0xFF];
//
//     //subKeys[keyPosition]     = subKeys[keyPosition - 4] ^ g(subKeys[keyPosition - 1], roundCoefficients[i - 1]);
//     subKeys[keyPosition]     = subKeys[keyPosition - 4] ^ gSubKey;
//     subKeys[keyPosition + 1] = subKeys[keyPosition]     ^ subKeys[keyPosition - 3];
//     subKeys[keyPosition + 2] = subKeys[keyPosition + 1] ^ subKeys[keyPosition - 2];
//     subKeys[keyPosition + 3] = subKeys[keyPosition + 2] ^ subKeys[keyPosition - 1];
//   }
// }

// __kernel void addRoundKey1(__global uchar* out)
// {
//   uchar data;
//   uchar roundKey[16];
//   roundKey = read_channel_intel(chr1);
//   uchar result;
//   printf("\naddRoundKey0: ");
//   for(int i = 0; i < 16; i++)
//   {
//     //uchar statePosition = i << 2;
//     //state[statePosition]     = state[statePosition]     ^ (*(roundKey + i) >> 24);
//     //state[statePosition + 1] = state[statePosition + 1] ^ ((*(roundKey + i) >> 16)&0xFF);
//     //state[statePosition + 2] = state[statePosition + 2] ^ ((*(roundKey + i) >> 8)&0xFF);
//     //state[statePosition + 3] = state[statePosition + 3] ^ (*(roundKey + i)&0xFF);
//     data = read_channel_intel(cha1);//state[i];
//     key = roundKey[i];
//     result = data ^ key;
//     //write_channel_intel(chb1, result);
//     //printf("data: %x, key: %x, result: %x\n",data,key,result);
//     printf("%x ",result);
//   }
// }