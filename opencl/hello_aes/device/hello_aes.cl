//AES

//#include <stdio.h>
//#include <stdint.h>
#include "aes.h"
//#include "galois.h"
//#include <opencl.h>

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

// Channels between KeyExpansion and AddRoundKey kernels
channel uchar ARK0toKE;
channel uchar KEtoARK1;
channel uchar KEtoARK2;
channel uchar KEtoARK3;
channel uchar KEtoARK4;
channel uchar KEtoARK5;
channel uchar KEtoARK6;
channel uchar KEtoARK7;
channel uchar KEtoARK8;
channel uchar KEtoARK9;
channel uchar KEtoARK10;

// Channels between AddRoundKey, ByteSubstitution, MixColumns, and ShiftRows kernels
channel uchar ARK0toBS0;
channel uchar BS0toMC0;
channel uchar MC0toARK1;
channel uchar ARK1toBS1;
channel uchar BS1toMC1;
channel uchar MC1toARK2;
channel uchar ARK2toBS2;
channel uchar BS2toMC2;
channel uchar MC2toARK3;
channel uchar ARK3toBS3;
channel uchar BS3toMC3;
channel uchar MC3toARK4;
channel uchar ARK4toBS4;
channel uchar BS4toMC4;
channel uchar MC4toARK5;
channel uchar ARK5toBS5;
channel uchar BS5toMC5;
channel uchar MC5toARK6;
channel uchar ARK6toBS6;
channel uchar BS6toMC6;
channel uchar MC6toARK7;
channel uchar ARK7toBS7;
channel uchar BS7toMC7;
channel uchar MC7toARK8;
channel uchar ARK8toBS8;
channel uchar BS8toMC8;
channel uchar MC8toARK9;
channel uchar ARK9toBS9;
channel uchar BS9toSR;
channel uchar SRtoARK10;


__kernel void addRoundKey0(__global uchar* state, __global uchar* roundKey)
{
  uchar data;
  uchar key;
  uchar result;
  uchar keys[16] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
  //DEBUG printf("\naddRoundKey0: ");
  for(int i = 0; i < 16; i++)
  {
    data = state[i];
    //key = roundKey[i];
    key = keys[i];
    result = data ^ key;

    write_channel_intel(ARK0toBS0, result);
    write_channel_intel(ARK0toKE, key);
    //printf("data: %x, key: %x, result: %x\n",data,key,result);
    //DEBUG printf("%x ",result);
  }
}

