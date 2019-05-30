#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "AI.h"
  char gDeep = DEEP;
//�Ƿ�ΪAI����,������Ĭ�϶��������£�����Ĭ��Ϊ0
static int isFirst = 0;
extern char AIChessColor;//AIִ�׺���.
extern char humChessColor;
//����ֵ�� 
static int max_alphabeta(signed char	 depth, chess_t chess,   char chessed[][BOARD_SIZE],signed int alpha, signed int beta);
//��С�� 	
static int min_alphabeta(signed char	 depth, chess_t chess,   char chessed[][BOARD_SIZE],signed int alpha, signed int beta);

//�Ʒֱ�,�������Ӹ���(bumber)�����˵Ŀ��Ӹ���(empty) 
static   int score_table(  char number,   char empty)
{
    if (number >= 5)
        return WIN5;
    else if (number == 4) {
        if (empty == 2)
            return ALIVE4;
        else if (empty==1)		
            return DIE4;
    }
    else if (number == 3) {
		if (empty == 2)
            return ALIVE3;
		else if (empty == 1)		
            return DIE3;
	}
	else if (number == 2) {
		if (empty == 2)
            return ALIVE2;
		else if (empty == 1)
            return DIE2;
    }
    else if (number == 1&&empty == 2)
        return ALIVE1;
    return 0;
}

//��б�ߡ���б�ߡ��ᡢ������ת��һά���������� 
static   int count_score(  char n[],   char chessColor)
{   
      char i = 1;//n[0]�Ѿ���ǰ����
	  char empty = 0;//�յ�λ��
	  char number = 0;//���ӵĸ���
	  int scoretmp = 0;
    if (n[0] == NULL_FLAG)
        empty++;
	else if (n[0] == chessColor)
        number++;
    
    while (i < BOARD_SIZE) 
	{
        if (n[i] == chessColor) {
            number++;
        }
        else if (n[i] == NULL_FLAG) 
		{
            if (number == 0) 
			{
                empty = 1;
            }
            else 
			{
                scoretmp += score_table(number, empty+1);
				empty = 1;
				number = 0;
            }
        }
        else 
		{
			scoretmp += score_table(number, empty);
			empty = 0;
			number = 0;
		}
		i++;
    }
    scoretmp += score_table(number, empty);
	return scoretmp;
}

//�ѵ�ǰ�����������߶��浽һά����,Ȼ����һ����� 
static signed int evaluate_board(  char chessed[][BOARD_SIZE])//������������������
{
	  int AIScore=0;
	  int humScore=0;
      char i, j;
    signed char	 x, y;//�����u8,x--,y--����ʱ�������
      char n[BOARD_SIZE];
    memset(n, 0, sizeof(n));
	//���� 
	for (i=0; i<BOARD_SIZE; i++) 
	{
        for(j=0; j<BOARD_SIZE; j++) 
		{
            n[j] = chessed[i][j];
        }
    	AIScore += count_score(n, AIChessColor);
    	humScore += count_score(n, humChessColor);
    	memset(n, 0, sizeof(n));
	}
	//����
	for (j=0; j<BOARD_SIZE; j++) 
	{
		for(i=0; i<BOARD_SIZE; i++) 
		{
            n[i] = chessed[i][j];
        }
		AIScore += count_score(n, AIChessColor);
		humScore += count_score(n, humChessColor);
    	memset(n, 0, sizeof(n));
	} 
	//�ϰ���б��(\)
	for (i=0; i<BOARD_SIZE; i++) 
	{
		for(x=i,y=0; x<BOARD_SIZE&&y<BOARD_SIZE; x++,y++) 
		{
            n[y] = chessed[x][y];
		}
		AIScore += count_score(n, AIChessColor);
		humScore += count_score(n, humChessColor);
    	memset(n, 0, sizeof(n));
	} 
	//�°���б��
	for (j=1; j<BOARD_SIZE; j++) 
	{
		for(x=0,y=j; y<BOARD_SIZE&&x<BOARD_SIZE; x++,y++) 
		{
            n[x] = chessed[x][y];
		}
		AIScore += count_score(n, AIChessColor);
		humScore += count_score(n, humChessColor);
    	memset(n, 0, sizeof(n));
	} 
	//�ϰ뷴б��(/)
	for (i=0; i<BOARD_SIZE; i++) 
	{
		for(y=i,x=0; y>=0&&x<BOARD_SIZE; y--,x++) 
		{
            n[x] = chessed[x][y];
		}
		AIScore += count_score(n, AIChessColor);
		humScore += count_score(n, humChessColor);
    	memset(n, 0, sizeof(n));
	} 
	//�°뷴б��
	for (j=1; j<BOARD_SIZE; j++) 
	{
		for (y=j,x=BOARD_SIZE-1; y<BOARD_SIZE&&x>=0; y++,x--) 
		{
            n[y-j] = chessed[x][y];
		}
		AIScore += count_score(n, AIChessColor);
		humScore += count_score(n, humChessColor);
    	memset(n, 0, sizeof(n));
	} 
    return AIScore-humScore;
}

