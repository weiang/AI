/*************************************************************************
	> File Name: nqueen.cpp
	> Author: Weiang
	> Mail: weiang@mail.ustc.edu.cn 
	> Created Time: 2014年04月13日 星期日 15时58分57秒
    > Describition: 
 ************************************************************************/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>

/**
 * Method 1: Recursion algorithm
 */
size_t **result;
int solNum = 0;
size_t *solution;
int iabs(int x)
{
	return x > 0 ? x : -x;
}

int legal(int c)
{
	int i;
	for (i = 0; i < c; i ++) {
		if ((solution[i] ==solution[c])
				|| iabs(solution[i] - solution[c]) == iabs(i - c))
			return 0;
	}
	return 1;
}

void nqueen1(int col, int n)
{
	int i, j;
	if (solNum >= 3)
		return;
	else if (col >= n) {
		for (j = 0; j != n; j ++)
			result[solNum][j] = solution[j];
		solNum ++;
	}
	else {
		for (i = 0; i < n; i ++) {
			solution[col] = i;
			if (legal(col)) 
				nqueen1(col + 1, n);
		}
	}
}

/*
 * Method 2: climbing mountain
 */

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
/* Method 3: local research */
int interval;
int *positive;
int *negative;
const int factor = 3;

void swap(int x, int y, size_t *queen)
{
	size_t tmp;

//	negative[x + queen[x]] --;
//	negative[y + queen[y]] --;
//	positive[x - queen[x] + interval] --;
//	positive[y - queen[y] + interval] --;
	
	// Swap queen[x] and queen[y]
	tmp = queen[x];
	queen[x] = queen[y];
	queen[y] = tmp;

//	negative[x + queen[x]] ++;
//	negative[y + queen[y]] ++;
//	positive[x - queen[x] + interval] ++;
//	positive[y - queen[y] + interval] ++;
}

int partialCollisions(int j, size_t *queen)
{
	return positive[j + queen[j]] + negative[j - queen[j] + interval] - 2;
}

int totalCollsions(int j, size_t *queen)
{
	return positive[j + queen[j]] + negative[j - queen[j] + interval] - 2;
}

int random(int x, int y)
{
	//srand(time(NULL));
	return rand() % (y - x + 1) + x;
}

int initialSearch(size_t *queen, int n)
{
	int i, j, m;
	int iterationTimes = factor * n;
//	printf("it = %d\n", iterationTimes);
	for (i = 0; i != n; i ++) {
		queen[i] = i;
	}
	j = 0;
	for (i = 0; i != iterationTimes; i ++) {
		m = random(j, n - 1);
		if (m == j)
			continue;
//		printf("j = %d, m = %d\n", j,  m);
		swap(j, m, queen);
		negative[j - queen[j] + interval] ++;
		positive[j + queen[j]] ++;
//		printf("negative[%d] = %d, positive[%d] = %d\n", j - queen[j] + interval, negative[j - queen[j] + interval],
//					j + queen[j], positive[j + queen[j]]);
		if (partialCollisions(j, queen) == 0) {
			j ++;
		}
		else {
			negative[j - queen[j] + interval] --;
			positive[j + queen[j]] --;
			swap(j, m, queen);
		}
	}

	for (i = j; i != n; i ++) {
		m = random(i, n - 1);
		swap(i, m, queen);
		if (i == m) {
			negative[i - queen[i] + interval] ++;
			positive[i + queen[i]] ++;
		}
		else {
			negative[i - queen[i] + interval] ++;
			positive[i + queen[i]] ++;
			negative[m - queen[i] + interval] ++;
			positive[m + queen[i]] ++;
		}
	}
//	for (i = 0; i != 2 * n - 1; i ++) {
//		printf("p[%d] = %d, ne[%d] = %d\n", i, positive[i], i, negative[i]);
//	}
//	printf("n - j = %d\n", n - j);
	return n - j;
}

