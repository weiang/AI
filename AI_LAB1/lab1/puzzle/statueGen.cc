/*************************************************************************
	> File Name: statueGen.cc
	> Author: Weiang
	> Mail: weiang@mail.ustc.edu.cn 
	> Created Time: 2014年04月13日 星期日 10时57分44秒
    > Describition: 
 ************************************************************************/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>
#include <ctime>

using namespace std;

int main(int argc, char *argv[])
{
	if (argc != 3) {
		cout << "Usage:" << argv[0] << " <FileName> <Number>" << endl;
		return 0;
	}
	int arr[13];
	int num = atoi(argv[2]);
	ofstream out(argv[1]);
//	cout << "Num = " << num << endl;
	srand(time(NULL));
	for (int i = 0; i != num; i ++) {
		for (int l = 0; l != 13; l ++)
			arr[l] = -1;
		for (int j = 0; j != 13; j ++) {
			if (j == 4 || j == 9) {
				out << -1 << " ";
			}
			bool flag = true;
			int tmp;
			while (flag) {
				tmp = rand() % 13;
				flag = false;
				int m = 0;
				while(arr[m] != -1) {
					if (arr[m] == tmp) {
						flag = true;
						break;
					}
					m ++;
				}
//				cout << "m = " << m << " j = " << j << endl;
			}
			arr[j] = tmp;
			out << tmp << " ";
			if (j == 2 || j == 4 || j == 7 || j == 9 || j == 12)
				out << endl;
		}
	out << endl;
	}
	out.close();
	return 0;
}