__kernel void keyExpansion()
{
  uchar roundCoefficients[10] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36};

  uchar subKeys[176];
  for(int j =0; j<16; j++)
  {
    subKeys[j] = read_channel_intel(ARK0toKE);
  }

  for(int i = 1; i < 11; i++)
  {
    uchar keyPosition = i << 4;

    subKeys[keyPosition]      = subKeys[keyPosition - 16] ^ (sBox[subKeys[keyPosition - 3]] ^ roundCoefficients[i - 1]);
    subKeys[keyPosition + 1]  = subKeys[keyPosition - 15] ^ (sBox[subKeys[keyPosition - 2]]);
    subKeys[keyPosition + 2]  = subKeys[keyPosition - 14] ^ (sBox[subKeys[keyPosition - 1]]);
    subKeys[keyPosition + 3]  = subKeys[keyPosition - 13] ^ (sBox[subKeys[keyPosition - 4]]);
    subKeys[keyPosition + 4]  = subKeys[keyPosition - 12] ^ subKeys[keyPosition];
    subKeys[keyPosition + 5]  = subKeys[keyPosition - 11] ^ subKeys[keyPosition + 1];
    subKeys[keyPosition + 6]  = subKeys[keyPosition - 10] ^ subKeys[keyPosition + 2];
    subKeys[keyPosition + 7]  = subKeys[keyPosition - 9]  ^ subKeys[keyPosition + 3];
    subKeys[keyPosition + 8]  = subKeys[keyPosition - 8] ^ subKeys[keyPosition + 4];
    subKeys[keyPosition + 9]  = subKeys[keyPosition - 7] ^ subKeys[keyPosition + 5];
    subKeys[keyPosition + 10] = subKeys[keyPosition - 6] ^ subKeys[keyPosition + 6];
    subKeys[keyPosition + 11] = subKeys[keyPosition - 5] ^ subKeys[keyPosition + 7];
    subKeys[keyPosition + 12] = subKeys[keyPosition - 4] ^ subKeys[keyPosition + 8];
    subKeys[keyPosition + 13] = subKeys[keyPosition - 3] ^ subKeys[keyPosition + 9];
    subKeys[keyPosition + 14] = subKeys[keyPosition - 2] ^ subKeys[keyPosition + 10];
    subKeys[keyPosition + 15] = subKeys[keyPosition - 1] ^ subKeys[keyPosition + 11];
  }

  // Write round Keys to corresponding AddRoundKey kernels
  for(int k = 0; k<16; k++) 
  {
    write_channel_intel(KEtoARK1, subKeys[16 + k]);   // 1*16=16
    write_channel_intel(KEtoARK2, subKeys[32 + k]);   // 2*16=32
    write_channel_intel(KEtoARK3, subKeys[48 + k]);   // 3*16=48
    write_channel_intel(KEtoARK4, subKeys[64 + k]);   // 4*16=64
    write_channel_intel(KEtoARK5, subKeys[80 + k]);   // 5*16=80
    write_channel_intel(KEtoARK6, subKeys[96 + k]);   // 6*16=96
    write_channel_intel(KEtoARK7, subKeys[112 + k]);  // 7*16=112
    write_channel_intel(KEtoARK8, subKeys[128 + k]);  // 8*16=128
    write_channel_intel(KEtoARK9, subKeys[144 + k]);  // 9*16=144
    write_channel_intel(KEtoARK10, subKeys[160 + k]); //10*16=160
  }
}

__kernel void byteSubstitution0()
{
  uchar state;
  //DEBUG printf("\nbyteSubstition0: ");
  for(int i = 0; i < 16; i++)
  {
    state = read_channel_intel(ARK0toBS0);
    state = sBox[state];
    //DEBUG printf("%x ",state);
    write_channel_intel(BS0toMC0, state);
  }
}

