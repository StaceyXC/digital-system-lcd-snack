#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<sys/ioctl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<unistd.h>
#include<linux/fb.h>
#include "snack.h"

#define PI 3.14159

typedef unsigned short TUINT16;

#define RGB565(R, G, B) \
    (((TUINT16) ((R) >> 3)) << 11) + (((TUINT16) ((G) >> 2)) << 5) + ((TUINT16) ((B) >> 3)) //保留每一个颜色高位，损失信息少

#define COLOR_BLACK RGB565(0, 0, 0)

#define COLOR_DRED RGB565(64, 0, 0)
#define COLOR_DGREEN RGB565(0, 64, 0)
#define COLOR_DBLUE RGB565(0, 0, 64)

#define COLOR_DYELLOW RGB565(64, 64, 0)
#define COLOR_DPURPLE RGB565(64, 0, 64)
#define COLOR_DDIAN RGB565(0, 64, 64)

#define COLOR_GRAY RGB565(64, 64, 64)

#define COLOR_RED RGB565(255, 0, 0)
#define COLOR_GREEN RGB565(0, 255, 0)
#define COLOR_BLUE RGB565(0, 0, 255)

#define COLOR_YELLOW RGB565(255, 255, 0)
#define COLOR_PURPLE RGB565(255, 0, 255)
#define COLOR_DIAN RGB565(0, 255, 255)

#define COLOR_WHITE RGB565(255, 255, 255)

void initset(void);
void clear(void);
void dot(int, int);
void setcolor(unsigned char, unsigned char, unsigned char);
void setbkcolor(unsigned char, unsigned char, unsigned char);
void line(int, int, int, int);
void rect(int, int, int, int);
void circle(int, int, int);
void display_bmp(char *);
void printlcd(int, int, char *);
void _draw_circle_8(int, int, int, int);
void snackgame(int); //贪吃蛇
void snack_move(int ); //move函数
int draw_snack( );//画蛇函数
void add_len();//给蛇增加长度
void food_gen();//随机生成食物
void update();
void mov();
void get_key();
void draw_food();