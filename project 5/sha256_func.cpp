#include"sha256.h"


uint_32 Ch(uint_32 x, uint_32 y, uint_32 z)
{
	return ((x & y) ^ ((~x) & z));
}

uint_32 Ma(uint_32 x, uint_32 y, uint_32 z)
{
	return	((x & y) ^ (x & z) ^ (y & z));
}
uint_32 sigma_uppercase_0(uint_32 arg)
{
	return _rotr(arg, 2) ^ _rotr(arg, 13) ^ _rotr(arg, 22);
}
uint_32 sigma_uppercase_1(uint_32 arg)
{
	return _rotr(arg, 6) ^ _rotr(arg, 11) ^ _rotr(arg, 25);
}
uint_32 sigma_lowercase_0(uint_32 arg)
{
	return _rotr(arg, 7) ^ _rotr(arg, 18) ^ (arg >> 3);
}
uint_32 sigma_lowercase_1(uint_32 arg)
{
	return _rotr(arg, 17) ^ _rotr(arg, 19) ^ (arg >> 10);
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
	uint_32 W[64];
	//message extension
	for (int j = 0; j <= 15; j++)
	{
		W[j] = endian_swap(B[j]);
	}
	for (int j = 16; j <= 63; j++)
	{
		W[j] = sigma_lowercase_1(W[j - 2]) + W[j - 7] + sigma_lowercase_0(W[j - 15]) + W[j - 16];
	}
	//compression function
	//uint_256 temp = (uint_256)malloc(32 * sizeof(uint_8));
	unsigned int temp[8];
	for (int j = 0; j <= 7; j++)
	{
		temp[j] = V[j];
	}
	enum NUM {
		V_A,V_B,V_C,V_D,V_E,V_F,V_G,V_H
	};
	uint_32 t0, t1;
	for (int j = 0; j <= 63; j++)
	{
		t0 = ((W[j] + IV_64[j]) + temp[V_H] + Ch(temp[V_E], temp[V_F], temp[V_G])) + sigma_uppercase_1(temp[V_E]);
		t1 = ((t0 + Ma(temp[V_A], temp[V_B], temp[V_C])) + sigma_uppercase_0(temp[V_A]));
		temp[V_H] = temp[V_G];
		temp[V_G] = temp[V_F];
		temp[V_F] = temp[V_E];
		temp[V_E] = temp[V_D] + t0;
		temp[V_D] = temp[V_C];
		temp[V_C] = temp[V_B];
		temp[V_B] = temp[V_A];
		temp[V_A] = t1;
	}
	for (int j = 0; j <= 7; j++)
	{
		V[j] = temp[j] + V[j];
	}
}

void sha256_do(uint_8* input, uint_256 output,long long size)
{
	//padding
	int k = ((448 - (size + 1)) % 512) >= 0 ?
		((448 - (size + 1)) % 512) : ((448 - (size + 1)) % 512) + 512;
	uint_8* temp = (uint_8*)calloc((int)((size + k + 65) >> 3), sizeof(uint_8));
	for (long long i = 0; i < (size >> 3); i++)
	{
		temp[i] = input[i];
	}
	temp[size >> 3] = 0x80;
	for (long long i = 0; i < 8; i++)
	{
		temp[((size + k + 65) >> 3) - i - 1] = ((uint_8*)(&size))[i];
	}

	output[0] = IV_8[0];
	output[1] = IV_8[1];
	output[2] = IV_8[2];
	output[3] = IV_8[3];
	output[4] = IV_8[4];
	output[5] = IV_8[5];
	output[6] = IV_8[6];
	output[7] = IV_8[7];

	long long num = ((size + k + 65) >> 9);

	for (long long i = 0; i < num; i++)
	{
		CF(output, (uint_512)(temp + (i * 64)));
	}
	free(temp);
}