/*************************************************************************
	> File Name: nqueen.c
	> Author: Weiang
	> Mail: weiang@mail.ustc.edu.cn 
	> Created Time: 2014年04月13日 星期日 16时14分59秒
    > Describition: 
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

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

void queen(int col, int n)
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
				queen(col + 1, n);
		}
	}
}
			
size_t **NQueen(size_t Queen_number)
{
	int i;
	solution = (size_t *)malloc(sizeof(size_t) * Queen_number);
	result = (size_t **)malloc(3 * sizeof(size_t *));
	for (i = 0; i!= 3; i ++)
		result[i] = (size_t *)malloc(Queen_number * sizeof(size_t));
	queen(0, Queen_number);
	free(solution);
	return result;
}

int main()
{
	int n, i, j;
	scanf("%d", &n);

	printf("n = %d\n", n);
	size_t **rs = NQueen(n);
	for (i = 0; i != solNum; i ++) {
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
