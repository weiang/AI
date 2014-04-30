#include <iostream>
#include <cstdlib>
#include <cstdio>
using namespace std;

int interval;

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
	for (i = 0; i != n; i ++) 
		cout << "i = " << i << "flag[i] = " << flag[i] << endl;
	for (i = 0; i != n; i ++) {

		if (flag[i] != 1) {
			cout << "i = " << i << " " << flag[i] << endl;
			break;
		}
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

int main()
{
	int n;
	scanf("%d", &n);
	int i;
	int a[1000];
	for (i = 0; i != 1000; i ++) 
		scanf("%d", &a[i]);

//	for (i = 0; i != 3; i ++) {
//		queenSearch(n);
//	}

	interval = 1000 - 1;
	bool result = testRowLegal(a, 1000);
	if (result)
		cout << "True" << endl;
	else 
		cout << "False" << endl;
	return 0;
}

