#pragma once
#include<stdlib.h>
#include<stdio.h>
#include<iostream>
#include<chrono>
#define uint_32 unsigned int// 4byte 1word
#define uint_8 unsigned char// 8bit 1byte
#define uint_256 unsigned int*
#define uint_512 unsigned int*



const uint_32 IV_8[8] = { 0x6a09e667, 0xbb67ae85,0x3c6ef372,0xa54ff53a,0x510e527f,0x9b05688c, 0x1f83d9ab,0x5be0cd19 };
const uint_32 IV_64[64] =
{
0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
   0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
   0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
   0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
   0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
   0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
   0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
   0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2

};

uint_32 Ch(uint_32 x, uint_32 y, uint_32 z);
uint_32 Ma(uint_32 x, uint_32 y, uint_32 z);
uint_32 sigma_uppercase_0(uint_32 arg);
uint_32 sigma_uppercase_1(uint_32 arg);
uint_32 sigma_lowercase_0(uint_32 arg);
uint_32 sigma_lowercase_1(uint_32 arg);
void CF(uint_256 V, uint_512 B);
void sha256_do(uint_8* input, uint_256 output, long long size);
uint_32 endian_swap(uint_32 arg);
