#include<iostream>
#include <limits.h>
using namespace std;
int AGE = 12*7;
int son_birth;
int son_death;
int main()
{//������и�С���ɣ���Ϊ�����Ȼ������������Ŀ�ָ���1/6,1/12,1/7�������������Ϊ12,7�Ĺ�����(6��12�ϲ�Ϊ12)

	int son_birth = AGE / 6 + AGE / 12 + AGE / 7 + 5;
	int son_death = AGE - 4;
		for (int i = 1;AGE< INT_MAX; AGE *= i, i++)   //��ʵû��Ҫ�����Ǹ����޴󣬵����Է��޽������ѭ����������һ�����ֵ
		{
			if (son_death - son_birth == AGE / 2)
			{
				cout << AGE;
				break;
			}
		}
		system("pause");
}