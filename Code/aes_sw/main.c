#include <stdio.h>
#include <stdint.h>
#include "aes.h"
#include "galois.h"

int main(int argc, char *argv[])
{
  int i = 0;
  FILE *fp;
  FILE *fp2;
  int c;
  char input[16];
  uint32_t key[4] = {0x00010203, 0x04050607, 0x08090A0B, 0x0C0D0E0F};

  fp2 = fopen("text.txt","w");

  for (i = 1; i < argc; i++) {
    fp = fopen(argv[i], "r");

    if (fp == NULL)
    {
        fprintf(stderr, "cat: can't open %s\n", argv[i]);
        continue;
    }

  i = 0;
  do {
      input[i] = fgetc(fp);
      if( feof(fp) )
      {
        for(int j = i; j < 16; j++)
        {
          input[j] = 0;
        }
        //encryptBlock((uint8_t*)input, key);
        decryptBlock((uint8_t*)input, key);
        fputc(0, fp2);
        break ;
      }
      i = (i + 1)&0xF;
      if (i == 0)
      {
        //encryptBlock((uint8_t*)input, key);
        decryptBlock((uint8_t*)input, key);
        for(int k = 0; k < 16; k++)
        {
          fputc(input[k], fp2);
        }
      };
   } while(1);

    fclose(fp);
    fclose(fp2);
  }

  return 0;
}
