#include"sm3.h"


int main()
{
	uint_8* a = (uint_8*)calloc(3 , sizeof(uint_8));
	
	a[0] = 0x61;
	a[1] = 0x62;
	a[2] = 0x63;	

	unsigned int b[8];
	int c[8] = { 0x66c7f0f4 , 0x62eeedd9 , 0xd1f2d46b , 0xdc10e4e2 , 0x4167c487 , 0x5cf2f7a2 , 0x297da02b , 0x8f4ba8e0 };
	printf("待加密内容：%#x %#x %#x\n",a[0],a[1],a[2]);
#define encrypto_num 10000000
	printf("独立加密次数：%d\n", encrypto_num);
	std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
	//10000000
	for (int i = 0; i < encrypto_num; i++)
	{
		sm3_do(a, b, 24);
	}

	std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed = end - start;
	std::cout << "\n耗时\n" << elapsed.count() << "s\n";
	printf("加密结果：%#x %#x %#x %#x %#x %#x %#x %#x \n", b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7]);
	for (int i = 0; i < 8; i++)
	{
		if (b[i] != c[i]) {
			printf("b[%d] = %#x error!\n", i, b[i]);
		}
	}
	float s = ((float)encrypto_num * (float)512) / ((float)8 * (float)1024 * (float)1024 * (float)1024);
	printf("加密模块吞吐量：%05f GiB/s\n", s);
	free(a);
	return 0;
}