__kernel void mixColumn0()
{
  uchar state[16];
  uchar tempState[16];
  uchar Bx2[16];
  uchar Bx3[16];

  //DEBUG printf("\nmixColumn0: ");

  for(int i = 0; i < 16; i++)
  {
    state[i] = read_channel_intel(BS0toMC0);
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
  tempState[12] =   Bx2[12] ^    Bx3[1] ^  state[6] ^ state[11];
  tempState[13] = state[12] ^    Bx2[1] ^    Bx3[6] ^ state[11];
  tempState[14] = state[12] ^  state[1] ^    Bx2[6] ^   Bx3[11];
  tempState[15] =   Bx3[12] ^  state[1] ^  state[6] ^   Bx2[11];

  for(int i = 0; i < 16; i++)
  {
    write_channel_intel(MC0toARK1, tempState[i]);
    //DEBUG printf("%x ",tempState[i]);
  }
}

__kernel void addRoundKey1()
{
  uchar data;
  uchar key;
  uchar result;
  //DEBUG printf("\naddRoundKey1: ");
  for(int i = 0; i < 16; i++)
  {
    data = read_channel_intel(MC0toARK1);//state[i];
    key = read_channel_intel(KEtoARK1);//roundKey[i];
    result = data ^ key;
    write_channel_intel(ARK1toBS1, result);
    //printf("\n\tdata: %x, key: %x, result: %x",data,key,result);
    //DEBUG printf("%x ",result);
  }
}


__kernel void byteSubstitution1()
{
  uchar state;
  //DEBUG printf("\nbyteSubstition1: ");
  for(int i = 0; i < 16; i++)
  {
    state = read_channel_intel(ARK1toBS1);
    state = sBox[state];
    //DEBUG printf("%x ",state);
    write_channel_intel(BS1toMC1, state);
  }
}

__kernel void mixColumn1()
{
  uchar state[16];
  uchar tempState[16];
  uchar Bx2[16];
  uchar Bx3[16];

  //DEBUG printf("\nmixColumn1: ");

  for(int i = 0; i < 16; i++)
  {
    state[i] = read_channel_intel(BS1toMC1);
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
  tempState[12] =   Bx2[12] ^    Bx3[1] ^  state[6] ^ state[11];
  tempState[13] = state[12] ^    Bx2[1] ^    Bx3[6] ^ state[11];
  tempState[14] = state[12] ^  state[1] ^    Bx2[6] ^   Bx3[11];
  tempState[15] =   Bx3[12] ^  state[1] ^  state[6] ^   Bx2[11];

  for(int i = 0; i < 16; i++)
  {
    write_channel_intel(MC1toARK2, tempState[i]);
    //DEBUG printf("%x ",tempState[i]);
  }
}

__kernel void addRoundKey2()
{
  uchar data;
  uchar key;
  uchar result;
  //DEBUG printf("\naddRoundKey2: ");
  for(int i = 0; i < 16; i++)
  {
    data = read_channel_intel(MC1toARK2);//state[i];
    key = read_channel_intel(KEtoARK2);//roundKey[i];
    result = data ^ key;
    write_channel_intel(ARK2toBS2, result);
    //printf("\n\tdata: %x, key: %x, result: %x",data,key,result);
    //DEBUG printf("%x ",result);
  }
}


__kernel void byteSubstitution2()
{
  uchar state;
  //DEBUG printf("\nbyteSubstition2: ");
  for(int i = 0; i < 16; i++)
  {
    state = read_channel_intel(ARK2toBS2);
    state = sBox[state];
    //DEBUG printf("%x ",state);
    write_channel_intel(BS2toMC2, state);
  }
}

__kernel void mixColumn2()
{
  uchar state[16];
  uchar tempState[16];
  uchar Bx2[16];
  uchar Bx3[16];

  //DEBUG printf("\nmixColumn2: ");

  for(int i = 0; i < 16; i++)
  {
    state[i] = read_channel_intel(BS2toMC2);
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
  tempState[12] =   Bx2[12] ^    Bx3[1] ^  state[6] ^ state[11];
  tempState[13] = state[12] ^    Bx2[1] ^    Bx3[6] ^ state[11];
  tempState[14] = state[12] ^  state[1] ^    Bx2[6] ^   Bx3[11];
  tempState[15] =   Bx3[12] ^  state[1] ^  state[6] ^   Bx2[11];

  for(int i = 0; i < 16; i++)
  {
    write_channel_intel(MC2toARK3, tempState[i]);
    //DEBUG printf("%x ",tempState[i]);
  }
}

__kernel void addRoundKey3()
{
  uchar data;
  uchar key;
  uchar result;
  //DEBUG printf("\naddRoundKey3: ");
  for(int i = 0; i < 16; i++)
  {
    data = read_channel_intel(MC2toARK3);//state[i];
    key = read_channel_intel(KEtoARK3);//roundKey[i];
    result = data ^ key;
    write_channel_intel(ARK3toBS3, result);
    //printf("\n\tdata: %x, key: %x, result: %x",data,key,result);
    //DEBUG printf("%x ",result);
  }
}


__kernel void byteSubstitution3()
{
  uchar state;
  //DEBUG printf("\nbyteSubstition3: ");
  for(int i = 0; i < 16; i++)
  {
    state = read_channel_intel(ARK3toBS3);
    state = sBox[state];
    //DEBUG printf("%x ",state);
    write_channel_intel(BS3toMC3, state);
  }
}

__kernel void mixColumn3()
{
  uchar state[16];
  uchar tempState[16];
  uchar Bx2[16];
  uchar Bx3[16];

  //DEBUG printf("\nmixColumn3: ");

  for(int i = 0; i < 16; i++)
  {
    state[i] = read_channel_intel(BS3toMC3);
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
  tempState[12] =   Bx2[12] ^    Bx3[1] ^  state[6] ^ state[11];
  tempState[13] = state[12] ^    Bx2[1] ^    Bx3[6] ^ state[11];
  tempState[14] = state[12] ^  state[1] ^    Bx2[6] ^   Bx3[11];
  tempState[15] =   Bx3[12] ^  state[1] ^  state[6] ^   Bx2[11];

  for(int i = 0; i < 16; i++)
  {
    write_channel_intel(MC3toARK4, tempState[i]);
    //DEBUG printf("%x ",tempState[i]);
  }
}

__kernel void addRoundKey4()
{
  uchar data;
  uchar key;
  uchar result;
  //DEBUG printf("\naddRoundKey4: ");
  for(int i = 0; i < 16; i++)
  {
    data = read_channel_intel(MC3toARK4);//state[i];
    key = read_channel_intel(KEtoARK4);//roundKey[i];
    result = data ^ key;
    write_channel_intel(ARK4toBS4, result);
    //printf("\n\tdata: %x, key: %x, result: %x",data,key,result);
    //DEBUG printf("%x ",result);
  }
}


__kernel void byteSubstitution4()
{
  uchar state;
  //DEBUG printf("\nbyteSubstition4: ");
  for(int i = 0; i < 16; i++)
  {
    state = read_channel_intel(ARK4toBS4);
    state = sBox[state];
    //DEBUG printf("%x ",state);
    write_channel_intel(BS4toMC4, state);
  }
}

__kernel void mixColumn4()
{
  uchar state[16];
  uchar tempState[16];
  uchar Bx2[16];
  uchar Bx3[16];

  //DEBUG printf("\nmixColumn4: ");

  for(int i = 0; i < 16; i++)
  {
    state[i] = read_channel_intel(BS4toMC4);
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
  tempState[12] =   Bx2[12] ^    Bx3[1] ^  state[6] ^ state[11];
  tempState[13] = state[12] ^    Bx2[1] ^    Bx3[6] ^ state[11];
  tempState[14] = state[12] ^  state[1] ^    Bx2[6] ^   Bx3[11];
  tempState[15] =   Bx3[12] ^  state[1] ^  state[6] ^   Bx2[11];

  for(int i = 0; i < 16; i++)
  {
    write_channel_intel(MC4toARK5, tempState[i]);
    //DEBUG printf("%x ",tempState[i]);
  }
}

__kernel void addRoundKey5()
{
  uchar data;
  uchar key;
  uchar result;
  //DEBUG printf("\naddRoundKey5: ");
  for(int i = 0; i < 16; i++)
  {
    data = read_channel_intel(MC4toARK5);//state[i];
    key = read_channel_intel(KEtoARK5);//roundKey[i];
    result = data ^ key;
    write_channel_intel(ARK5toBS5, result);
    //printf("\n\tdata: %x, key: %x, result: %x",data,key,result);
    //DEBUG printf("%x ",result);
  }
}


__kernel void byteSubstitution5()
{
  uchar state;
  //DEBUG printf("\nbyteSubstition5: ");
  for(int i = 0; i < 16; i++)
  {
    state = read_channel_intel(ARK5toBS5);
    state = sBox[state];
    //DEBUG printf("%x ",state);
    write_channel_intel(BS5toMC5, state);
  }
}

__kernel void mixColumn5()
{
  uchar state[16];
  uchar tempState[16];
  uchar Bx2[16];
  uchar Bx3[16];

  //DEBUG printf("\nmixColumn5: ");

  for(int i = 0; i < 16; i++)
  {
    state[i] = read_channel_intel(BS5toMC5);
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
  tempState[12] =   Bx2[12] ^    Bx3[1] ^  state[6] ^ state[11];
  tempState[13] = state[12] ^    Bx2[1] ^    Bx3[6] ^ state[11];
  tempState[14] = state[12] ^  state[1] ^    Bx2[6] ^   Bx3[11];
  tempState[15] =   Bx3[12] ^  state[1] ^  state[6] ^   Bx2[11];

  for(int i = 0; i < 16; i++)
  {
    write_channel_intel(MC5toARK6, tempState[i]);
    //DEBUG printf("%x ",tempState[i]);
  }
}

__kernel void addRoundKey6()
{
  uchar data;
  uchar key;
  uchar result;
  //DEBUG printf("\naddRoundKey6: ");
  for(int i = 0; i < 16; i++)
  {
    data = read_channel_intel(MC5toARK6);//state[i];
    key = read_channel_intel(KEtoARK6);//roundKey[i];
    result = data ^ key;
    write_channel_intel(ARK6toBS6, result);
    //printf("\n\tdata: %x, key: %x, result: %x",data,key,result);
    //DEBUG printf("%x ",result);
  }
}


__kernel void byteSubstitution6()
{
  uchar state;
  //DEBUG printf("\nbyteSubstition6: ");
  for(int i = 0; i < 16; i++)
  {
    state = read_channel_intel(ARK6toBS6);
    state = sBox[state];
    //DEBUG printf("%x ",state);
    write_channel_intel(BS6toMC6, state);
  }
}

__kernel void mixColumn6()
{
  uchar state[16];
  uchar tempState[16];
  uchar Bx2[16];
  uchar Bx3[16];

  //DEBUG printf("\nmixColumn6: ");

  for(int i = 0; i < 16; i++)
  {
    state[i] = read_channel_intel(BS6toMC6);
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
  tempState[12] =   Bx2[12] ^    Bx3[1] ^  state[6] ^ state[11];
  tempState[13] = state[12] ^    Bx2[1] ^    Bx3[6] ^ state[11];
  tempState[14] = state[12] ^  state[1] ^    Bx2[6] ^   Bx3[11];
  tempState[15] =   Bx3[12] ^  state[1] ^  state[6] ^   Bx2[11];

  for(int i = 0; i < 16; i++)
  {
    write_channel_intel(MC6toARK7, tempState[i]);
    //DEBUG printf("%x ",tempState[i]);
  }
}

__kernel void addRoundKey7()
{
  uchar data;
  uchar key;
  uchar result;
  //DEBUG printf("\naddRoundKey7: ");
  for(int i = 0; i < 16; i++)
  {
    data = read_channel_intel(MC6toARK7);//state[i];
    key = read_channel_intel(KEtoARK7);//roundKey[i];
    result = data ^ key;
    write_channel_intel(ARK7toBS7, result);
    //printf("\n\tdata: %x, key: %x, result: %x",data,key,result);
    //DEBUG printf("%x ",result);
  }
}


__kernel void byteSubstitution7()
{
  uchar state;
  //DEBUG printf("\nbyteSubstition7: ");
  for(int i = 0; i < 16; i++)
  {
    state = read_channel_intel(ARK7toBS7);
    state = sBox[state];
    //DEBUG printf("%x ",state);
    write_channel_intel(BS7toMC7, state);
  }
}

__kernel void mixColumn7()
{
  uchar state[16];
  uchar tempState[16];
  uchar Bx2[16];
  uchar Bx3[16];

  //DEBUG printf("\nmixColumn7: ");

  for(int i = 0; i < 16; i++)
  {
    state[i] = read_channel_intel(BS7toMC7);
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
  tempState[12] =   Bx2[12] ^    Bx3[1] ^  state[6] ^ state[11];
  tempState[13] = state[12] ^    Bx2[1] ^    Bx3[6] ^ state[11];
  tempState[14] = state[12] ^  state[1] ^    Bx2[6] ^   Bx3[11];
  tempState[15] =   Bx3[12] ^  state[1] ^  state[6] ^   Bx2[11];

  for(int i = 0; i < 16; i++)
  {
    write_channel_intel(MC7toARK8, tempState[i]);
    //DEBUG printf("%x ",tempState[i]);
  }
}

__kernel void addRoundKey8()
{
  uchar data;
  uchar key;
  uchar result;
  //DEBUG printf("\naddRoundKey8: ");
  for(int i = 0; i < 16; i++)
  {
    data = read_channel_intel(MC7toARK8);//state[i];
    key = read_channel_intel(KEtoARK8);//roundKey[i];
    result = data ^ key;
    write_channel_intel(ARK8toBS8, result);
    //printf("\n\tdata: %x, key: %x, result: %x",data,key,result);
    //DEBUG printf("%x ",result);
  }
}


__kernel void byteSubstitution8()
{
  uchar state;
  //DEBUG printf("\nbyteSubstition8: ");
  for(int i = 0; i < 16; i++)
  {
    state = read_channel_intel(ARK8toBS8);
    state = sBox[state];
    //DEBUG printf("%x ",state);
    write_channel_intel(BS8toMC8, state);
  }
}

__kernel void mixColumn8()
{
  uchar state[16];
  uchar tempState[16];
  uchar Bx2[16];
  uchar Bx3[16];

  //DEBUG printf("\nmixColumn8: ");

  for(int i = 0; i < 16; i++)
  {
    state[i] = read_channel_intel(BS8toMC8);
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
  tempState[12] =   Bx2[12] ^    Bx3[1] ^  state[6] ^ state[11];
  tempState[13] = state[12] ^    Bx2[1] ^    Bx3[6] ^ state[11];
  tempState[14] = state[12] ^  state[1] ^    Bx2[6] ^   Bx3[11];
  tempState[15] =   Bx3[12] ^  state[1] ^  state[6] ^   Bx2[11];

  for(int i = 0; i < 16; i++)
  {
    write_channel_intel(MC8toARK9, tempState[i]);
    //DEBUG printf("%x ",tempState[i]);
  }
}

__kernel void addRoundKey9()
{
  uchar data;
  uchar key;
  uchar result;
  //DEBUG printf("\naddRoundKey9: ");
  for(int i = 0; i < 16; i++)
  {
    data = read_channel_intel(MC8toARK9);//state[i];
    key = read_channel_intel(KEtoARK9);//roundKey[i];
    result = data ^ key;
    write_channel_intel(ARK9toBS9, result);
    //printf("\n\tdata: %x, key: %x, result: %x",data,key,result);
    //DEBUG printf("%x ",result);
  }
}

__kernel void byteSubstitution9()
{
  uchar state;
  //DEBUG printf("\nbyteSubstition9: ");
  for(int i = 0; i < 16; i++)
  {
    state = read_channel_intel(ARK9toBS9);
    state = sBox[state];
    //DEBUG printf("%x ",state);
    write_channel_intel(BS9toSR, state);
  }
}

__kernel void shiftRows()
{
  uchar state[16];
  for(int r = 0; r < 16; r++)
  {
    state[r] = read_channel_intel(BS9toSR);
  }
  uchar tempState[16];

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

  for(int i = 0; i < 16; i++)
  {
    write_channel_intel(SRtoARK10, tempState[i]);
  }
}


// Last kernel in data pipe; write to output buffer 
__kernel void addRoundKey10(__global uchar* out)
{
  uchar data;
  uchar key;
  uchar result;
  //DEBUG printf("\naddRoundKey10: ");
  for(int i = 0; i < 16; i++)
  {
    data = read_channel_intel(SRtoARK10);
    key = read_channel_intel(KEtoARK10);
    result = data ^ key;
    out[i] = result;
    //DEBUG printf("%x ",result);
  }
}

