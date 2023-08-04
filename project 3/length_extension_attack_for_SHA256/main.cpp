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

	printf("被填充后的消息A：\nmessage||message_padding\n");
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
	
	memcpy(&(fake_message_0[((size_sec + k + 65) >> 3)]), message, size_mes >> 3);
	printf("\n被填充后的消息A+待伪造的消息B：\nmessage||message_padding||faker_message\n");
	for (long long i = 0; i < ((size + k + 1 + 64) >> 3); i++)
	{

		printf("%02x", fake_message_0[i]);
	}
	printf("\n被填充后的消息A+伪造的消息B：\nmessage||message_padding||faker\n");
	sm3_do(fake_message_0, faker, (size + ((long long)k) + ((long long)65)));
	if (memcmp(faker, fake_H, 32) == 0)
	{
		printf("\n----attack success----\n");
		printf("\nmessage||message_padding||faker_message=\n");
		for (int j = 0; j < ((size + k + 1 + 64) >> 3); j++)
		{
			printf("%c", fake_message_0[j]);
		}
		printf("\nSM3(message||message_padding||faker_message)=\n");
		for (int j = 0; j < 8; j++)
		{
			printf("%08x ", faker[j]);
		}
		printf("\nfaker_message=\n");
		for (int j = 0; j < 32; j++)
		{
			printf("%c", message[j]);
		}
		printf("\nSM3(faker_message),IV=SM3(message||message_padding)=\n");
		for (int j = 0; j < 8; j++)
		{
			printf("%08x ", fake_H[j]);
		}
	}
}
