#include"sm3.h"
#include<intrin.h>
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

uint_32 rotate_left_32(uint_32 arg, int num)
{
	return (arg << num) | (arg >> (32 - num));
}

uint_32 P0(uint_32 arg)
{
	return arg ^ rotate_left_32(arg, 9) ^ rotate_left_32(arg, 17);
}

uint_32 P1(uint_32 arg)
{
	return arg ^ rotate_left_32(arg, 15) ^ rotate_left_32(arg, 23);
}

uint_32 endian_swap(uint_32 arg)
{
	return ((arg & 0xff000000) >> 24)
		| ((arg & 0x00ff0000) >> 8)
		| ((arg & 0x0000ff00) << 8)
		| ((arg & 0x000000ff) << 24);
}

void CF(uint_256 V, uint_512 B)
{
	uint_32 W_68[68] = { 0 };
	//message extension
	for (int j = 0; j <= 15; j++)
	{
		W_68[j] = endian_swap(B[j]);
	}	
	__m128i tmp1, tmp2, tmp3, tmp4, tmp5;
	__m128i ze = _mm_set_epi32(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
	

	//compression function
	int temp[8];
	for (int j = 0; j <= 7; j++)
	{
		temp[j] = V[j];
	}
	enum NUM {
		V_A, V_B, V_C, V_D, V_E, V_F, V_G, V_H
	};
	uint_32 TT1, TT2;
	__m256i zee = _mm256_set_epi32(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
		0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
	__m256i bbb = _mm256_set_epi32(
		6, 5, 4, 7,
		2, 1, 0, 3);
	for (int j = 0; j <= 11; j++)
	{
		TT2 = ((temp[V_A] << 12) | ((temp[V_A] & 0xFFFFFFFF) >> 20));
		TT1 = TT2 + temp[V_E] + t[j];
		TT1 = ((TT1 << 7) | ((TT1 & 0xFFFFFFFF) >> 25));
		TT2 = TT2 ^ TT1;
		temp[V_D] = temp[V_D] + FF(temp[V_A], temp[V_B], temp[V_C], j) + TT2 + (W_68[j] ^ W_68[j + 4]);
		temp[V_H] = temp[V_H] + GG(temp[V_E], temp[V_F], temp[V_G], j) + TT1 + W_68[j];
		temp[V_B] = ((temp[V_B] << 9) | ((temp[V_B] & 0xFFFFFFFF) >> 23));
		temp[V_F] = ((temp[V_F] << 19) | ((temp[V_F] & 0xFFFFFFFF) >> 13));
		temp[V_H] = (temp[V_H]
			^ ((temp[V_H] << 9) | ((temp[V_H] & 0xFFFFFFFF) >> 23))
			^ ((temp[V_H] << 17) | ((temp[V_H] & 0xFFFFFFFF) >> 15)));
		__m256i aaa = _mm256_set_epi32(
			temp[V_H], temp[V_G], temp[V_F], temp[V_E],
			temp[V_D], temp[V_C], temp[V_B], temp[V_A]);
		__m256i ccc = _mm256_permutevar8x32_epi32(aaa, bbb);
		_mm256_maskstore_epi32((temp), zee, ccc);
	}

	for (int j = 12; j <= 63; j++)
	{
		if (j == 63)
		{
			W_68[67] = P1(W_68[51] ^ W_68[58] ^ rotate_left_32(W_68[64], 15))
				^ rotate_left_32(W_68[54], 7)
				^ W_68[61];
		}
		else if (j % 3 == 0) {
			int i = j + 4;
			tmp1 = _mm_setr_epi32(W_68[i - 16], W_68[i - 15], W_68[i - 14], 0x00000000);//1	
			tmp2 = _mm_setr_epi32(W_68[i - 9], W_68[i - 8], W_68[i - 7], 0x00000000);//2
			tmp3 = _mm_setr_epi32(W_68[i - 3], W_68[i - 2], W_68[i - 1], 0x00000000);//3
			tmp4 = _mm_setr_epi32(W_68[i - 13], W_68[i - 12], W_68[i - 11], 0x00000000);//4
			tmp5 = _mm_setr_epi32(W_68[i - 6], W_68[i - 5], W_68[i - 4], 0x00000000);//5

			tmp1 = _mm_xor_si128(tmp1, tmp2);//1+2
			tmp3 = _mm_or_si128(_mm_slli_epi32(tmp3, 15), _mm_srli_epi32(tmp3, 17));//3 <<<
			tmp1 = _mm_xor_si128(tmp1, tmp3);
			tmp1 = _mm_xor_si128
			(tmp1,
				_mm_xor_si128(_mm_or_si128(_mm_slli_epi32(tmp1, 15), _mm_srli_epi32(tmp1, 17)),
					_mm_or_si128(_mm_slli_epi32(tmp1, 23), _mm_srli_epi32(tmp1, 9)))
			);
			tmp4 = _mm_xor_si128(_mm_or_si128(_mm_slli_epi32(tmp4, 7), _mm_srli_epi32(tmp4, 25)), tmp5);
			tmp1 = _mm_xor_si128(tmp1, tmp4);
			_mm_maskstore_epi32((int*)(W_68 + i), ze, tmp1);
		}
		TT2 = ((temp[V_A] << 12) | ((temp[V_A] & 0xFFFFFFFF) >> 20));
		TT1 = TT2 + temp[V_E] + t[j];
		TT1 = ((TT1 << 7) | ((TT1 & 0xFFFFFFFF) >> 25));
		TT2 = TT2 ^ TT1;
		temp[V_D] = temp[V_D] + FF(temp[V_A], temp[V_B], temp[V_C], j) + TT2 + (W_68[j] ^ W_68[j + 4]);
		temp[V_H] = temp[V_H] + GG(temp[V_E], temp[V_F], temp[V_G], j) + TT1 + W_68[j];
		temp[V_B] = ((temp[V_B] << 9) | ((temp[V_B] & 0xFFFFFFFF) >> 23));
		temp[V_F] = ((temp[V_F] << 19) | ((temp[V_F] & 0xFFFFFFFF) >> 13));
		temp[V_H] = (temp[V_H]
			^ ((temp[V_H] << 9) | ((temp[V_H] & 0xFFFFFFFF) >> 23))
			^ ((temp[V_H] << 17) | ((temp[V_H] & 0xFFFFFFFF) >> 15)));
		__m256i aaa = _mm256_set_epi32(
			temp[V_H], temp[V_G], temp[V_F], temp[V_E],
			temp[V_D], temp[V_C], temp[V_B], temp[V_A]);
		__m256i ccc = _mm256_permutevar8x32_epi32(aaa, bbb);
		_mm256_maskstore_epi32((temp), zee, ccc);
	}


	for (int j = 0; j <= 7; j++)
	{
		V[j] = temp[j] ^ V[j];
	}
}

void sm3_do(uint_8* input, uint_256 output, long long size)
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
	for (int i = 0; i < 8; i++)
	{
		output[i] = IV[i];
	}
	long long num = ((size + k + 65) >> 9);

	for (long long i = 0; i < num; i++)
	{
		CF(output, (uint_512)(temp + (i * 64)));
	}
	delete temp;
}