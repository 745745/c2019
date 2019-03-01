#include<iostream>
#include<cmath>
using namespace std;
int build_primelist(void);
bool ISprime(int x);
bool inquire(int *A,int a,int b); //�����������Ƿ���Ԫ��a�������С,b����Ҫ���ҵ�Ԫ��
const int MAX = 1000000;
int A[MAX];

int main()    
{ 
	int p=build_primelist();//pΪMAX���ڵ���������
	int *B = new int[p + 1];
	int n = 1;
	for (int i = 2; i <= MAX; i++)   //����һ��ȫ������������B
		if (A[i] == 1)
		{
			B[n] =i;
			n++;
		}


			for (int i = 4; i <= MAX; i+=2)   //���濪ʼ��֤
			
				
					for (int a = 1; B[a]<i; a++)
					{
						if (inquire(B, p + 1, i - B[a]))
						{
							cout << i << "���ϸ�°ͺղ���,��Ϊ" <<B[a]<<"+"<<i-B[a]<< endl;
							break;
						}
					}
				
			delete[]B; system("pause");
				
}

bool ISprime(int x)
{
	if (x > 6)
	{
		if (x % 6 != 1 && x % 6 != 5)     //����ֻ����6n+1������6n-1����ʽ
			return false;
	}
	if (x % 2 == 0)
		return false;
	for (int i = 2; i <= int(sqrt(x)); i++)
	{
		if (x%i == 0)
			return false;
	}
	return true;

}

int build_primelist(void)
{
	int b = 0;
	fill_n(A, MAX, 1);
	for (int i = 3; i <= MAX; i++)
	{
		if (ISprime(i))
			b++;
		if (i % 2 == 0)
			A[i] = 0;
		if (!ISprime(i))
		{
			int b = i;
			for (int n = 1; b <= MAX; b *= n, n++)
				A[b] = 0;
		}
	}
	return b;
	
}

bool inquire(int *A, int a, int b)
{
	for (int i = 1; i <= a; i++)
		if (A[i] == b)
			return true;

	return false;
}