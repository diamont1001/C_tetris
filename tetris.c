/*
* tetris.c (1.0)
* 游戏的实现
*
* By Eric-JR Chen
* diamont1001@163.com
* 2011-3-4
*/

#include "tetris.h"
#include "main.h"

BLOCK curBlock;             //当前方块
BLOCK nextBlock;            //下一个方块
int BLOCKS_TOP = 0;         //当前blocks的最高点
int TotalScore = 0;         //游戏分数
char BG_BOARD[ROW][COL+1];  //游戏背景标志，0：未被占用， 1：已被占用

/* 定义方块类型(共有19种) */
const SHAPE BLOCK_PATTERN[MAX_BLOCKS] = {
/* 0 - 3
 *  ■ □ □ □                ■ ■ ■ □                ■ ■ □ □                □ □ ■ □
 *  ■ □ □ □                ■ □ □ □                □ ■ □ □                ■ ■ ■ □
 *  ■ ■ □ □                □ □ □ □                □ ■ □ □                □ □ □ □
 *  □ □ □ □                □ □ □ □                □ □ □ □                □ □ □ □
 */
  {1, 2, 0x88, 0xc0, 1}, {2, 1, 0xe8, 0x00, 2}, {1, 2, 0xc4, 0x40, 3}, {2, 1, 0x2e, 0x00, 0},
/* 4 - 7
 *  □ ■ □ □                ■ □ □ □                ■ ■ □ □                ■ ■ ■ □
 *  □ ■ □ □                ■ ■ ■ □                ■ □ □ □                □ □ ■ □
 *  ■ ■ □ □                □ □ □ □                ■ □ □ □                □ □ □ □
 *  □ □ □ □                □ □ □ □                □ □ □ □                □ □ □ □
 */
  {1, 2, 0x44, 0xc0, 5}, {2, 1, 0x8e, 0x00, 6}, {1, 2, 0xc8, 0x80, 7}, {2, 1, 0xe2, 0x00, 4},
/* 8 - 9
 *  ■ □ □ □                □ ■ ■ □
 *  ■ ■ □ □                ■ ■ □ □
 *  □ ■ □ □                □ □ □ □
 *  □ □ □ □                □ □ □ □
 */
  {1, 2, 0x8c, 0x40, 9}, {2, 1, 0x6c, 0x00, 8},
/* 10 - 11
 *  □ ■ □ □                ■ ■ □ □
 *  ■ ■ □ □                □ ■ ■ □
 *  ■ □ □ □                □ □ □ □
 *  □ □ □ □                □ □ □ □
 */
  {1, 2, 0x4c, 0x80, 11}, {2, 1, 0xc6, 0x00, 10},
/* 12 - 15
 *  □ ■ □ □                ■ □ □ □                 ■ ■ ■ □                 □ ■ □ □
 *  ■ ■ ■ □                ■ ■ □ □                 □ ■ □ □                 ■ ■ □ □
 *  □ □ □ □                ■ □ □ □                 □ □ □ □                 □ ■ □ □
 *  □ □ □ □                □ □ □ □                 □ □ □ □                 □ □ □ □
 */
  {2, 1, 0x4e, 0x00, 13}, {1, 2, 0x8c, 0x80, 14}, {2, 1, 0xe4, 0x00, 15}, {1, 2, 0x4c, 0x40, 12},
/* 16 - 17
 *  ■ □ □ □                ■ ■ ■ ■
 *  ■ □ □ □                □ □ □ □
 *  ■ □ □ □                □ □ □ □
 *  ■ □ □ □                □ □ □ □
 */
  {0, 3, 0x88, 0x88, 17}, {3, 0, 0xf0, 0x00, 16},
/* 18
 *  ■ ■ □ □
 *  ■ ■ □ □
 *  □ □ □ □
 *  □ □ □ □
 */
  {2, 2, 0xcc, 0x00, 18}
};

/* 初始化当前两个BLOCKS */
void initBlocks(void){
    curBlock.x = FIRST_BLOCK_X;
    curBlock.y = 0;
    curBlock.index = JR_randomIn(18);

    nextBlock.x = FIRST_BLOCK_X;
    nextBlock.y = 0;
    nextBlock.index = JR_randomIn(18);
}

/* 生成新的方块 */
void getBlocks(int n){
    curBlock.x = nextBlock.x;
    curBlock.y = nextBlock.y;
    curBlock.index = nextBlock.index;

    if(n < 0 || n > 18)
        nextBlock.index = JR_randomIn(18);
    else
        nextBlock.index = n;
}

/* 初始化背景数据 */
void initBG(void){
    int i = 0, j = 0;
    for(i=0; i<ROW; i++)
        for(j=0; j<=COL; j++)
            BG_BOARD[i][j] = 0;
}