//�Ƿ����ھ�:����֮���Ƿ����Ӵ���(�����Ƕ��ֻ����Լ����Ӷ�����) 
static   int has_neighbors(int x, int y,  char chessed[][BOARD_SIZE])
{
	int s = 2;	//����֮������
	  char i = 0, j = 0;
	for (i = (x-s>0?x-s:0); i <= x+s&&i<BOARD_SIZE; i++)
		for (j = (y-s>0?y-s:0); j <= y+s&&j<BOARD_SIZE; j++)
			if (i !=  0 || j != 0)
				if (NULL_FLAG != (chessed[i][j]))
					return TRUE;
	return FALSE;
}

//�������Ż�:��Ŀ��������Ƚ��й��֣�Ȼ������,��Ϊalpha-beta��֦�����ڿ���˳��.
//������������(�������ӵĿ�λ) 		
static void generate_point(chess_queue *queue,  char chessed[][BOARD_SIZE])	
{
    int i,j,k = 0;
	for (i=0; i<BOARD_SIZE; i++) 
	{   
		for (j=0; j<BOARD_SIZE; j++) 
		{      
            //���ھӵĿ���,��Ϊ�����ӵĶ���
			if ((chessed[i][j]==NULL_FLAG) && has_neighbors(i,j,chessed)) 
			{
                queue->chess[k].x = i;
                queue->chess[k].y = j;
                queue->len = k+1;
                k++;
			}
        }
    }
}

//alpha����ʾĿǰΪֹ��һ���ҵ�����С��
//beta����ʾĿǰΪֹ��һ���ҵ��������
//��min�㣨��ң�����ʱ,���������� 
static int min_alphabeta(signed char	 depth, chess_t chess,   char chessed[][BOARD_SIZE],signed int alpha, signed int beta)
{
	signed int res = evaluate_board(chessed);
      char i, x, y;
    signed int tmpScore;
    signed int best = WIN5;
    chess_queue queue;

    if (depth <= 0) 
	{
        return res;
    }
    if (check(chess.x)| check(chess.y)) 
	{
        return GO_ERROR;
    }
	generate_point(&queue,chessed);
	for (i=0; i<queue.len; i++) 
	{
	    x = queue.chess[i].x;
        y = queue.chess[i].y;
        chessed[x][y] = humChessColor;//������һ����
    
        tmpScore = max_alphabeta(depth-1, queue.chess[i],chessed,best<alpha?best:alpha, beta);
        chessed[x][y] = NULL_FLAG;  
		if (tmpScore < best) 
		{      
            best = tmpScore; 
        }

        if (tmpScore < beta) 
		{
            break;
        }
	} 
	return best;
}

