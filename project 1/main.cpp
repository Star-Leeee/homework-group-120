#include"sm3.h"


int main()
{
	

	unsigned int** H_0 = (unsigned int**)malloc(procedure * sizeof(unsigned int*));
	unsigned int** H_1 = (unsigned int**)malloc(procedure * sizeof(unsigned int*));

	//initialization
#pragma omp parallel for num_threads(4) 
	for (int i = 0; i < procedure; i++)
	{
		H_0[i] = (unsigned int*)malloc(8 * sizeof(unsigned int));
		for (int j = 0; j < 8; j++)
		{
			H_0[i][j] = (unsigned int)((rand() * i + j));
		}
	//	printf("H_0[%d]\n%#010x %#010x %#010x %#010x %#010x %#010x %#010x %#010x\n", i, H_0[i][0], H_0[i][1], H_0[i][2], H_0[i][3], H_0[i][4], H_0[i][5], H_0[i][6], H_0[i][7]);
		H_1[i] = (unsigned int*)calloc(8 , sizeof(unsigned int));
		sm3_do((uint_8*)(H_0[i]), H_1[i], 256);
	}

	int byte = 1;
	omp_lock_t	read, write;
	omp_init_lock(&read);
	omp_init_lock(&write);

	int read_count=0, write_count=0;
#pragma omp parallel for num_threads(4) 
	for (int i = 0; i < procedure; i++)
	{

		for (int j = i + 1; j < procedure; j++)
		{
			omp_set_lock(&read);

			if (memcmp(H_1[i], H_1[j], byte) == 0)
			{
				omp_set_lock(&write);
				printf("byte=%d\n", byte);
				printf("H_0[%d]\n%#010x %#010x %#010x %#010x %#010x %#010x %#010x %#010x\n", i, H_0[i][0], H_0[i][1], H_0[i][2], H_0[i][3], H_0[i][4], H_0[i][5], H_0[i][6], H_0[i][7]);
				printf("H_0[%d]\n%#010x %#010x %#010x %#010x %#010x %#010x %#010x %#010x\n", j, H_0[j][0], H_0[j][1], H_0[j][2], H_0[j][3], H_0[j][4], H_0[j][5], H_0[j][6], H_0[j][7]);
				printf("H_1[%d]\n%#010x %#010x %#010x %#010x %#010x %#010x %#010x %#010x\n", i, H_1[i][0], H_1[i][1], H_1[i][2], H_1[i][3], H_1[i][4], H_1[i][5], H_1[i][6], H_1[i][7]);
				printf("H_1[%d]\n%#010x %#010x %#010x %#010x %#010x %#010x %#010x %#010x\n", j, H_1[j][0], H_1[j][1], H_1[j][2], H_1[j][3], H_1[j][4], H_1[j][5], H_1[j][6], H_1[j][7]);
				byte++;
				omp_unset_lock(&write);
			}
			omp_unset_lock(&read);

		}
	}

	omp_destroy_lock(&read);
	omp_destroy_lock(&write);

#pragma omp parallel for num_threads(4) 
	for (int i = 0; i < procedure; i++)
	{
		free(H_0[i]);
		free(H_1[i]);
	}
	system("pause");
}