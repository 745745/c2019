#include<iostream>
#include<cmath>
using namespace std;
void build_primelist(int* A);
bool ISprime(int x);
const int MAX = 1000000;
int A[MAX];

int main()    //������ʹ��ɸ��,�������Ϊһ����(һ����̫���ˡ���������20�������鶼��û����)
{
	build_primelist(A);
	 int n;
	cin >> n;
	if (n == 1)
		cout << "1�Ȳ�������Ҳ���Ǻ���";
	else if (A[n] == 1)
		cout << "������";
	else cout << "�Ǻ���";  
	
	system("pause");
}

bool ISprime(int x)
{
	if (x % 2 == 0)
		return 0;
	for (int i = 2; i <=int(sqrt(x)); i++)  
	{
		if (x%i == 0)
			return false;
	}
	return true;   
	
}

void build_primelist(int* A)
{
	fill_n(A, MAX, 1);

	for (int i = 3; i <= MAX; i++)
	{
		if (i % 2 == 0)
			A[i] = 0;
		if (!ISprime(i))
		{
			int b = i;
			for (int n = 1; b <= MAX; b *= n, n++)
				A[b] = 0;

		}
	}
	
}