//��max�㣨����AI������ʱ,������õ���� 
static int max_alphabeta(signed char	 depth, chess_t chess,  char chessed[][BOARD_SIZE],signed int alpha, signed int beta)	
{
	signed int res = evaluate_board(chessed);
      char i, x, y;
    signed int tmpScore;
    signed int best = -WIN5;
    chess_queue queue;   
    //��һ�������������ܲ�����Ӯ
	if ((depth <= 0) || (is_win(chess.x, chess.y, humChessColor))) 
	{
        return res;
	}
    if (check(chess.x)| check(chess.y)) 
	{
        return GO_ERROR;
    }
	generate_point(&queue,chessed);

	for (i=0; i<queue.len; i++) 
	{
	    x = queue.chess[i].x;
        y = queue.chess[i].y;
        chessed[x][y] = AIChessColor;//������һ����
        tmpScore = min_alphabeta(depth-1, queue.chess[i], chessed,alpha, best>beta?best:beta);
        chessed[x][y] = NULL_FLAG;//�ָ��ɿ���
		if (tmpScore > best) 
		{      
            best = tmpScore; 
        }
        if (tmpScore > alpha) 
		{
            break;
        }
	} 
	return best;
}

//����Сֵ����depth��������Ž� 
static void chess_ai_minmax_alphabeta(chess_t *chess,char chessed[][BOARD_SIZE],signed char	depth)
{
	  char i = 0, k = 0;
      char x = 0, y = 0;
    signed int tmp = 0;
    signed int best = -WIN5;
    chess_queue option_queue; //��ѡ�Ŀ��Ӷ���
    chess_queue sure_queue;//����ʵ�����λ��
	generate_point(&option_queue,chessed);
    
	for (i = 0; i < option_queue.len; i++) 
	{
        x = option_queue.chess[i].x;
        y = option_queue.chess[i].y;
	    chessed[x][y] = AIChessColor;    //��������AIѡ����ɫ����ֹ����ݹ�ʱ���ٵݹ鵽
        tmp = min_alphabeta(depth-1, option_queue.chess[i],chessed, WIN5, -WIN5);        
		if (tmp == best) 
		{      
            sure_queue.chess[k].x = option_queue.chess[i].x;
            sure_queue.chess[k].y = option_queue.chess[i].y;
			sure_queue.len = k+1;
            k++;
        }
        //�ҵ�һ�����õķ�,����ǰ���λ��ȫ�����
		if (tmp > best) 
		{
			best = tmp;
            k = 0;
			sure_queue.len = 1;//�����ѡ����
            sure_queue.chess[k].x = option_queue.chess[i].x;
            sure_queue.chess[k].y = option_queue.chess[i].y;
		}
		chessed[x][y] = NULL_FLAG;//�ָ��ɿ���
	}
	k =(int)(rand()%sure_queue.len);//����ж����߷���,���ѡ��һ��
	if ( check(sure_queue.chess[k].x) || check(sure_queue.chess[k].y)) {
        chess->x = 0;
        chess->y = 0;
        MessageBox(NULL, TEXT("Error AI space ��"), TEXT("��ʾ"), MB_OK);
        return;
    }
	chess->x = sure_queue.chess[k].x;
	chess->y = sure_queue.chess[k].y;
}

//AI���� 
int AI_play_chess(  char chessed[][BOARD_SIZE],chess_t *aiChess)
{
    chess_t chess;
    chess.x = 0;
    chess.y = 0;
    //���̿յ�����,���һ���м����.(���Ӷ������ɲ���)
    if (isFirst) 
	{
        chess.x =(int)(rand()%5) + 5;
        chess.y =(int)(rand()%5) + 5; 
    } else 
	{
        chess_ai_minmax_alphabeta(&chess,chessed,gDeep);
    }
    isFirst = 0;
    //���õ���xy���긳ֵ��aiChess
    aiChess->x = chess.x;
    aiChess->y = chess.y;
    return 0;
}