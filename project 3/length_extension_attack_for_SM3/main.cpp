#include"sm3.h"


int main()
{
	uint_8	security[32];

	for (int i = 0; i < 32; i++)
	{
		security[i] = (uint_8)(rand() * rand());
	}
	unsigned int security_H0[8];
	unsigned int fake_H[8];
	unsigned int faker[8];

	sm3_do(security, security_H0, 256);

	uint_8	message[32] = { 'i','h','a','t','e','y','o','u' };
	sm3_do_length_extension_attack(message, fake_H, security_H0, 256,512);
	
	
	uint_8* fake_message_0;// , * fake_message_1;
	long long size_sec = 256, size_mes = 256;
	long long size = size_sec + size_mes;
	int k = ((448 - (size_sec + 1)) % 512) >= 0 ?
		((448 - (size_sec + 1)) % 512) : ((448 - (size_sec + 1)) % 512) + 512;
	fake_message_0 = (uint_8*)calloc((int)((size + k + 1 + 64) >> 3), sizeof(uint_8));
	//fake_message_1 = (uint_8*)calloc((int)((size + k + 1 + 64) >> 3), sizeof(uint_8));

	for (long long i = 0; i < (size_sec >> 3); i++)
	{
		fake_message_0[i] = security[i];
	}
	//memcpy(fake_message_1, security, 32);
	fake_message_0[size_sec >> 3] = 0x80;
	for (long long i = 0; i < 8; i++)
	{
		fake_message_0[((size_sec + k + 65) >> 3) - i - 1] = ((uint_8*)(&size_sec))[i];
	}
	for (long long i = 0; i < ((size + k + 1 + 64) >> 3); i++)
	{

		printf("%02x", fake_message_0[i]);
	}
	memcpy(&(fake_message_0[((size_sec + k + 65) >> 3)]), message, size_mes >> 3);
	printf("\n");
	for (long long i = 0; i < ((size + k + 1 + 64) >> 3); i++)
	{

		printf("%02x", fake_message_0[i]);
	}
	sm3_do(fake_message_0, faker, (size + ((long long)k) + ((long long)65)));
	if (memcmp(faker, fake_H, 32) == 0)
	{
		printf("attack success\n");
	}
}
