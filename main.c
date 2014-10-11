/*
* main.c (1.0)
* 游戏主文件
*
* By Eric-JR Chen
* diamont1001@163.com
* 2011-3-4
*/

#include <stdio.h>
#include <stdlib.h>
#include "main.h"

//游戏资源
const char *BG = "▓";//"■";   //背景图案
const char *BODY = "□";//"▓"; //方块图案

static STATUS sys_gs;       //游戏状态
static int sys_level = 1;   //等级(1-9)
static DIRECTION sys_dir;   //控制走向

//控制屏幕刷新
static int JR_isRefresh = 0;
//纹理缓存
static SHOWNODE showBuff[ROW*COL*2];
static int BuffIndex = 0;


/* 将一个 NODE放到纹理缓冲区 */
void showInBuff(SHOWNODE buffer){
    showBuff[BuffIndex].x = buffer.x;
    showBuff[BuffIndex].y = buffer.y;
    strcpy(showBuff[BuffIndex].text, buffer.text);
    BuffIndex ++;
}

/* SHOW WELCOME */
void showWelcome(void){
    printf("\n\n");
    printf("\t\t************ JR_Tetris 1.0.1 *************\n");
    printf("\t\t*       Welcome to Tetris Gameing.       *\n");
    printf("\t\t*        By Eric-JR Chen 2011-3          *\n");
    printf("\t\t*          diamont1001@163.com           *\n");
    printf("\t\t*           【Enter】 Start              *\n");
    printf("\t\t*           【 Esc 】 Esc                *\n");
    printf("\t\t******************************************\n");
}

/* 1 : 渲染当前block到buff
 * 0 : 擦除当前block
 */
void showBlock(int n){
    int i = 0, j = 0;
    //移位查看SHAPE
    for(i=0; i<2; i++)
        for(j=0; j<8; j++)
            if(BLOCK_PATTERN[curBlock.index].box[i] & (0x80>>j)){
                SHOWNODE node;
                node.x = curBlock.x + (j%4) * 2;
                node.y = curBlock.y + (j/4) + 2*i;
                if(n){
                    strcpy(node.text, BODY);
                }else{
                    strcpy(node.text, BG);
                }
                showInBuff(node);
            }
}

/* 显示下一个BLOCKS */
void showNextBlock(int x, int y){
    int i = 0, j = 0;
    JR_SetCursor(x, y);
    printf("Next : \t\t");
    JR_SetCursor(x, y+1);
    printf("\t\t\t");
    JR_SetCursor(x, y+2);
    printf("\t\t\t");
    JR_SetCursor(x, y+3);
    printf("\t\t\t");
    //移位查看SHAPE
    for(i=0; i<2; i++)
        for(j=0; j<8; j++)
            if(BLOCK_PATTERN[nextBlock.index].box[i] & (0x80>>j)){
                SHOWNODE node;
                node.x = x + 8 + (j%4)*2;
                node.y = y + i*2 + j/4;
                strcpy(node.text, BG);
                showInBuff(node);
            }
}

/* 显示其他提示 */
void showTips(){
    int x = COL * 2 + 1;
    int y = 5;
    JR_SetCursor(x, y++);
    printf("Level: %d", sys_level);
    JR_SetCursor(x, y++);
    printf("Score: %d", TotalScore);
    JR_SetCursor(x, y++);
    switch(sys_gs){
        case GAMING:
            printf("status: GAMING\t");
        break;
        case PAUSE:
            printf("status: PAUSE\t");
        break;
        case DIE:
            printf("status: GAME OVER\t");
        break;
        case STOP:
            printf("status: STOPED\t");
        break;
    }
}

void setGameSpeed(void){
    stopTimer();
    setTimer(500 - sys_level*40);
    startTimer();
}

/* 更新分数 */
void UpdateScore(void){
    int level = 1 + TotalScore / 5000;  //5000分一个LEVEL
    sys_level = level < 10 ? level : 9; //一共9个LEVEL
    setGameSpeed();
}

