/*
 * LCD graphics user interface library
 *
 * Author:    Chenglei Peng
 * Created:   Sep 01, 2010
 * Updated:   Oct 10, 2018
 * Copyright: (C) Chenglei Peng <pcl@nju.edu.cn>
 *
 * This program is an example for BBB LCD GUI library. You can modify it under the
 * terms of the GNU GPL v2.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "lcd.h"
#include "key.h"
#include <time.h>

#define PI 3.14159
unsigned short RED = RGB565(255, 0, 0);
unsigned short color;
int fd;
unsigned char *addr;
struct fb_var_screeninfo vinfo;
int style = 0;
char key; //键盘信号
Node *snk;
int food_x;
int food_y;

void initset(void)
{
	int fd;
	int screensize = 0;

	fd = open("/dev/fb0", O_RDWR);
	ioctl(fd, FBIOGET_VSCREENINFO, &vinfo);
	printf("Initializing screen parameters.\n");
	//      printf("bits_per_pixel:%d\n",vinfo.bits_per_pixel);
	//      printf("xres:%d\n",vinfo.xres);
	//      printf("yres:%d\n",vinfo.yres);

	screensize = vinfo.bits_per_pixel * vinfo.xres * vinfo.yres / 8;
	printf("screensize:%d\n", screensize);
	addr = (unsigned char *)mmap(0, screensize,
								 PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); //内存映射，映射screensize大小的内存空间
}

void clear(void)
{
	printf("Clearing screen.\n");
	bzero(addr, vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8); // addr内存的前n个字节清零，按字节所以除以8
}

void setcolor(unsigned char Red, unsigned char Green, unsigned char Blue)
{
	printf("Setting foreground color. Red=%d, Green=%d, Blue=%d\n", Red,
		   Green, Blue);
	color = RGB565(Red, Green, Blue);
}

void dot(int x, int y)
{
	*(addr + vinfo.bits_per_pixel / 8 * (x + y * vinfo.xres) + 0) = //该像素的第八位，因为addr是字节型的指针，所以一个字节一个字节赋值
		color & 0xFF;
	*(addr + vinfo.bits_per_pixel / 8 * (x + y * vinfo.xres) + 1) = //该像素的高八位
		(color >> 8) & 0xFF;
}

void setbkcolor(unsigned char Red, unsigned char Green, unsigned char Blue) //设置背景色
{
	int x, y;
	printf("Setting background color. Red=%d, Green=%d, Blue=%d\n", Red,
		   Green, Blue);
	color = RGB565(Red, Green, Blue);
	for (x = 0; x < vinfo.xres; x++)
	{
		for (y = 0; y < vinfo.yres; y++)
		{
			dot(x, y);
		}
	}
}

void setlinestyle(int sty)
{
	style = sty;
}

void circle(int x, int y, int r)
{
	int xtemp, ytemp, i;
	for (i = 0; i < 360; i++)
	{
		xtemp = x + r * cos(i * PI / 180);
		ytemp = y + r * sin(i * PI / 180);
		dot(xtemp, ytemp);
	}
}

void line(int x1, int y1, int x2, int y2)
{
	float k;
	int i;
	switch (style)
	{
	case 0:
	{
		if (x1 == x2)
		{
			if (y1 > y2)
			{
				i = y1;
				y1 = y2;
				y2 = i;
			}
			for (i = y1; i <= y2; i++)
				dot(x1, i);
		}
		else if (abs(x1 - x2) >= abs(y1 - y2))
		{
			k = 1.0 * (y2 - y1) / (x2 - x1);
			if (x1 > x2) // swap x,y
			{
				i = x1;
				x1 = x2;
				x2 = i;
				i = y1;
				y1 = y2;
				y2 = i;
			}
			for (i = x1; i <= x2; i++)
			{
				dot(i, (y1 + (i - x1) * k));
			}
		}
		else
		{
			k = 1.0 * (x2 - x1) / (y2 - y1);
			if (y1 > y2)
			{
				i = x1;
				x1 = x2;
				x2 = i;
				i = y1;
				y1 = y2;
				y2 = i;
			}
			for (i = y1; i <= y2; i++)
			{
				dot((x1 + (i - y1) * k), i);
			}
		}
	}
	case 1:
	{
		if (x1 == x2)
		{
			if (y1 > y2)
			{
				i = y1;
				y1 = y2;
				y2 = i;
			}
			for (i = y1; i <= y2; i += 2)
				dot(x1, i);
		}
		else if (abs(x1 - x2) >= abs(y1 - y2))
		{
			k = 1.0 * (y2 - y1) / (x2 - x1);
			if (x1 > x2) // swap x,y
			{
				i = x1;
				x1 = x2;
				x2 = i;
				i = y1;
				y1 = y2;
				y2 = i;
			}
			for (i = x1; i <= x2; i += 2)
			{
				dot(i, (y1 + (i - x1) * k));
			}
		}
		else
		{
			k = 1.0 * (x2 - x1) / (y2 - y1);
			if (y1 > y2)
			{
				i = x1;
				x1 = x2;
				x2 = i;
				i = y1;
				y1 = y2;
				y2 = i;
			}
			for (i = y1; i <= y2; i += 2)
			{
				dot((x1 + (i - y1) * k), i);
			}
		}
	}
	}
}

void rect(int x1, int y1, int x2, int y2)
{
	printf("Drawing a rectangle.\n");
	line(x1, y1, x1, y2);
	line(x1, y1, x2, y1);
	line(x1, y2, x2, y2);
	line(x2, y1, x2, y2);
}

void mysin(int length)
{
	int x, y = 0, ybefore = 0;
	printf("Drawing a sin curve.\n");
	for (x = 0; x < length; x++)
	{
		y = 100 * sin(x / 3.14 * 40) + 120;
		line(x, ybefore, x + 1, y);
		ybefore = y;
	}
}

void display_bmp(char *path)
{
	int file, x, y;
	char buf[3] = {0};
	file = open(path, O_RDONLY, 0666);
	if (file < 0)
	{
		perror("open");
		exit(1);
	}
	lseek(file, 54, SEEK_SET); //前54字节是一些配置信息
	for (y = 479; y >= 0; y--)
	{
		for (x = 0; x <= 799; x++)
		{
			read(file, buf, 3);
			color = RGB565(buf[2], buf[1], buf[0]);
			dot(x, y);
		}
	}
	close(file);
}

void printlcd(int x, int y, char *s) //显示字符串
{
	register int i, j, k;
	FILE *fpa, *fph;
	char buffera[16], bufferh[32];
	unsigned char qh, wh;
	unsigned long location;

	if ((fpa = fopen("./asc16", "rb")) == NULL)
		perror("open asc16");
	if ((fph = fopen("./hzk16", "rb")) == NULL)
		perror("open hzk16");
	printf("Printing string: %s\n", s);
	while (*s)
	{
		if ((char)(*s) > 0)
		{
			fseek(fpa, (*s) * 16, 0);
			fread(buffera, 1, 16, fpa);
			for (i = 0; i < 16; i++)
				for (j = 0; j < 8; j++)
				{
					if (((buffera[i] >> (7 - j)) & 0x01) !=
						0)
						dot(j + x, i + y);
				}
			s++; //英文字母一个字节
			x += 8;
		}
		else
		{
			qh = *s - 0xa0;
			wh = *(s + 1) - 0xa0;
			printf("wh=%d\n", wh);
			location = (94 * (qh - 1) + (wh - 1)) * 32L;
			fseek(fph, location, SEEK_SET);
			fread(bufferh, 32, 1, fph);
			for (i = 0; i < 16; i++)
				for (j = 0; j < 2; j++)
					for (k = 0; k < 8; k++)
						if (((bufferh[i * 2 + j] >>
							  (7 - k)) &
							 0x01) != 0)
							dot(x + 8 * j + k,
								y + i);
			s += 2; //一个汉字两个字节
			x += 16;
		}
	}
	fclose(fpa);
	fclose(fph);
}

void _draw_circle_8(int xc, int yc, int x, int y)
{
	dot(xc + x, yc + y);
	dot(xc - x, yc + y);
	dot(xc + x, yc - y);
	dot(xc - x, yc - y);
	dot(xc + y, yc + x);
	dot(xc - y, yc + x);
	dot(xc + y, yc - x);
	dot(xc - y, yc - x);
}

int draw_circle(int xc, int yc, int r)
{

	printf("Drawing a circle. radius=%d\n", r);
	if (xc + r < 0 || xc - r >= 800 || yc + r < 0 || yc - r >= 480)
		return 0;

	int x = 0, y = r, yi, d;
	d = 3 - 2 * r;

	while (x <= y)
	{
		for (yi = x; yi <= y; yi++)
			_draw_circle_8(xc, yc, x, yi);

		if (d < 0)
		{
			d = d + 4 * x + 6;
		}
		else
		{
			d = d + 4 * (x - y) + 10;
			y--;
		}
		x++;
	}
	return 1;
}
void snackgame(int len) //的初识长度
{
	snk = CreateEmptyList(vinfo.xres / 2, vinfo.yres / 2, len);
	draw_snack(snk);
	pthread_t t1;
	pthread_t t2; // xiancheng
	key = 'w';	  //初始化

	pthread_create(&t1, NULL, upadate, NULL);
	pthread_create(&t2, NULL, get_key, NULL);
	while (1);

}
void upadate()
{
	while (1)
	{
		clear();
		if (*(addr + vinfo.bits_per_pixel / 8 * (snk->xh + snk->yh * vinfo.xres) + 0) == color & 0xFF && *(addr + vinfo.bits_per_pixel / 8 * (snk->xh + snk->yh * vinfo.xres) + 1) == (color >> 8) & 0xFF)
		{
			printlcd(200, 200, "Game over!");
			break;
		}
		if (*(addr + vinfo.bits_per_pixel / 8 * (snk->xh + snk->yh * vinfo.xres) + 0) == RED & 0xFF && *(addr + vinfo.bits_per_pixel / 8 * (snk->xh + snk->yh * vinfo.xres) + 1) == (RED >> 8) & 0xFF)
		{
			food_gen();
			add_len(snk);
			clear();
		}
		draw_snack();
		draw_food();
		mov();
		sleep(0.5);
	}
}
void mov()
{
	int step = 4;
	switch (key)
	{
	case 'w':
		snk->yh = snk->yh - step;
		break;
	case 's':
		snk->yh = snk->yh + step;
		break;
	case 'a':
		snk->xh = snk->xh - step;
		break;
	default:
		snk->xh = snk->xh + step;
		break;
	}

	Node *p = snk;
	Node *temp;
	while (p->next != NULL)
	{
		temp = p;
		p = p->next;
	}
	if (temp->xh > p->xh)
		p->xh = p->xh + step;
	else if (temp->xh < p->xh)
		p->xh = p->xh - step;
	else if (temp->yh > p->yh)
		p->yh = p->yh + step;
	else if (temp->yh < p->yh)
		p->yh = p->yh - step;
}
void get_key()
{
	key = scanKeyboard();
}

void draw_snack()
{
	int lensum = 0;
	Node *p = snk;
	while (p->next != NULL)
	{
		line(p->xh, p->yh, p->next->xh, p->next->yh);
		p = p->next;
	}
}
void add_len() //尾部增加
{
	int step = 4; //一次增加的长度
	Node *p = snk;
	Node *temp;
	while (p->next != NULL)
	{
		temp = p;
		p = p->next;
	}
	if (temp->xh > p->xh)
		p->xh = p->xh - step;
	else if (temp->xh < p->xh)
		p->xh = p->xh + step;
	else if (temp->yh > p->yh)
		p->yh = p->yh - step;
	else if (temp->yh < p->yh)
		p->yh = p->yh + step;
}
void food_gen()
{
	srand((int)time(0));
	food_x = rand() % 800;
	food_y = rand() % 480;
}
void draw_food()
{
	setcolor(255,0,0);
	while (draw_circle(food_x, food_y, 8) == 0)
		food_gen(); //返回0则重新生成
	setcolor(255,255,255);
}