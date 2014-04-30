/*************************************************************************
	> File Name: t2.cc
	> Author: Weiang
	> Mail: weiang@mail.ustc.edu.cn 
	> Created Time: 2014年04月17日 星期四 15时15分37秒
    > Describition: 
 ************************************************************************/

#include <iostream>

using namespace std;

int main()
{
	int i;
	int flag[1000];
	for (i = 0; i != 1000; i ++) {
		flag[i] = 0;
	}

	for (i = 0; i != 1000; i ++) {
		int tmp;
		cin >> tmp;
		flag[tmp] ++;
	}
	for (i = 0; i != 1000; i ++) {
		cout  << i << ", " << flag[i] << endl;
	}
	return 0;
}

