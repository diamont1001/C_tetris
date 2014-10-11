/*
* tetris.h (1.0)
* 方块资源声明
*
* By Eric-JR Chen
* diamont1001@163.com
* 2011-3-4
*/

#ifndef TETRIS_H
#define TETRIS_H

#define MAX_BLOCKS  19      //19种方格形状
#define FIRST_BLOCK_X   6   //第一个出来的方块的X坐标

extern const char *BG;      //背景图案
extern const char *BODY;    //方块图案

/* 方块结构 */
typedef struct _SHAPE{
    int offset_d;   //下偏移量
    int offset_r;   //右偏移量
    char box[2];    //16位,每4位来表示一个方块的一行，如 "0x88,0xc0"
    int next;       //下个变形方块的下标
}SHAPE;

/* 一个方块 */
typedef struct _BLOCK{
    int x;      //左上角坐标
    int y;      //
    int index;  //方块类型（BLOCK_PATTERN的下标:0-18）
}BLOCK;


/* 初始化当前两个BLOCKS */
void initBlocks(void);

/* 生成新的方块
 * n为【0，18】时，block类型值为n
 * 否则block取随机 */
void getBlocks(int n);

/* 初始化背景数据 */
void initBG(void);

/* 设置背景数据
 * n=1:设置， n=0:清空
 */
void setBG(BLOCK b, int n);

/* 渲染游戏背景 */
void DrawBG(void);

/* 检查blocks是否合法 */
int checkBlock(BLOCK b);

/*
 * 检测方块是否可以按照指定方向移动
 * 1:LEFT, 2:RIGHT, 3:DOWN
 */
int canMoveTo(int dir);

/*
 * 根据方向移动方块一格
 * 1:LEFT, 2:RIGHT, 3:DOWN
*/
int setBlocks(int dir);

/* 变形 */
void changeBlocks(void);

/* 更新游戏结果(分数/GameOver) */
int checkResult(void);

#endif
