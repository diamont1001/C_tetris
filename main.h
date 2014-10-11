/*
* main.h (1.0)
* 游戏资源声明
*
* By Eric-JR Chen
* diamont1001@163.com
* 2011-3-4
*/

#ifndef MAIN_H
#define MAIN_H

#include "JR_Cursor.h"
#include "JR_KeyBoard.h"
#include "JR_Random.h"
#include "JR_timer.h"
#include "tetris.h"

#define ROW 15
#define COL 10

extern BLOCK curBlock;  //当前方块
extern BLOCK nextBlock; //下一个方块
extern const SHAPE BLOCK_PATTERN[MAX_BLOCKS];
extern char BG_BOARD[ROW][COL+1]; //游戏背景标志，0：未被占用， 1：已被占用
extern int BLOCKS_TOP;  //当前blocks的最高点
extern int TotalScore;  //游戏分数

/* 游戏状态 */
typedef enum _STATUS{
    WELCOME, GAMING, DIE, PAUSE, STOP
}STATUS;

/* 方向 */
typedef enum _DIRECTION{
    NONE, LEFT, RIGHT, DOWN
}DIRECTION;

/* 屏幕元素 */
typedef struct _SHOWNODE{
    int x;
    int y;
    char text[3];
}SHOWNODE;


#endif
