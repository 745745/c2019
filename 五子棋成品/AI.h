#pragma once


//AI会用到棋盘定义的数据类型
#include "chess board.h"
#define DEEP    2 //最大搜索4层(但是4层比较慢，不建议用)

//计分板
typedef enum score_e 
{
    ALIVE4 = 10000,         //活4
    ALIVE3 = 1000,          //活3
    DIE4   = 1000,          //死4
    ALIVE2 = 100,           //活2
    DIE3   = 100,           //死3
    DIE2   = 10,            //死2
    ALIVE1 = 10             //活1
} score;

//空子的序列，记录没有落子的棋盘
typedef struct chess_queue 
{
    chess_t chess[BOARD_SIZE*BOARD_SIZE];
      char len;
} chess_queue;
//AI下棋,传入棋盘和选择的x，y
int AI_play_chess(  char chessd[][BOARD_SIZE],chess_t *aiChess);