bool finalSearch(int k, size_t *queen, int n)
{
	int i, j;
	bool b;
	int cnt = 0;
	int itTimes = 3 * n;

//	printf("In final\n");
	for (i = n - k; i != n && cnt < itTimes;  i ++) {
		if (totalCollsions(i, queen) > 0) {
			do {
				cnt ++;
				j = random(0, n - 1);
	//			printf("i = %d, j = %d\n", i, j);
				positive[i + queen[i]] --;
				negative[i - queen[i] + interval] --; 
				positive[j + queen[j]] --;
				negative[j - queen[j] + interval] --;
				swap(i, j, queen);
				positive[i + queen[i]] ++;
				negative[i - queen[i] + interval] ++;
				positive[j + queen[j]] ++;
				negative[j - queen[j] + interval] ++;
	//			printf("negative[%d] = %d, positive[%d] = %d\n", i - queen[i] + interval, negative[i - queen[i] + interval], i + queen[i], positive[i + queen[i]]);
	//			printf("negative[%d] = %d, positive[%d] = %d\n", j - queen[j] + interval, negative[j - queen[j] + interval], j + queen[j], positive[j + queen[j]]);
	
				b = (totalCollsions(i, queen) > 0) | (totalCollsions(j, queen) > 0);
				if (b) {
					positive[i + queen[i]] --;
					negative[i - queen[i] + interval] --; 
					positive[j + queen[j]] --;
					negative[j - queen[j] + interval] --;
					swap(i, j, queen);
					positive[i + queen[i]] ++;
					negative[i - queen[i] + interval] ++;
					positive[j + queen[j]] ++;
					negative[j - queen[j] + interval] ++;
		
				}
			} while (b);
		}
	}
	if (cnt == 3 * n)
		return false;
	return true;
}

bool testRowLegal(size_t *queen, int n)
{
	int i;
	int *flag;
	flag = (int *)malloc(sizeof(int) * n);
	for (i = 0; i != n; i ++)
		flag[i] = 0;
	for (i = 0; i != n; i ++) {
		flag[queen[i]] ++;
	}
	for (i = 0; i != n; i ++) {
		if (flag[i] != 1)
			break;
	}
	if (i == n)
		return true;
	return false;
}

bool testDiagLegal(size_t *queen, int n)
{
	int i = 0;
	int *p = (int *)malloc(sizeof(int) * (2 * n - 1));
	int *ne = (int *)malloc(sizeof(int) * (2 * n - 1));
	for (i = 0; i != 2 * n - 1; i ++) {
		p[i] = 0;
		ne[i] = 0;
	}
	for (i = 0; i != n; i ++) {
		ne[i - queen[i] + interval] ++;
		p[i + queen[i]] ++;
	}
	for (i = 0; i != 2 * n - 1; i ++) {
//		printf("i = %d, p[i] = %d, ne[i] = %d\n", i, p[i], ne[i]);
		if (p[i] > 1 || ne[i] > 1)
			break;
	}
	free(p);
	free(ne);
	if (i == 2 * n - 1)
		return true;
	return false;
}
bool testLegal(size_t *queen, int n)
{
	if (!testRowLegal(queen, n) | !testDiagLegal(queen, n))
		return false;
	return true;
}
void nqueen3(size_t *queen, int n)
{
	int k;
	int i;
//	int *queen = (int *)malloc(sizeof(int) * n);
	negative = (int *)malloc(sizeof(int) * (2 * n - 1));
	positive = (int *)malloc(sizeof(int) * (2 * n - 1));
	interval = n - 1;

	for (i = 0; i != 2 * n - 1; i ++) {
		negative[i] = 0;
		positive[i] = 0;
	}
	bool passTest = false;
	while (!passTest) {
		bool flag = false;
		while (!flag) {
			k = initialSearch(queen, n);
			flag = finalSearch(k, queen, n);
		}
//		if (!testRowLegal(queen, n))
//			printf("Row error\n");
//		if (!testDiagLegal(queen, n))
//			printf("Diag error\n");
		passTest = testLegal(queen, n);
	}
		
//	for (k = 0; k != n; k ++) {
//		printf("%d ", queen[k]);
//	}
//	printf("\n");
//	free(queen);
	free(negative);
	free(positive);
}

size_t **NQueen(size_t Queen_number)
{
	int i;
	solution = (size_t *)malloc(sizeof(size_t) * Queen_number);
	result = (size_t **)malloc(3 * sizeof(size_t *));
	for (i = 0; i!= 3; i ++)
		result[i] = (size_t *)malloc(Queen_number * sizeof(size_t));
	if (0 < Queen_number && Queen_number < 15) {
		nqueen1(0, Queen_number);
		free(solution);
	}
	else if (Queen_number < 30000) {
		for (i = 0; i != 3; i ++) {
			nqueen2(result[i], Queen_number);
		}
	}
	else {
		for (i = 0; i != 3; i ++) {
			nqueen3(result[i], Queen_number);
		}
	}
	return result;
}



int main()
{
	int n, i, j;
	scanf("%d", &n);

	printf("n = %d\n", n);
	size_t **rs = NQueen(n);
	for (i = 0; i != 3; i ++) {
		if (n < 15 && i == solNum)
			break;
		printf("Solution %d:\n", i + 1);
		for (j = 0; j != n; j ++)
			printf("%lu ", rs[i][j]);
		printf("\n");
	}
	for (i = 0; i != 3; i ++) {
		free(rs[i]);
	}
	free(rs);
	return 0;
}
