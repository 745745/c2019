#include"head.h"
using namespace std;

struct good
{
	char TYPENAME[50]; //�ͺ�
	int NUM; //����

};
good A[1000];


void read()
{
	fstream out("goods.txt", ios::in|ios::out);

	for(int i=0; !out.eof();i++)
	{
		out >> A[i].TYPENAME>>A[i].NUM;
	}
	
		
}


void print()
{
	system("cls");
	cout << "ѡ��Ҫִ�еĹ��ܣ�1����ʾ����б�2����⣬3�����⣻4���˳�" << endl;

}

void write()
{
	fstream in("goods.txt", ios::in | ios::out | ios::trunc);
	int x = 0;
	while (A[x].TYPENAME[0]!='\0')
	{
		in << A[x].TYPENAME << " " << A[x].NUM<<endl;
		x++;
	}
	in.close();
}

void select_mode()
{
	int x;
	cout << "ѡ��Ҫִ�еĹ��ܣ�1����ʾ����б�2����⣬3�����⣻4���˳�"<<endl;
	while (cin >> x)
	{
		if (x < 1 || x>4)
		{
			cout << "��������ȷ��ָ��";
			continue;
		}
		switch (x)
		{
		case 1:show_list(); break;
		case 2:in_Warehouse(); break;
		case 3:out_Warehouse(); break;
		case 4:write(); return;
		}
	}
}

void show_list()  //��ʾ����б�
{
	print();
	for (int i = 0; A[i].TYPENAME[0]!= '\0'; i++)
		cout << A[i].TYPENAME << " " << A[i].NUM<<endl;
}

void in_Warehouse()
{
	show_list();
	int x = 1,y=1;
	int num;
	char NAME[50];
	cout << "��������Ҫ������Ʒ����������"<<endl;
	cin >> NAME>>num;
	for (int i = 0; A[i].TYPENAME[0] != '\0'; i++)
		{
			if (strcmp(NAME, A[i].TYPENAME) == 0)
			{
			x = 0;
			A[i].NUM += num;
			cout << strcmp(NAME, A[i].TYPENAME)<<" " <<x << endl;
			}
			y = i;
		}
	if (x)
	{  
		memcpy(A[y].TYPENAME,NAME,sizeof(NAME));
		A[y].NUM = 0;    //�ȸ�ֵ�������δ��ֵ����δ֪�������
		A[y].NUM += num;
	}
}

void out_Warehouse()
{
	show_list();
	int x = 1, y = 1;
	int num;
	char NAME[50];
	cout << "��������Ҫ�������Ʒ����������"<<endl;
	cin >> NAME>>num;
	for (int i = 0; A[i].TYPENAME[0] != '\0'; i++)
	{
		if (strcmp(NAME, A[i].TYPENAME) == 0)
		{
			x = 0;
			if (A[i].NUM >= num)
			{
				A[i].NUM -= num;
			}
			else cout << "����Ʒ�������㣬�޷�����"<<endl;
			
		}
		y = i;   //��¼һ�¹��ж�������Ʒ
	}
	if (x)
	{
		cout << "����û��������Ʒ"<<endl;
	}
}