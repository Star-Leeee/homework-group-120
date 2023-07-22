#include"sm3.h"
//#include<stdlib.h>
#include<iostream>
#include<chrono>
int main()
{
	uint_8* a = (uint_8*)malloc(3 * sizeof(uint_8));
	a[0]=0x61;
	a[1]=0x62;
	a[2]=0x63;
	uint_256 b= (uint_256)malloc(32 * sizeof(uint_8));
	//for (int i = 0; i < 8; i++)
	//{
		//printf("%-#12x", b[i]);
	//}
	std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
	
	for (int i = 0; i < 10000000; i++)
	{
		sm3_do(a, b, 24);
	}
	std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed = end - start;
	std::cout << "\nºÄÊ±\n" << elapsed.count() << "s\n";
	for (int i = 0; i < 8; i++)
	{
		printf("%-#12x", b[i]);
	}
	return 0;
	delete a;
	delete b;
}