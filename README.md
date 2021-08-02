# C语言写的Windows字符界面俄罗斯方块

跟之前的贪吃蛇一样，都是用C语言写的，Windows字符界面的小游戏，没有调用到任何三方图形库。

![](https://raw.githubusercontent.com/diamont1001/C_tetris/master/docs/1.gif)

## 实现

都是调用到了之前封装好的几个资源:

- JR_timer
- JR_Random
- JR_KeyBoard
- JR_Cursor

- 运行环境：`Windows`
- 开发环境：`code::blocks + MinGW`

## 问题收集

有的朋友用VS或者VC打开，发现编译不过，是因为编译器不同，VS要求比较严格，声明和定义一定要放到前面，

解决方法：在 JR_timer.c 里面，把以下这句放到函数或者文件的前面就行了： `extern void mainTimer();`

## 附

 - [游戏试玩](http://download.csdn.net/download/diamont1001/3061555)
 - [原 CSDN 资源下载地址](http://download.csdn.net/download/diamont1001/3061541)
 
