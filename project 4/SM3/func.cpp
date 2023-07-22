#include"sm3.h"
#include<stdlib.h>
#include<stdio.h>

uint_32 FF(uint_32 X, uint_32 Y, uint_32 Z, int j)
{
	if (0 <= j && j <= 15)
	{
		return X ^ Y ^ Z;
	}
	else if (16 <= j && j <= 63)
	{
		return (X & Y) | (X & Z) | (Y & Z);
	}
	else
	{
		printf("parameter error");
		exit('0');
	}
}

uint_32 GG(uint_32 X, uint_32 Y, uint_32 Z, int j)
{
	if (0 <= j && j <= 15)
	{
		return X ^ Y ^ Z;
	}
	else if (16 <= j && j <= 63)
	{
		return (X & Y) | ((~X) & Z);
	}
	else
	{
		printf("parameter error");
		exit('0');
	}
}

uint_32 cycle_left_32(uint_32 arg, int num)
{
	return (arg << num) | (arg >> (32 - num));
}

uint_32 P0(uint_32 arg)
{
	return arg ^ cycle_left_32(arg, 9) ^ cycle_left_32(arg, 17);
}

uint_32 P1(uint_32 arg)
{
	return arg ^ cycle_left_32(arg, 15) ^ cycle_left_32(arg, 23);
}

uint_32 T(int j)
{
	return ((j <= 15) && (j >= 0)) ? 0x79cc4519 : 0x7a879d8a;
}

uint_32 endian_swap(uint_32 arg)
{
	return ((arg & 0xff000000) >> 24)
		| ((arg & 0x00ff0000) >> 8)
		| ((arg & 0x0000ff00) << 8)
		| ((arg & 0x000000ff) << 24);
}
uint_256 CF(uint_256 V, uint_512 B)
{

	uint_32 W_68[68];
	uint_32 W_64[64];
	//message extension
	for (int j = 0; j <= 15; j++)
	{
			W_68[j] = endian_swap(B[j]);
	}
	for (int j = 16; j <= 67; j++)
	{
		W_68[j] = P1(W_68[j - 16] ^ W_68[j - 9] ^ cycle_left_32(W_68[j - 3], 15))
			^ cycle_left_32(W_68[j - 13], 7)
			^ W_68[j - 6];
	}
	for (int j = 0; j <= 63; j++)
	{
		W_64[j] = W_68[j] ^ W_68[j + 4];
	}
	//compression function
	uint_256 temp = (uint_256)malloc(32 * sizeof(uint_8));
	//uint_256 ret = (uint_256)malloc(32 * sizeof(uint_8));
	for (int j = 0; j <= 7; j++)
	{
		temp[j] = V[j];
	}
	enum NUM {
		V_A,V_B,V_C,V_D,V_E,V_F,V_G,V_H
	};
	uint_32 SS1, SS2, TT1, TT2;
	for (int j = 0; j <= 63; j++)
	{
		SS1 = cycle_left_32(cycle_left_32(temp[V_A], 12) + temp[V_E] + cycle_left_32(T(j), j % 32), 7);
		SS2 = SS1 ^ cycle_left_32(temp[V_A], 12);
		TT1 = FF(temp[V_A], temp[V_B], temp[V_C], j) + temp[V_D] + SS2 + W_64[j];
		TT2 = GG(temp[V_E], temp[V_F], temp[V_G], j) + temp[V_H] + SS1 + W_68[j];
		temp[V_D] = temp[V_C];
		temp[V_C] = cycle_left_32(temp[V_B], 9);
		temp[V_B] = temp[V_A];
		temp[V_A] = TT1;
		temp[V_H] = temp[V_G];
		temp[V_G] = cycle_left_32(temp[V_F], 19);
		temp[V_F] = temp[V_E];
		temp[V_E] = P0(TT2);
	}
	for (int j = 0; j <= 7; j++)
	{
		V[j] = temp[j] ^ V[j];
	}
	delete temp;
		return V;
}

void sm3_do(uint_8* input, uint_256 output,long long size)
{
	//padding
	int k = ((448 - (size + 1)) % 512) >= 0 ?
		((448 - (size + 1)) % 512) : ((448 - (size + 1)) % 512) + 512;
	uint_8* temp = (uint_8*)calloc((int)((size + k + 1 + 64) >> 3), sizeof(uint_8));
	for (long long i = 0; i < (size >> 3); i++)
	{
		temp[i] = input[i];
	}
	temp[size >> 3] = 0x80;
	temp[((size + k + 1 + 64) >> 3) - 1] = size;

	//iteration
	uint_256 V = (uint_256)malloc(32 * sizeof(uint_8));
	for (int i = 0; i < 8; i++)
	{
		V[i] = IV[i];
	}
	long long num = ((size + k + 65) >> 9);
	
	for (long long i = 0; i < num; i++)
	{
		V = CF(V, (uint_512)(temp+(i*64)));
	}
	for (int i = 0; i < 8; i++)
	{
		output[i] = V[i];
	}
	delete temp;
	delete V;
}