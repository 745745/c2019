#include"head.h"
using namespace std;
char A[25][25];
void read_map()
{
	int m;
	cout << "请选择你要玩的地图" << endl;
	cin >> m;
	if (m == 1)
	{
		fstream read("迷宫地图.txt", ios::in || ios::out || ios::app);
		read >> noskipws;
		for (int j = 1; j <= 21; j++)
		{
			for (int i = 1; i <= 22; i++)
			{
				read >> A[j][i];
			}
		}		
		int p = move();
		read.close();
		ofstream write("迷宫地图.txt",  ios::app);
			write << endl << p;
	}
	else 
	{
		fstream read("迷宫地图2.txt", ios::in || ios::out || ios::app);
		read >> noskipws;
		for (int j = 1; j <= 21; j++)
		{
			for (int i = 1; i <= 22; i++)
			{
				read >> A[j][i];
			}
		}
		int p = move();
		read.close();
	
		ofstream write("迷宫地图.txt",   ios::app);
		write << endl << p;
	}
}

int move()
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
			if (A[x - 1][y] != 'X'&&A[x - 1][y] != 'a')
			{
				A[x - 1][y] = A[x][y]; A[x][y] = ' '; x--; total++;
			}
			else if (A[x - 1][y] != 'X'&&A[x - 1][y] == 'a')
			{
				if (A[x - 2][y] != 'X')
				{
					A[x - 2][y] = 'a';
					A[x - 1][y] = A[x][y];
					A[x][y] = ' ';
					x--;
					total++;
				}
			}
			break;
		case(80):
			if (A[x + 1][y] != 'X'&&A[x + 1][y] != 'a')
			{
				A[x + 1][y] = A[x][y]; A[x][y] = ' '; x++; total++;
			}
			else if (A[x + 1][y] != 'X'&&A[x + 1][y] == 'a')
			{
				if (A[x + 2][y] != 'X')
				{
					A[x + 2][y] = 'a';
					A[x + 1][y] = A[x][y];
					A[x][y] = ' ';
					x++;
					total++;
				}
			}
			break;
		case(75):
			if (A[x][y - 1] != 'X'&&A[x][y - 1] != 'a')
			{
				A[x][y - 1] = A[x][y];  A[x][y] = ' '; y--; total++;
			}
			else if (A[x][y - 1] != 'X'&&A[x][y - 1] == 'a')
			{
				if (A[x][y - 2] != 'X')
				{
					A[x][y - 2] = 'a';
					A[x][y - 1] = A[x][y];
					A[x][y] = ' ';
					y--;
					total++;
				}
			}
			break;
		case(77):
			if (A[x][y + 1] != 'X'&&A[x][y + 1] != 'a')
			{
				A[x][y + 1] = A[x][y];  A[x][y] = ' '; y++; total++;
			}
			else if (A[x][y + 1] != 'X'&&A[x][y + 1] == 'a')
			{
				if (A[x][y + 2] != 'X')
				{
					A[x][y + 2] = 'a';
					A[x][y + 1] = A[x][y];
					A[x][y] = ' ';
					y++;
					total++;
				}
			}
			break;
		}
		int target = 0;
		print();
		for (int i = 1; i <= 21; i++)
			for (int j = 1; j <= 22; j++)
			{
				if (A[i][j] == 'b')
					target++;
			}
		if (target == 0)
			break;
	}
	return total;
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
	cout << endl << "箱子是a,目标点是b"<<endl ;
}

