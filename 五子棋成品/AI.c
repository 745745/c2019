#include "AI.h"
#include"UI.h"
#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
unsigned char gDeep = DEEP;
static int isFirst = 0;
extern unsigned char AIchesscolor;
extern unsigned char humchesscolor;
static unsigned int score_table(unsigned char number, unsigned char empty)
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
static unsigned int count_score(unsigned char n[], unsigned char chessColor)
{   
	unsigned char i = 1;
	unsigned char empty = 0;
	unsigned char number = 0;
	unsigned int scoretmp = 0;
    if (n[0] == nullchess)
        empty++;
	else if (n[0] == chessColor)
        number++;
    
    while (i < board_size) {
        if (n[i] == chessColor) {
            number++;
        }
        else if (n[i] == nullchess) {
            if (number == 0) {
                empty = 1;
            }
            else {
                scoretmp += score_table(number, empty+1);
				empty = 1;
				number = 0;
            }
        }
        else {
			scoretmp += score_table(number, empty);
			empty = 0;
			number = 0;
		}
		i++;
    }
    scoretmp += score_table(number, empty);
	return scoretmp;
}

static int evaluate_board(unsigned char chessed[][board_size])//������������������
{
	unsigned int AIScore=0;
	unsigned int humScore=0;
	unsigned char i, j;
    char x, y;
	unsigned char n[board_size];
    memset(n, 0, sizeof(n));
	
	for (i=0; i<board_size; i++) {
        for(j=0; j<board_size; j++) {
            n[j] = chessed[i][j];
        }
    	AIScore += count_score(n, AIchesscolor);
    	humScore += count_score(n, humchesscolor);
    	memset(n, 0, sizeof(n));
	}
	//����
	for (j=0; j<board_size; j++) {
		for(i=0; i<board_size; i++) {
            n[i] = chessed[i][j];
        }
		AIScore += count_score(n, AIchesscolor);
		humScore += count_score(n, humchesscolor);
    	memset(n, 0, sizeof(n));
	} 
	//�ϰ���б��(\)
	for (i=0; i<board_size; i++) {
		for(x=i,y=0; x<board_size&&y<board_size; x++,y++) {
            n[y] = chessed[x][y];
		}
		AIScore += count_score(n, AIchesscolor);
		humScore += count_score(n, humchesscolor);
    	memset(n, 0, sizeof(n));
	} 
	//�°���б��
	for (j=1; j<board_size; j++) {
		for(x=0,y=j; y<board_size&&x<board_size; x++,y++) {
            n[x] = chessed[x][y];
		}
		AIScore += count_score(n, AIchesscolor);
		humScore += count_score(n, humchesscolor);
    	memset(n, 0, sizeof(n));
	} 
	//�ϰ뷴б��(/)
	for (i=0; i<board_size; i++) {
		for(y=i,x=0; y>=0&&x<board_size; y--,x++) {
            n[x] = chessed[x][y];
		}
		AIScore += count_score(n, AIchesscolor);
		humScore += count_score(n, humchesscolor);
    	memset(n, 0, sizeof(n));
	} 
	//�°뷴б��
	for (j=1; j<board_size; j++) {
		for (y=j,x=board_size-1; y<board_size&&x>=0; y++,x--) {
            n[y-j] = chessed[x][y];
		}
		AIScore += count_score(n, AIchesscolor);
		humScore += count_score(n, humchesscolor);
    	memset(n, 0, sizeof(n));
	} 
    return AIScore-humScore;
}
/*�Ƿ����ھ�:����֮���Ƿ����Ӵ���(�����Ƕ��ֻ����Լ����Ӷ�����)*/
static unsigned int has_neighbors(int x, int y,unsigned char chessed[][board_size])
{
	int s = 2;	//����֮������
	unsigned char i = 0, j = 0;
	for (i = (x-s>0?x-s:0); i <= x+s&&i<board_size; i++)
		for (j = (y-s>0?y-s:0); j <= y+s&&j<board_size; j++)
			if (i !=  0 || j != 0)
				if (chessed[i][j] != 0)
					return 1;
	return 0;
}

