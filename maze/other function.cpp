#include"head.h"
using namespace std;
char A[25][25];
void read_maze()
{
	fstream read("迷宫地图.txt", ios::in || ios::out||ios::app);
	read >> noskipws;
	for (int j = 1; j <= 22; j++)
	{
		for (int i = 1; i <= 22; i++)
		{
			read >> A[j][i];
		}
	}
	print();
}

void move( )
{
	int total=0;
	int d;
	int x = 2;
	int y = 2;
	while ((d = _getch()) != 27)
	{
		system("cls");


		switch (d)    //开始移动目前位置
		{
		case(72):
			if (A[x - 1][y] != '8')
			{
				A[x - 1][y] = A[x][y]; A[x][y] = ' '; x--; total++;
			}
			break;
		case(80):
			if (A[x + 1][y] != '8')
			{
				A[x + 1][y] = A[x][y]; A[x][y] = ' '; x++; total++;
			}
			break;
		case(75):
			if (A[x][y - 1] != '8')
			{
				A[x][y - 1] = A[x][y];  A[x][y] = ' '; y--; total++;
			}
			break;
		case(77):
			if (A[x][y + 1] != '8')
			{
				A[x][y + 1] = A[x][y];  A[x][y] = ' '; y++; total++;
			}
			break;
		}
		print();
		if (x==20&&y==20)
		{
			cout << "你总共走了" << total << "步"<<endl;
			break;
		}
	}
	
}

void print()
{
	for (int j = 1; j <= 22; j++)
	{
		for (int i = 1; i <= 22; i++)
		{
			cout << A[j][i];
		}
	}
}