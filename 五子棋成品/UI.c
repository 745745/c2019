#include "AI.h"
#include "UI.h"
#include <windows.h>
#include <math.h>



unsigned char chessPoints[board_size][board_size];//����
unsigned char AIchesscolor = whitechess;//AIִ�׺���.
unsigned char humchesscolor = blackchess;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("MyGobang");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;
	if (!RegisterClass(&wndclass)) {
		MessageBox(NULL, TEXT("���������Ҫ�� Windows NT ����ִ�У�"), szAppName, MB_ICONERROR);
		return 0;
	}
	hwnd = CreateWindow(szAppName, 
		TEXT("xxxx��������"), 
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		610, 
		610,
		NULL, 
		NULL, 
		hInstance, 
		NULL);
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
    //gobang_init();
	return msg.wParam;
}
// ���ƺ�ɫʵ�ĵ�
HRESULT _DrawBlackSolidPoint(HDC hdc, int radius, POINT postion)
{
	SelectObject(hdc, GetStockObject(BLACK_BRUSH));
	Ellipse(hdc, postion.x - radius, postion.y - radius, postion.x + radius, postion.y + radius);
	SelectObject(hdc, GetStockObject(WHITE_BRUSH));
	return S_OK;
}
// ���ư�ɫ���ĵ�
HRESULT _DrawWhiteHollowPoint(HDC hdc, int radius, POINT postion)
{
	SelectObject(hdc, GetStockObject(WHITE_BRUSH));
	Ellipse(hdc, postion.x - radius, postion.y - radius, postion.x + radius, postion.y + radius);
	return S_OK;
}

//��������
HRESULT _DrawSolidPoint(unsigned char type, HDC hdc, int radius, POINT postion)
{
    if(type == blackchess) {
        _DrawBlackSolidPoint(hdc,radius,postion);
    }else {
        _DrawWhiteHollowPoint(hdc,radius,postion);
    }
    return S_OK;
}

