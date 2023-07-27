#include"sm3.h"




int main()
{
	std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();

	unsigned char a[32];

	unsigned int** H_i = (unsigned int**)malloc(procedure * sizeof(unsigned int*));
	unsigned int** H_2i = (unsigned int**)malloc(procedure * sizeof(unsigned int*));
	unsigned int** H_0 = (unsigned int**)malloc(procedure * sizeof(unsigned int*));
	unsigned int** H_1 = (unsigned int**)malloc(procedure * sizeof(unsigned int*));

	//initialization

	for (int i = 0; i < procedure; i++)
	{
		H_0[i] = (unsigned int*)malloc(8 * sizeof(unsigned int));
		H_1[i] = (unsigned int*)malloc(8 * sizeof(unsigned int));
		H_i[i] = (unsigned int*)malloc(8 * sizeof(unsigned int));
		H_2i[i] = (unsigned int*)malloc(8 * sizeof(unsigned int));

		for (int j = 0; j < 32; j++)
		{
			a[j] = (unsigned char)(((i+114-j) * (j*514-i))%256);
		}
		sm3_do(a, H_i[i], 256);
		memcpy(H_0[i], H_i[i], 32);
		sm3_do((uint_8*)(H_i[i]), H_i[i], 256);
		memcpy(H_1[i], H_i[i], 32);
		sm3_do((uint_8*)(H_i[i]), H_2i[i], 256);
	}

	bool flag = false;
	int byte = 1;
	

	for (;;)
	{
		#pragma omp parallel for num_threads(procedure) shared(byte)
		for (int i = 0; i < procedure; i++)
		{

			if (memcmp(H_i[i], H_2i[i], byte) == 0)
			{
				printf("byte=%d\n", byte);

				printf("H_0[%d] =\n ", i);
				for (int j = 0; j < 8; j++)
				{
					printf("%10X", H_0[i][j]);
				}
				printf("\n");

				printf("H_1[%d] =\n ", i);
				for (int j = 0; j < 8; j++)
				{
					printf("%10X", H_1[i][j]);
				}
				printf("\n");

				printf("H_i[%d] =\n ", i);
				for (int j = 0; j < 8; j++)
				{
					printf("%10X", H_i[i][j]);
				}
				printf("\n");

				printf("H_2i[%d] =\n ", i);
				for (int j = 0; j < 8; j++)
				{
					printf("%10X", H_2i[i][j]);
				}
				printf("\n\n");

				(byte) += 1;
				break;
			}

			memcpy(H_0[i], H_i[i], 32);
			sm3_do((uint_8*)(H_i[i]), H_i[i], 256);

			sm3_do((uint_8*)(H_2i[i]), H_2i[i], 256);
			memcpy(H_1[i], H_2i[i], 32);
			sm3_do((uint_8*)(H_2i[i]), H_2i[i], 256); 
		}

		//if (flag == true)
		//{
		//	std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();
		//	std::chrono::duration<double> elapsed = end - start;
		///	std::cout << "\nºÄÊ±\n" << elapsed.count() << "s\n";
		//	break;
		//}
	}
	for (int i = 0; i < procedure; i++)
	{
		free(H_0[i]);
		free(H_1[i]);
		free(H_i[i]);
		free(H_2i[i]);
	}
	system("pause");
}