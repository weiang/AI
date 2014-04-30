#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

using namespace std;

int interval;
int *positive;
int *negative;
const int factor = 3;

void swap(int x, int y, int *queen)
{
	int tmp;

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

int partialCollisions(int j, int *queen)
{
	return positive[j + queen[j]] + negative[j - queen[j] + interval] - 2;
}

int totalCollsions(int j, int *queen)
{
	return positive[j + queen[j]] + negative[j - queen[j] + interval] - 2;
}

int random(int x, int y)
{
	//srand(time(NULL));
	return rand() % (y - x + 1) + x;
}

int initialSearch(int *queen, int n)
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

bool finalSearch(int k, int *queen, int n)
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

bool testRowLegal(int *queen, int n)
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

bool testDiagLegal(int *queen, int n)
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
bool testLegal(int *queen, int n)
{
	if (!testRowLegal(queen, n) | !testDiagLegal(queen, n))
		return false;
	return true;
}
void queenSearch(int n)
{
	int k;
	int i;
	int *queen = (int *)malloc(sizeof(int) * n);
	negative = (int *)malloc(sizeof(int) * (2 * n - 1));
	positive = (int *)malloc(sizeof(int) * (2 * n - 1));
	interval = n - 1;

	for (i = 0; i != 2 * n - 1; i ++) {
		negative[i] = 0;
		positive[i] = 0;
	}
	int flag = false;

	while (!flag) {
		k = initialSearch(queen, n);
		flag = finalSearch(k, queen, n);
	}
		
	for (k = 0; k != n; k ++) {
		printf("%d ", queen[k]);
	}
	printf("\n");
	free(queen);
	free(negative);
	free(positive);
}

int main()
{
	int n;
	scanf("%d", &n);
	int i;
	int a[] = {0, 5, 4, 2, 1, 3}; 

//	for (i = 0; i != 3; i ++) {
//		queenSearch(n);
//	}

	interval = 5;
	bool result = testDiagLegal(a, 5);
	if (result)
		cout << "True" << endl;
	else 
		cout << "False" << endl;
	return 0;
}
