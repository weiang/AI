
#include "stdlib.h"
#include "time.h"
#include "string.h"
#include "math.h"
#include "stdio.h"
#include <iostream>

using namespace std;
long *solution2;    // 解 
long NumCollisions; // 当前解的冲突总数

long *positive2; 
long *negative2; 

inline void Swap2(long &x, long &y)
{
	long temp;

	temp = y;
	y = x;
	x = temp;
}

void GeneratePermutation(int N)
{
	for (long i = 0; i < N; i++) solution2[i] = i;

	long rand_index;

	for (long i = 0; i < N; i++)
	{
		rand_index = rand() % N;
		Swap2(solution2[i],solution2[rand_index]);
	}
}

long CountCollisions(int N)
{	
	long max = 2 * N - 1;

	NumCollisions = 0;

	for (long i = 0; i < max; i++)
	{
		NumCollisions += positive2[i] > 1 ? positive2[i] - 1: 0;
		NumCollisions += negative2[i] > 1 ? negative2[i] - 1: 0;
	}

	return NumCollisions;
}


void SetDiagonals(int N)
{
	memset(positive2,0,sizeof(long)*(2*N-1));
	memset(negative2,0,sizeof(long)*(2*N-1));

	for (long i = 0; i < N; i++)
	{
		positive2[i - solution2[i] + N -1]++;
		negative2[i + solution2[i]]++;
	}
}

long Swap2Evaluate(long i, long j, int N)
{
	long ret = 0;

	// 两个解在同一个条正对角线上
	if (i - solution2[i] == j - solution2[j])
	{
		ret = (positive2[i - solution2[i] + N -1] > 2 ? 2: 1) + (negative2[i + solution2[i]] > 1 ? 1 : 0) +
			   (negative2[j + solution2[j]] > 1 ? 1 : 0) -
			   (negative2[i + solution2[j]] > 0 ? 2 : 1) -
			   (positive2[i - solution2[j] + N -1] > 0 ? 1: 0) - (positive2[j - solution2[i] + N -1] > 0 ? 1: 0);			   

		return ret;
	}

	// 两个解在同一条负对角线上
	if (i + solution2[i] == j + solution2[j])
	{
		ret = (negative2[i + solution2[i]] > 2 ? 2: 1) + (positive2[i - solution2[i] + N -1] > 1 ? 1 : 0) +
			  (positive2[j - solution2[j] + N -1] > 1 ? 1 : 0) -
			  (positive2[i - solution2[j] + N -1] > 0 ? 2 : 1) -
			  (negative2[i + solution2[j]] > 0 ? 1: 0) - (negative2[j + solution2[i]] > 0 ? 1: 0);			   

		return ret;
	}


	// 一般情况，即两个解在不同的对角线上
	ret = (positive2[i - solution2[i] + N -1] > 1 ? 1 : 0) + (negative2[i + solution2[i]] > 1 ? 1 : 0) + 
		  (positive2[j - solution2[j] + N -1] > 1 ? 1 : 0) + (negative2[j + solution2[j]] > 1 ? 1 : 0) - 
		  (positive2[i - solution2[j] + N -1] > 0 ? 1 : 0) - (negative2[i + solution2[j]] > 0 ? 1 : 0) - 
		  (positive2[j - solution2[i] + N -1] > 0 ? 1 : 0) - (negative2[j + solution2[i]] > 0 ? 1 : 0);

	return ret; // ret > 0 表明冲突减少，ret < 0 表示冲突增加
}

/*----------------------------------------------
本函数交换 solution2[i], solution2[j], 更新对角线数组及冲突总数
 ----------------------------------------------*/
void Swap2Queens(long i, long j, long gain, int N)
{
	positive2[i - solution2[i] + N -1] --;
	negative2[i + solution2[i]] --;
	positive2[j - solution2[j] + N -1] --;
	negative2[j + solution2[j]] --;

	Swap2(solution2[i], solution2[j]);

	positive2[i - solution2[i] + N -1] ++;
	negative2[i + solution2[i]] ++;
	positive2[j - solution2[j] + N -1] ++;
	negative2[j + solution2[j]] ++;

	NumCollisions -= gain;
}

void nqueen2(size_t *queen, int N)
{

	solution2 = new long[N]; // 使用下标 0 - (N-1)

	positive2 = new long[2*N-1];

	negative2 = new long[2*N-1];

	do {
		GeneratePermutation(N);
		SetDiagonals(N);
		CountCollisions(N);

		bool flag = true;
		long gain;

		while (flag)
		{
			flag = false;
			for (long i = 0; i < N; i++)
				for (long j = i; j < N; j++)
					if (positive2[i - solution2[i] + N -1] > 1 || negative2[i + solution2[i]] > 1 || 
						positive2[j - solution2[j] + N -1] > 1 || negative2[j + solution2[j]] > 1)
					{
						gain = Swap2Evaluate(i, j, N);

						if ( gain > 0)
						{
							flag = true; 
							Swap2Queens(i, j, gain, N); 
						}	
					}					
		}
		
	} while (NumCollisions > 0);

	for (int i = 0; i != N; i ++) 
		queen[i] = solution2[i]; 

	delete[] solution2;
	delete[] positive2;
	delete[] negative2;
}