// ��ȡһС���Ⱥ͸߶�
HRESULT _GetCellWidthAndHeight(POINT ptLeftTop, int cxClient, int cyClient, int *cxCell, int *cyCell)
{
	*cxCell = (cxClient - ptLeftTop.x * 2) / (board_size - 1);
	*cyCell = (cyClient - ptLeftTop.y * 2) / (board_size - 1);
	return S_OK;
}
// ��ʵ������ת��Ϊ�߼����꣬������Ҫ����ʵ�ʵ㵽���̵��ת��
HRESULT _ExChangeLogicalPosition(POINT actualPostion, POINT ptLeftTop, int cxClient, int cyClient, POINT *logicalPostion)
{
	// ���һС��Ŀ�Ⱥ͸߶�
	int cxCell = 0, cyCell = 0;
	_GetCellWidthAndHeight(ptLeftTop, cxClient, cyClient, &cxCell, &cyCell);
	// �������Ч��
	if (actualPostion.x < ptLeftTop.x || actualPostion.x > ptLeftTop.x + (board_size-1) * cxCell ||
		actualPostion.y < ptLeftTop.y || actualPostion.y > ptLeftTop.y + (board_size-1) * cyCell) {
		MessageBox(NULL ,TEXT("�������������壡"), TEXT("��ʾ"), MB_OK);
		return S_FALSE;
	}
	// ��ȡ�����ĸ���
	int xCount = 0, yCount = 0;
	POINT sidePoints[4] = { 0 };
	for (int x = ptLeftTop.x; x <= ptLeftTop.x + (board_size-1) * cxCell; x += cxCell, xCount++) {
		if (actualPostion.x >= x && actualPostion.x <= x + cxCell) {
			sidePoints[0].x = x;
			sidePoints[2].x = x;
			sidePoints[1].x = x + cxCell;
			sidePoints[3].x = x + cxCell;
			break;
		}
	}
	for (int y = ptLeftTop.y; y <= ptLeftTop.y + (board_size-1) * cyCell; y += cyCell, yCount++) {
		if (actualPostion.y >= y && actualPostion.y <= y + cyCell) {
			sidePoints[0].y = y;
			sidePoints[1].y = y;
			sidePoints[2].y = y + cyCell;
			sidePoints[3].y = y + cyCell;
			break;
		}
	}
	// ���㵱ǰ�㵽�ĸ��㵽��ǰ�����
	double lengthCount[4] = { 0 };
	for (int item = 0; item < 4; ++item) {
		lengthCount[item] = pow(abs(sidePoints[item].x - actualPostion.x), 2) + pow(abs(sidePoints[item].y - actualPostion.y), 2);
	}
	// ��ȡ�ĸ�����ֵ����̵�һ��
	int shortestIndex = 0;
	for (int item = 0; item < 4; ++item) {
		if (lengthCount[item] < lengthCount[shortestIndex]) {
			shortestIndex = item;
		}
	}
	// �����߼����꣬�����±�Ϊ0�ĵ�Ϊ��׼��
	if (1 == shortestIndex) {
		xCount += 1;
	} 
	else if (2 == shortestIndex) {
		yCount += 1; 
	}
	else if (3 == shortestIndex) {
		xCount += 1;
		yCount += 1;
	}
	logicalPostion->x = xCount;
	logicalPostion->y = yCount;
	return S_OK;
}
// ���߼�����ת��Ϊʵ������
HRESULT _ExchangeActualPositon(POINT logicalPos, int cxCell, int cyCell, POINT ptLeftTop, POINT *actualPos)
{
	actualPos->x = ptLeftTop.x + logicalPos.x * cxCell;
	actualPos->y = ptLeftTop.y + logicalPos.y * cyCell;
	return S_OK;
}
// ��������
HRESULT DrawChessBoard(HDC hdc, POINT ptLeftTop, int cxClient, int cyClient)
{
	// ���һС��Ŀ�Ⱥ͸߶�
	int cxCell = 0, cyCell = 0,row_line;
	HPEN hPen, hOldPen;
	hPen = CreatePen(PS_SOLID, 1, RGB(102, 204, 255));
	hOldPen = SelectObject(hdc, hPen);
	_GetCellWidthAndHeight(ptLeftTop, cxClient, cyClient, &cxCell, &cyCell);
	// ��������
	for (int col = 0; col < board_size; ++col) {
		MoveToEx(hdc, ptLeftTop.x + col * cxCell, ptLeftTop.y, NULL);
		LineTo(hdc, ptLeftTop.x + col * cxCell, ptLeftTop.y + (board_size-1) * cyCell);
	}
	// ���ƻ�ɫ�ĺ���
	for (int row = 0; row < board_size/2; ++row) {
		MoveToEx(hdc, ptLeftTop.x, ptLeftTop.y + cyCell + row * 2 * cyCell, NULL);
		LineTo(hdc, ptLeftTop.x + (board_size-1) * cxCell, ptLeftTop.y + cyCell + row * 2 * cyCell);
	}
	// ���ƺ�ɫ�ĺ���
    row_line = (board_size%2 == 0)?(board_size/2):(board_size/2+1);
	for (int row = 0; row < row_line; ++row) {
		MoveToEx(hdc, ptLeftTop.x, ptLeftTop.y + row * 2 * cyCell, NULL);
		LineTo(hdc, ptLeftTop.x + (board_size-1) * cxCell, ptLeftTop.y + row * 2 * cyCell);
	}
	return S_OK;
}
// ����������ص�
HRESULT DrawFiveHeavyPoint(HDC hdc, POINT ptLeftTop, int cxClient, int cyClient)
{
	// ���һС��Ŀ�Ⱥ͸߶�
	int cxCell = 0, cyCell = 0;
	_GetCellWidthAndHeight(ptLeftTop, cxClient, cyClient, &cxCell, &cyCell);
	// ���߼���ת��Ϊʵ�ʵ�
	POINT logicalPoint[5] = { 3, 3, 3, 11, 11, 3, 11, 11, 7, 7 };
	POINT actualPoint[5] = { 0 };
	for (int cPt = 0; cPt < 5; ++cPt) {
		_ExchangeActualPositon(logicalPoint[cPt], cxCell, cyCell, ptLeftTop, &actualPoint[cPt]);
	}
	// ���������ɫʵ�ĵ�
	for (int cPt = 0; cPt < 5; ++cPt) {
		_DrawBlackSolidPoint(hdc, R, actualPoint[cPt]);
	}
	return S_OK;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
    char logTxt[64] = {0};
	//����������
	static int cxClient, cyClient;
	//ÿ������Ŀ�Ⱥ͸߶�
	int cxCell = 0, cyCell = 0;
	//�������Ͻ�����
	POINT ptLeftTop;
	ptLeftTop.x = 30;
	ptLeftTop.y = 40;
	// �����ʵ�ʵ�
	POINT actualPosition;
	// �����ʵ�ʵ����ת�������߼���
	POINT logicalPostion;
	// �����ʵ�ʵ����ת�������߼����Ӧ��ʵ�ʵ�
	POINT changedActualPosition;
    int result = -1;
	// ʤ����
	int winner = nullchess;
	// ��������ֵ
	HRESULT hResult = S_FALSE;
    chess_t chess;
    chess.x = 0;
    chess.y = 0;
	switch (message)
	{
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;
	case WM_LBUTTONDOWN:
		// ���ʵ�ʵ�
		actualPosition.x = LOWORD(lParam);
		actualPosition.y = HIWORD(lParam);
		// ��ö�Ӧ�ļ��������߼���
		hResult = _ExChangeLogicalPosition(actualPosition, ptLeftTop, cxClient, cyClient, &logicalPostion);
		if (S_FALSE == hResult) {
			return 0;
		}
        result = hum_play_chess(logicalPostion.x,logicalPostion.y);
        if (result == -1) {
            return 0;
        }
        if(is_win(logicalPostion.x,logicalPostion.y,humchesscolor)) {
            winner = humchesscolor;
        }
		// ���һС��Ŀ�Ⱥ͸߶�
		_GetCellWidthAndHeight(ptLeftTop, cxClient, cyClient, &cxCell, &cyCell);
		// ���߼���ת��Ϊʵ�ʵ�
		POINT changedActualPosition;
		_ExchangeActualPositon(logicalPostion, cxCell, cyCell, ptLeftTop, &changedActualPosition);
		// ����ʵ�ʵ�
		hdc = GetDC(hwnd);
		_DrawSolidPoint(humchesscolor,hdc, CHESS_PIECE_RADIUS, changedActualPosition);
		ReleaseDC(hwnd, hdc);
        if (humchesscolor == winner) {
			MessageBox(hwnd, TEXT("�˻�ʤ��"), TEXT("��ʾ"), MB_OK);
            return 0;
		}
        //����AI���һ���㵽chess����
        AI_play_chess(chessPoints,&chess);
        chessPoints[chess.x][chess.y] = AIchesscolor;
        if(is_win(chess.x,chess.y,AIchesscolor)) {
            winner = AIchesscolor;
        }
        logicalPostion.x = chess.x;
        logicalPostion.y = chess.y;
		// ���һС��Ŀ�Ⱥ͸߶�
		_GetCellWidthAndHeight(ptLeftTop, cxClient, cyClient, &cxCell, &cyCell);
		// ���߼���ת��Ϊʵ�ʵ�
		_ExchangeActualPositon(logicalPostion, cxCell, cyCell, ptLeftTop, &changedActualPosition);
		// ����ʵ�ʵ�
		hdc = GetDC(hwnd);
		_DrawSolidPoint(AIchesscolor,hdc, CHESS_PIECE_RADIUS, changedActualPosition);
		ReleaseDC(hwnd, hdc);
                     
        if (AIchesscolor == winner) {
			MessageBox(hwnd, TEXT("���Ի�ʤ��"), TEXT("��ʾ"), MB_OK);
		}
		return 0;
	case WM_RBUTTONDOWN:
		return 0;
	case WM_MBUTTONDOWN:
        winner = nullchess;
        //�м䰴����������³�ʼ������
		memset(chessPoints, 0, sizeof(chessPoints));
		InvalidateRect(hwnd, NULL, 1);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		// ��ʼ������
		DrawChessBoard(hdc, ptLeftTop, cxClient, cyClient);// ��������
		DrawFiveHeavyPoint(hdc, ptLeftTop, cxClient, cyClient);// ����������ص�
		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

/*
�������
*/
int hum_play_chess(int a,int b)
{
    int x = a;
    int y = b;
    int ret = 0;
    if (check(x)||check(y)) {
        MessageBox(NULL, TEXT("Number Error."), TEXT("��ʾ"), MB_OK);
        return -1;
    }
    if (chessPoints[x][y] != nullchess) {
        MessageBox(NULL, TEXT("�˴��Ѿ������ӣ�����ѡ"), TEXT("��ʾ"), MB_OK);
        return -1;    
    }
    
    chessPoints[x][y] = humchesscolor;
    return 0;
}

/*�ж��Ƿ�ʤ��*/
unsigned int is_win(unsigned char x, unsigned char y, unsigned char color)
{           
	int count = 0;
	int i,j,m;
	int size = board_size;
	//��
	for (i = (x-4>0?x-4:0); (i <= x+4)&&(i<size); i++) {
		if (color == (chessPoints[i][y])) {
			count++;
			if (count >= 5) {
				return 1;
			}
		}
		else
			count = 0;
	}
	//��
	count = 0;
	for (j=(y-4>0?y-4:0); (j<=y+4)&&(j<size); j++){
		if (color == (chessPoints[x][j])) {
			count++;
			if (count >= 5) {
				return 1;
			}
		}
		else
			count = 0;
	}
	//��б��(\)
	count = 0;
    for(m = 0; m <= 4; m++) {
        if((x-m == 0) || (y-m == 0))
            break;
    }
	for (i=x-m,j=y-m;(i>=0)&&(i<=x+4)&&(i<size)&&(j>=0)&&(j<=y+4)&&(j<size); i++,j++) {
		if (color == (chessPoints[i][j])) {
			count++;
			if (count >= 5) {
				return 1;
			}
		}
		else
			count = 0;
    }

	//��б��(/)
	count = 0;
    for(m = 0; m <= 4; m++) {
        if((x-m == 0) || (y+m == (size-1)))
            break;
    }
	for (i=x-m,j=y+m;(i>=0)&&(i<=x+4)&&(i<size)&&(j>=y-4)&&(j>=0)&&(j<size); i++,j--) {
		if (color == (chessPoints[i][j])) {
			count++;
			if (count >= 5) {
				return 1;
			}
		}
		else {
			count = 0;
		}
	}
	return 0;
}