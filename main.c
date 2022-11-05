#include <stdio.h>
#include "lcd.h"

int main()
{
	initset();
	clear();
	setbkcolor(0, 0, 0);
	setcolor(255, 255, 255);
	snackgame(10);
	draw_circle(512, 300, 200);
	rect(200, 200, 824, 400);
	printlcd(200, 200, "Hello, NJU!");
	display_bmp("a1.bmp");
	return 0;
}
