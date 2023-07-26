#include"sm3.h"


int main()
{

	uint_8* a = (uint_8*)calloc(3 , sizeof(uint_8));
	for (int i = 0; i < 3; i+=4)
	{
		a[i+0] = 0x61;
		a[i+1] = 0x62;
		a[i+2] = 0x63;
		//a[i+3] = 0x64;
	}
	
	//a[4] = 0x00;

	uint_256 b = (uint_256)malloc(32 * sizeof(uint_8));
	int c[8] = {	0x66c7f0f4 , 0x62eeedd9 , 0xd1f2d46b , 0xdc10e4e2 , 0x4167c487 , 0x5cf2f7a2 , 0x297da02b , 0x8f4ba8e0 };

	std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
	//10000000
	for (int i = 0; i < 1; i++)
	{
		sm3_do(a, b, 24);
	}
	std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed = end - start;
	std::cout << "\nºÄÊ±\n" << elapsed.count() << "s\n";
	for (int i = 0; i < 8; i++)
	{
		if (b[i] != c[i]) {
			printf("%#x\n", b[i]);
		}
	}
	delete a;
	delete b;
	return 0;
}