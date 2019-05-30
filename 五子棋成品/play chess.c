#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "AI.h"
  char gDeep = DEEP;
//是否为AI先下,本程序默认都是人先下，所以默认为0
static int isFirst = 0;
extern char AIChessColor;//AI执白后行.
extern char humChessColor;
//极大值层 
static int max_alphabeta(signed char	 depth, chess_t chess,   char chessed[][BOARD_SIZE],signed int alpha, signed int beta);
//极小层 	
static int min_alphabeta(signed char	 depth, chess_t chess,   char chessed[][BOARD_SIZE],signed int alpha, signed int beta);

//计分表,依据连子个数(bumber)和两端的空子个数(empty) 
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

//正斜线、反斜线、横、竖，均转成一维数组来计算 
static   int count_score(  char n[],   char chessColor)
{   
      char i = 1;//n[0]已经提前计算
	  char empty = 0;//空的位子
	  char number = 0;//连子的个数
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

//把当前局势所有连线都存到一维数组,然后算一遍分数 
static signed int evaluate_board(  char chessed[][BOARD_SIZE])//评估函数，评估局势
{
	  int AIScore=0;
	  int humScore=0;
      char i, j;
    signed char	 x, y;//如果是u8,x--,y--运算时可能溢出
      char n[BOARD_SIZE];
    memset(n, 0, sizeof(n));
	//横排 
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
	//纵排
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
	//上半正斜线(\)
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
	//下半正斜线
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
	//上半反斜线(/)
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
	//下半反斜线
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

//是否有邻居:两步之内是否有子存在(不论是对手还是自己的子都可以) 
static   int has_neighbors(int x, int y,  char chessed[][BOARD_SIZE])
{
	int s = 2;	//两步之内有子
	  char i = 0, j = 0;
	for (i = (x-s>0?x-s:0); i <= x+s&&i<BOARD_SIZE; i++)
		for (j = (y-s>0?y-s:0); j <= y+s&&j<BOARD_SIZE; j++)
			if (i !=  0 || j != 0)
				if (NULL_FLAG != (chessed[i][j]))
					return TRUE;
	return FALSE;
}

//还可以优化:将目标空子们先进行估分，然后排序,因为alpha-beta剪枝依赖于空子顺序.
//产生空子序列(可以下子的空位) 		
static void generate_point(chess_queue *queue,  char chessed[][BOARD_SIZE])	
{
    int i,j,k = 0;
	for (i=0; i<BOARD_SIZE; i++) 
	{   
		for (j=0; j<BOARD_SIZE; j++) 
		{      
            //有邻居的空子,做为可下子的队列
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

//alpha：表示目前为止上一层找到的最小数
//beta：表示目前为止上一层找到的最大数
//当min层（玩家）下棋时,考虑最坏的情况 
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
        chessed[x][y] = humChessColor;//尝试下一个子
    
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

//当max层（电脑AI）下棋时,考虑最好的情况 
static int max_alphabeta(signed char	 depth, chess_t chess,  char chessed[][BOARD_SIZE],signed int alpha, signed int beta)	
{
	signed int res = evaluate_board(chessed);
      char i, x, y;
    signed int tmpScore;
    signed int best = -WIN5;
    chess_queue queue;   
    //上一步是玩家下棋可能产生输赢
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
        chessed[x][y] = AIChessColor;//尝试下一个子
        tmpScore = min_alphabeta(depth-1, queue.chess[i], chessed,alpha, best>beta?best:beta);
        chessed[x][y] = NULL_FLAG;//恢复成空子
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

//极大极小值搜索depth步后的最优解 
static void chess_ai_minmax_alphabeta(chess_t *chess,char chessed[][BOARD_SIZE],signed char	depth)
{
	  char i = 0, k = 0;
      char x = 0, y = 0;
    signed int tmp = 0;
    signed int best = -WIN5;
    chess_queue option_queue; //待选的空子队列
    chess_queue sure_queue;//最合适的下子位置
	generate_point(&option_queue,chessed);
    
	for (i = 0; i < option_queue.len; i++) 
	{
        x = option_queue.chess[i].x;
        y = option_queue.chess[i].y;
	    chessed[x][y] = AIChessColor;    //将该子置AI选的颜色，防止后面递归时，再递归到
        tmp = min_alphabeta(depth-1, option_queue.chess[i],chessed, WIN5, -WIN5);        
		if (tmp == best) 
		{      
            sure_queue.chess[k].x = option_queue.chess[i].x;
            sure_queue.chess[k].y = option_queue.chess[i].y;
			sure_queue.len = k+1;
            k++;
        }
        //找到一个更好的分,把以前存的位子全部清除
		if (tmp > best) 
		{
			best = tmp;
            k = 0;
			sure_queue.len = 1;//清空终选队列
            sure_queue.chess[k].x = option_queue.chess[i].x;
            sure_queue.chess[k].y = option_queue.chess[i].y;
		}
		chessed[x][y] = NULL_FLAG;//恢复成空子
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
int AI_play_chess(  char chessed[][BOARD_SIZE],chess_t *aiChess)
{
    chess_t chess;
    chess.x = 0;
    chess.y = 0;
    //棋盘空的下棋,随机一个中间的子.(空子队列生成不了)
    if (isFirst) 
	{
        chess.x =(int)(rand()%5) + 5;
        chess.y =(int)(rand()%5) + 5; 
    } else 
	{
        chess_ai_minmax_alphabeta(&chess,chessed,gDeep);
    }
    isFirst = 0;
    //将得到的xy坐标赋值给aiChess
    aiChess->x = chess.x;
    aiChess->y = chess.y;
    return 0;
}