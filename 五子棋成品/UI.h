#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#define PACK_STRUCT __attribute__((packed))
#define board_size      15//棋盘大小为15x15
#define R 4
#define CHESS_PIECE_RADIUS	13
#define nullchess  0
#define whitechess 1
#define blackchess 2
#define check(x)    (((x)<0) || ((x)>=board_size))
//棋子
typedef struct chess_s {
    unsigned char x;
    unsigned char y;
} chess_t;

//玩家下棋
int hum_play_chess(int a,int b);
//赢棋检测
unsigned int is_win(unsigned char x, unsigned char y, unsigned char color);

