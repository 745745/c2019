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

static int evaluate_board(unsigned char chessed[][board_size])//评估函数，评估局势
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
	//纵排
	for (j=0; j<board_size; j++) {
		for(i=0; i<board_size; i++) {
            n[i] = chessed[i][j];
        }
		AIScore += count_score(n, AIchesscolor);
		humScore += count_score(n, humchesscolor);
    	memset(n, 0, sizeof(n));
	} 
	//上半正斜线(\)
	for (i=0; i<board_size; i++) {
		for(x=i,y=0; x<board_size&&y<board_size; x++,y++) {
            n[y] = chessed[x][y];
		}
		AIScore += count_score(n, AIchesscolor);
		humScore += count_score(n, humchesscolor);
    	memset(n, 0, sizeof(n));
	} 
	//下半正斜线
	for (j=1; j<board_size; j++) {
		for(x=0,y=j; y<board_size&&x<board_size; x++,y++) {
            n[x] = chessed[x][y];
		}
		AIScore += count_score(n, AIchesscolor);
		humScore += count_score(n, humchesscolor);
    	memset(n, 0, sizeof(n));
	} 
	//上半反斜线(/)
	for (i=0; i<board_size; i++) {
		for(y=i,x=0; y>=0&&x<board_size; y--,x++) {
            n[x] = chessed[x][y];
		}
		AIScore += count_score(n, AIchesscolor);
		humScore += count_score(n, humchesscolor);
    	memset(n, 0, sizeof(n));
	} 
	//下半反斜线
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
/*是否有邻居:两步之内是否有子存在(不论是对手还是自己的子都可以)*/
static unsigned int has_neighbors(int x, int y,unsigned char chessed[][board_size])
{
	int s = 2;	//两步之内有子
	unsigned char i = 0, j = 0;
	for (i = (x-s>0?x-s:0); i <= x+s&&i<board_size; i++)
		for (j = (y-s>0?y-s:0); j <= y+s&&j<board_size; j++)
			if (i !=  0 || j != 0)
				if (chessed[i][j] != 0)
					return 1;
	return 0;
}

//还可以优化:将目标空子们先进行估分，然后排序,因为alpha-beta剪枝依赖于空子顺序.
/*产生空子序列(可以下子的空位)*/		
static void generate_point(chess_queue *queue, unsigned char chessed[][board_size])
{
    int i,j,k = 0;
	for (i=0; i<board_size; i++) {   
		for (j=0; j<board_size; j++) {      
            //有邻居的空子,做为可下子的队列
			if ((chessed[i][j]==nullchess) && has_neighbors(i,j,chessed)) {
                queue->chess[k].x = i;
                queue->chess[k].y = j;
                queue->len = k+1;
                k++;
			}
        }
    }
}

//alpha：表示目前为止上一层找到的最小数
//beta：表示目前为止上一层找到的最大数
//当min层（玩家）下棋时,考虑最坏的情况
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
        chessed[x][y] = humchesscolor;//尝试下一个子
    
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

//当max层（电脑AI）下棋时,考虑最好的情况
static int max_alphabeta(char depth, chess_t chess,unsigned char chessed[][board_size],int alpha, int beta)	
{
    unsigned char i, x, y;
    int tmpScore;
    int best = INT_FAST32_MIN;
    chess_queue queue;   
    //上一步是玩家下棋可能产生输赢
	if ((depth <= 0) || (is_win(chess.x, chess.y, humchesscolor))) {
        return evaluate_board(chessed);
	}
	//下到棋盘外面了
    if (check(chess.x)| check(chess.y)) {
        return -1;
    }
	generate_point(&queue,chessed);
	for (i=0; i<queue.len; i++) {
	    x = queue.chess[i].x;
        y = queue.chess[i].y;
        chessed[x][y] = AIchesscolor;//尝试下一个子
        tmpScore = min_alphabeta(depth-1, queue.chess[i], chessed,alpha, best>beta?best:beta);
        chessed[x][y] = nullchess;//恢复成空子
		if (tmpScore > best) {      
            best = tmpScore; 
        }
        if (tmpScore > alpha) {
            break;
        }
	} 
	return best;
}

//极大极小值搜索depth步后的最优解
static void chess_ai_minmax_alphabeta(chess_t *chess, unsigned char chessed[][board_size],char depth)
{
	unsigned char i = 0, k = 0;
    unsigned char x = 0, y = 0;
    int tmp = 0;
    int best = -WIN5;
    chess_queue option_queue; //待选的空子队列
    chess_queue sure_queue;//最合适的下子位置
	generate_point(&option_queue,chessed);
    
	for (i = 0; i < option_queue.len; i++) {
        x = option_queue.chess[i].x;
        y = option_queue.chess[i].y;
	    chessed[x][y] = AIchesscolor;    //将该子置AI选的颜色，防止后面递归时，再递归到
        tmp = min_alphabeta(depth-1, option_queue.chess[i],chessed, WIN5, -WIN5);        
		if (tmp == best) {      
            sure_queue.chess[k].x = option_queue.chess[i].x;
            sure_queue.chess[k].y = option_queue.chess[i].y;
			sure_queue.len = k+1;
            k++;
        }
        //找到一个更好的分,把以前存的位子全部清除
		if (tmp > best) {
			best = tmp;
            k = 0;
			sure_queue.len = 1;//清空终选队列
            sure_queue.chess[k].x = option_queue.chess[i].x;
            sure_queue.chess[k].y = option_queue.chess[i].y;
		}
		chessed[x][y] = nullchess;//恢复成空子
	}
	k =(int)(rand()%sure_queue.len);//如果有多个最高分数,随机选择一个
	if ( check(sure_queue.chess[k].x) || check(sure_queue.chess[k].y)) {
        chess->x = 0;
        chess->y = 0;
        MessageBox(NULL, TEXT("Error AI space ！"), TEXT("提示"), MB_OK);
        return;
    }
	chess->x = sure_queue.chess[k].x;
	chess->y = sure_queue.chess[k].y;
}

//AI下棋
int AI_play_chess(unsigned char chessed[][board_size],chess_t *aiChess)
{
    chess_t chess;
    chess.x = 0;
    chess.y = 0;
    //棋盘空的下棋,随机一个中间的子.(空子队列生成不了)
    if (isFirst) {
        chess.x =(int)(rand()%5) + 5;
        chess.y =(int)(rand()%5) + 5; 
    } else {
        chess_ai_minmax_alphabeta(&chess,chessed,gDeep);
    }
    isFirst = 0;
    //将得到的xy坐标赋值给aiChess
    aiChess->x = chess.x;
    aiChess->y = chess.y;
    return 0;
}