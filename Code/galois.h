#ifndef GALOIS_H
#define GALOIS_H

#include <stdint.h>

//Table of multiplicative inverses: x*x^(-1) = 1
uint8_t multiplicativeInverse[256];

//Table of exponentials: E(x) = 3^x
uint8_t exponential[256];

//Table of logarithms: x = 3^L(x)
uint8_t logarithm[256];

//Galois Field GF(2^8) multiplication
uint8_t galois_multiply(uint8_t, uint8_t);
//uint8_t multiply(uint8_t a, uint8_t b);

#endif
