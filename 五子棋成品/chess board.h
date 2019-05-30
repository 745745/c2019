#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define GO_OK            (0)
#define GO_ERROR         (-1)
#define GO_WIN           (1)
#define PACK_STRUCT __attribute__((packed))
#define FALSE  0
#define TRUE   1 


#define BOARD_SIZE      15
#define FIVE_MARK_POINT_RADIUS 4
#define CHESS_PIECE_RADIUS	13
//黑白空三种类型
#define NULL_FLAG  0
#define WHITE_FLAG 1
#define BLACK_FLAG 2
#define check(x)    (((x)<0) || ((x)>=BOARD_SIZE))
//棋子
typedef struct chess_s 
{
      char x;
      char y;
} chess_t;

//初始化棋盘
void gobang_init(void);
//玩家下棋
int hum_play_chess(int a,int b);
//赢棋检测
  int is_win(  char x,   char y,   char color);