/* 设置背景数据
 * n=1:设置， n=0:清空
 */
void setBG(BLOCK b, int n){
    int i = 0, j = 0;
    int x = 0, y = 0;

    for(i=0; i<2; i++)
        for(j=0; j<8; j++)
            if(BLOCK_PATTERN[b.index].box[i] & (0x80>>j)){
                x = b.x + (j%4) * 2;
                y = b.y + (j/4) + 2*i;
                BG_BOARD[y][x/2] = n;
            }
}

/* 渲染游戏背景 */
void DrawBG(void){
    int i = 0, j = 0;
    JR_SetCursorStart();
    for(i=0; i<ROW; i++){
        for(j=0; j<COL; j++)
            if(!BG_BOARD[i][j])
                printf("%s", BG);
            else
                printf("%s", BODY);
        printf("\t\t\t\t\t\t\n");
    }
    JR_SetCursor(0, ROW+1);
}

/* 检查blocks是否合法 */
int checkBlock(BLOCK b){
    int i = 0, j = 0;
    //检测边界
    if( b.x < 0
     || (b.x + 8 -  BLOCK_PATTERN[b.index].offset_r*2) > COL*2
     || b.y > (ROW-4+BLOCK_PATTERN[b.index].offset_d)
       )
       return 0;

    //检测碰撞
    for(i=0; i<2; i++){
        for(j=0; j<8; j++){
            if( (BLOCK_PATTERN[b.index].box[i] & (0x80 >> j))   //转换后的BLOCK中该点为“实”
               && BG_BOARD[b.y+(j/4)+2*i][b.x/2+(j%4)]          //当前点，BG为“实”
                )
                return 0;
        }
    }
    return 1;
}

/*
 * 检测方块是否可以按照指定方向移动
 * 1:LEFT, 2:RIGHT, 3:DOWN
 */
int canMoveTo(int dir){
    BLOCK b;
    b.x = curBlock.x;
    b.y = curBlock.y;
    b.index = curBlock.index;
    switch(dir){
        case 1:     //LEFT
            b.x -= 2;
            break;
        case 2:     //RIGHT
            b.x += 2;
            break;
        case 3:     //DOWN
            b.y += 1;
            break;
    }
    setBG(curBlock, 0);
    if(checkBlock(b)){
        setBG(curBlock, 1);
        return 1;
    }
    setBG(curBlock, 1);
    return 0;
}

/*
 * 根据方向移动方块一格
 * 1:LEFT, 2:RIGHT, 3:DOWN
*/
int setBlocks(int dir){
    BLOCK b;
    b.x = curBlock.x;
    b.y = curBlock.y;
    b.index = curBlock.index;
    switch(dir){
        case 1:     //LEFT
            b.x -= 2;
            break;
        case 2:     //RIGHT
            b.x += 2;
            break;
        case 3:     //DOWN
            b.y += 1;
            break;
    }
    setBG(curBlock, 0);
    if(checkBlock(b)){
        curBlock.x = b.x;
        curBlock.y = b.y;
        setBG(curBlock, 1);
        return 1;
    }
    setBG(curBlock, 1);
    return 0;
}

/* 变形 */
void changeBlocks(void){
    BLOCK b;
    b.x = curBlock.x;
    b.y = curBlock.y;
    b.index = BLOCK_PATTERN[curBlock.index].next;
    setBG(curBlock, 0);
    if(checkBlock(b)){
        curBlock.index = b.index;
    }
    setBG(curBlock, 1);
}

/* 更新游戏结果(分数/GameOver) */
int checkResult(void){
    int i = 0, j = 0, k = 0;
    int flag = 1;
    int counter = 1;

    BLOCKS_TOP = BLOCKS_TOP > curBlock.y ? curBlock.y : BLOCKS_TOP;
    BLOCKS_TOP = BLOCKS_TOP > 0 ? BLOCKS_TOP : 0;

    //消行
    for(i=ROW-1; i>=BLOCKS_TOP; i--){
        flag = 1;
        for(j=0; j<COL; j++){
            if(!BG_BOARD[i][j]){
                flag = 0;
                j = COL;
            }
        }
        if(flag){
            for(j=i; j>BLOCKS_TOP; j--){
                for(k=0; k<COL; k++)
                    BG_BOARD[j][k] = BG_BOARD[j-1][k];
            }
            for(k=0; k<COL; k++)
                    BG_BOARD[BLOCKS_TOP][k] = 0;
            i = ROW;
            BLOCKS_TOP ++;
            //分数
            TotalScore += (counter ++) * 100;
        }
    }
    DrawBG();

    if(BLOCKS_TOP <= 0)
        return 0;

    return 1;
}
