#include <stdio.h>
#include <stdint.h>
#include "aes.h"
#include "galois.h"

int main(int argc, char *argv[])
{
  // int i;
  // FILE *fp;
  // uint8_t str[16];
  //
  // for (i = 1; i < argc; i++) {
  //   fp = fopen(argv[i], "r");
  //
  //   if (fp == NULL)
  //   {
  //       fprintf(stderr, "cat: can't open %s\n", argv[i]);
  //       continue;
  //   }
  //
  //   // while (fgets(str,16,fp) != EOF) {
  //   //     puts(str);
  //   // }
  //   if( fgets (str, 17, fp)!=NULL )
  //   {
  //     /* writing content to stdout */
  //     puts(str);
  //     printf("%s,\n",str);
  //   }
  //   fclose(fp);
  // }

  for(uint16_t i = 0; i <= 255; i++)
  {
    for(uint16_t j = 0; j <= 255; j++)
    {
      uint8_t a = multiply(i,j);
      uint8_t b = multiply_1(i,j);
      if(i < 3)
      {
        printf("Table: %X\nAlgor: %X\n",a ,b);
      }
      if(a != b)
      {
        printf("Error: i = %d, j = %d\n",i ,j);
      }

    }
  }

  return 0;
}

// int main()
// {
//   uint8_t input[16] = {0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF};
//   uint32_t key[4] = {0x00010203, 0x04050607, 0x08090A0B, 0x0C0D0E0F};
//
//   for(uint8_t i = 0; i < 4; i++)
//   {
//     subKeys[i] = key[i];
//   }
//
//   encryptBlock(input, subKeys);
//
//   return 0;
// }
