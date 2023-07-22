#pragma once

#define uint_32 unsigned int// 4byte 1word
#define uint_8 unsigned char// 8bit 1byte
#define uint_256 unsigned int*
#define uint_512 unsigned int*



const uint_32 IV[8] =
{
	0x7380166f, 0x4914b2b9, 0x172442d7, 0xda8a0600, 0xa96f30bc, 0x163138aa, 0xe38dee4d, 0xb0fb0e4e
};

uint_32 T(int j);
uint_32 cycle_left_32(uint_32 arg, int num);
uint_32 FF(uint_32 X, uint_32 Y, uint_32 Z, int j);
uint_32 GG(uint_32 X, uint_32 Y, uint_32 Z, int j);
uint_32 P0(uint_32 arg);
uint_32 P1(uint_32 arg);
uint_256 CF(uint_256 V, uint_512 B);
void sm3_do(uint_8* input, uint_256 output, long long size);
uint_32 endian_swap(uint_32 arg);
