#pragma once
#include "UI.h"
#define DEEP    2
typedef enum score_e {
    WIN5   = 100000,        //5连           
    ALIVE4 = 10000,         //活4
    ALIVE3 = 1000,          //活3
    DIE4   = 1000,          //死4
    ALIVE2 = 100,           //活2
    DIE3   = 100,           //死3
    DIE2   = 10,            //死2
    ALIVE1 = 10             //活1
} score;

//空子的序列，记录没有落子的棋盘
typedef struct chess_queue_s {
    chess_t chess[board_size*board_size];
    unsigned char len;
} chess_queue;
int AI_play_chess(unsigned char chessd[][board_size],chess_t *aiChess);
static int max_alphabeta(char depth, chess_t chess, unsigned char chessed[][board_size], int alpha, int beta);
static int min_alphabeta(char depth, chess_t chess, unsigned char chessed[][board_size], int alpha, int beta);