/* 方块向下走一格，否则重新生成新的方块 */
void UpdateBlocks(){
    if(!canMoveTo(DOWN)){
        if(!checkResult()){
            sys_gs = DIE;
        }else{  //生成新的方块
            getBlocks(-1);
            showNextBlock(COL*2+1, 1);
        }
        UpdateScore();
        showTips();
    }else {     //blocks向下走一格
        showBlock(0);
        setBlocks(DOWN);
        showBlock(1);
    }
    sys_dir = NONE;
}

/* 屏幕渲染 */
void Render(){
    int i = 0;
    JR_isRefresh = 0;
    switch(sys_gs){
        case WELCOME:
            showWelcome();
            break;
        case GAMING:
            for(i=0; i<BuffIndex; i++){
                JR_SetCursor(showBuff[i].x, showBuff[i].y);
                printf("%s", showBuff[i].text);
            }
            BuffIndex = 0;
            break;
        case DIE:
            sys_gs = STOP;
            break;
        case PAUSE:
            JR_SetCursor(0, ROW);
            printf("Pause! Press 【Enter】 to Continue.\t\t\t\t");
            break;
        case STOP:
            JR_SetCursor(0, ROW);
            printf("Game over! Press 【Enter】 to Restart or 【Esc】 to Esc.\t\t");
            break;
    }
    JR_SetCursor(0, ROW+1);
}

/* 初始化游戏数据，开始游戏 */
void startGame(void){
    BuffIndex = 0;
    BLOCKS_TOP = ROW - 1;
    sys_gs = GAMING;
    sys_dir = NONE;
    sys_level = 1;
    TotalScore = 0;
    setGameSpeed();

    initBG();
    DrawBG();

    initBlocks();
    showBlock(1);

    showNextBlock(COL*2+1, 1);
    showTips();

    JR_isRefresh = 1;

    JR_SetCursor(0, ROW);
    printf("\t\t\t\t\t\t\t");
}

void init(void){
    sys_gs = WELCOME;
    JR_isRefresh = 1;
}

void UpdateKeys(int ms){
    static int ticks = 0;
    int t = JR_GetTicks();
    if(t - ticks < ms) return ;

    ticks = t;
    if(JR_AnyKeys()){
        sys_dir = NONE;
        if(JR_IsKeyDown(JR_KEY_ESC)){   //Esc
            sys_gs = STOP;
            exit(0);
        }else if(JR_IsKeyDown(JR_KEY_ENTER)){   //Pause,Resume
            if(sys_gs == WELCOME || sys_gs == STOP){
                startGame();
            }else if(sys_gs == GAMING){
                sys_gs = PAUSE;
                showTips();
            }else if(sys_gs == PAUSE){
                JR_SetCursor(0, ROW);
                printf("\t\t\t\t\t\t\t");
                sys_gs = GAMING;
                showTips();
            }
        }else if((sys_gs == GAMING) && (JR_IsKeyDown(JR_KEY_UP) || JR_IsKeyDown('w') || JR_IsKeyDown('W'))){     //变形
            showBlock(0);   //擦除原来blocks
            changeBlocks();
            showBlock(1);   //重画
        }else if((sys_gs == GAMING) && (JR_IsKeyDown(JR_KEY_DOWN) || JR_IsKeyDown('s') || JR_IsKeyDown('S'))){   //下
            UpdateBlocks();
            showBlock(1);
        }else if((sys_gs == GAMING) && (JR_IsKeyDown(JR_KEY_LEFT) || JR_IsKeyDown('a') || JR_IsKeyDown('A'))){   //左
            sys_dir = LEFT;
            showBlock(0);
            setBlocks(sys_dir);
            showBlock(1);
        }else if((sys_gs == GAMING) && (JR_IsKeyDown(JR_KEY_RIGHT) || JR_IsKeyDown('d') || JR_IsKeyDown('D'))){  //右
            sys_dir = RIGHT;
            showBlock(0);
            setBlocks(sys_dir);
            showBlock(1);
        }
        JR_isRefresh = 1;
    }
}

/* 每个interval更新一次 */
void mainTimer(void){
    if(sys_gs == GAMING){
        UpdateBlocks(); //处理自动下跌情况
        JR_isRefresh = 1;
    }
}

int main()
{
    init();
    while(1){
        UpdateKeys(11);
        if(JR_isRefresh){ //按需刷新屏幕
            Render();
        }
        ontimer();
    }
    return 0;
}