//�������Ż�:��Ŀ��������Ƚ��й��֣�Ȼ������,��Ϊalpha-beta��֦�����ڿ���˳��.
/*������������(�������ӵĿ�λ)*/		
static void generate_point(chess_queue *queue, unsigned char chessed[][board_size])
{
    int i,j,k = 0;
	for (i=0; i<board_size; i++) {   
		for (j=0; j<board_size; j++) {      
            //���ھӵĿ���,��Ϊ�����ӵĶ���
			if ((chessed[i][j]==nullchess) && has_neighbors(i,j,chessed)) {
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
static int min_alphabeta(char depth, chess_t chess, unsigned char chessed[][board_size],int alpha, int beta)
{
    unsigned char i, x, y;
    int tmpScore;
    int best = WIN5;
    chess_queue queue;
    if (depth <= 0) 
	{
        return evaluate_board(chessed);
    }
    if (check(chess.x)| check(chess.y)) 
	{
        return -1;
    }
	generate_point(&queue,chessed);
	for (i=0; i<queue.len; i++) 
	{
	    x = queue.chess[i].x;
        y = queue.chess[i].y;
        chessed[x][y] = humchesscolor;//������һ����
    
        tmpScore = max_alphabeta(depth-1, queue.chess[i],chessed,best<alpha?best:alpha, beta);
        chessed[x][y] = nullchess;  
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
static int max_alphabeta(char depth, chess_t chess,unsigned char chessed[][board_size],int alpha, int beta)	
{
    unsigned char i, x, y;
    int tmpScore;
    int best = INT_FAST32_MIN;
    chess_queue queue;   
    //��һ�������������ܲ�����Ӯ
	if ((depth <= 0) || (is_win(chess.x, chess.y, humchesscolor))) {
        return evaluate_board(chessed);
	}
	//�µ�����������
    if (check(chess.x)| check(chess.y)) {
        return -1;
    }
	generate_point(&queue,chessed);
	for (i=0; i<queue.len; i++) {
	    x = queue.chess[i].x;
        y = queue.chess[i].y;
        chessed[x][y] = AIchesscolor;//������һ����
        tmpScore = min_alphabeta(depth-1, queue.chess[i], chessed,alpha, best>beta?best:beta);
        chessed[x][y] = nullchess;//�ָ��ɿ���
		if (tmpScore > best) {      
            best = tmpScore; 
        }
        if (tmpScore > alpha) {
            break;
        }
	} 
	return best;
}

//����Сֵ����depth��������Ž�
static void chess_ai_minmax_alphabeta(chess_t *chess, unsigned char chessed[][board_size],char depth)
{
	unsigned char i = 0, k = 0;
    unsigned char x = 0, y = 0;
    int tmp = 0;
    int best = -WIN5;
    chess_queue option_queue; //��ѡ�Ŀ��Ӷ���
    chess_queue sure_queue;//����ʵ�����λ��
	generate_point(&option_queue,chessed);
    
	for (i = 0; i < option_queue.len; i++) {
        x = option_queue.chess[i].x;
        y = option_queue.chess[i].y;
	    chessed[x][y] = AIchesscolor;    //��������AIѡ����ɫ����ֹ����ݹ�ʱ���ٵݹ鵽
        tmp = min_alphabeta(depth-1, option_queue.chess[i],chessed, WIN5, -WIN5);        
		if (tmp == best) {      
            sure_queue.chess[k].x = option_queue.chess[i].x;
            sure_queue.chess[k].y = option_queue.chess[i].y;
			sure_queue.len = k+1;
            k++;
        }
        //�ҵ�һ�����õķ�,����ǰ���λ��ȫ�����
		if (tmp > best) {
			best = tmp;
            k = 0;
			sure_queue.len = 1;//�����ѡ����
            sure_queue.chess[k].x = option_queue.chess[i].x;
            sure_queue.chess[k].y = option_queue.chess[i].y;
		}
		chessed[x][y] = nullchess;//�ָ��ɿ���
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
int AI_play_chess(unsigned char chessed[][board_size],chess_t *aiChess)
{
    chess_t chess;
    chess.x = 0;
    chess.y = 0;
    //���̿յ�����,���һ���м����.(���Ӷ������ɲ���)
    if (isFirst) {
        chess.x =(int)(rand()%5) + 5;
        chess.y =(int)(rand()%5) + 5; 
    } else {
        chess_ai_minmax_alphabeta(&chess,chessed,gDeep);
    }
    isFirst = 0;
    //���õ���xy���긳ֵ��aiChess
    aiChess->x = chess.x;
    aiChess->y = chess.y;
    return 0;
}