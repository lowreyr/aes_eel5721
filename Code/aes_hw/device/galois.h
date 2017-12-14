#ifndef GALOIS_H
#define GALOIS_H

//#include <stdint.h>
typedef uchar uint8_t;

//Table of multiplicative inverses: x*x^(-1) = 1
uchar multiplicativeInverse[256];

//Table of exponentials: E(x) = 3^x
uchar exponential[256];

//Table of logarithms: x = 3^L(x)
uchar logarithm[256];

//Galois Field GF(2^8) multiplication
uchar galois_multiply(uchar, uchar);
//uint8_t multiply(uint8_t a, uint8_t b);

#endif
