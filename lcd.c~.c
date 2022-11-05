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
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <linux/fb.h>
#include<fcntl.h>
#include<sys/mman.h>

#define PI 3.14159

unsigned short color;
int fd;
unsigned char *addr;
struct fb_var_screeninfo vinfo;
int style = 0;

void
initset (void)
{
  int fd;
  int screensize = 0;

  fd = open ("/dev/fb0", O_RDWR);
  ioctl (fd, FBIOGET_VSCREENINFO, &vinfo);
  printf ("Initializing screen parameters.\n");
//      printf("bits_per_pixel:%d\n",vinfo.bits_per_pixel);
//      printf("xres:%d\n",vinfo.xres);
//      printf("yres:%d\n",vinfo.yres);

  screensize = vinfo.bits_per_pixel * vinfo.xres * vinfo.yres / 8;
  printf ("screensize:%d\n", screensize);
  addr = (unsigned char *) mmap (0, screensize,
				 PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

}

void
clear (void)
{
  printf ("Clearing screen.\n");
  bzero (addr, vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8);
}

void
setcolor (unsigned char Red, unsigned char Green, unsigned char Blue)
{
  printf ("Setting foreground color. Red=%d, Green=%d, Blue=%d\n", Red, Green,
	  Blue);
  color = (Red << 11) | ((Green << 5) & 0x07E0) | (Blue & 0x1F);
}

void
dot (int x, int y)
{
  *(addr + vinfo.bits_per_pixel / 8 * (x + y * vinfo.xres) + 0) =
    color & 0xFF;
  *(addr + vinfo.bits_per_pixel / 8 * (x + y * vinfo.xres) + 1) =
    (color >> 8) & 0xFF;
}

void
setbkcolor (unsigned char Red, unsigned char Green, unsigned char Blue)
{
  int x, y;
  printf ("Setting background color. Red=%d, Green=%d, Blue=%d\n", Red, Green,
	  Blue);
  color = (Red << 11) | ((Green << 5) & 0x07E0) | (Blue & 0x1F);
  for (x = 0; x < vinfo.xres; x++)
    {
      for (y = 0; y < vinfo.yres; y++)
	{
	  dot (x, y);
	}
    }
}


void
setlinestyle (int sty)
{
  style = sty;
}

void
circle (int x, int y, int r)
{
  int xtemp, ytemp, i;
  for (i = 0; i < 360; i++)
    {
      xtemp = x + r * cos (i * PI / 180);
      ytemp = y + r * sin (i * PI / 180);
      dot (xtemp, ytemp);
    }
}

void
line (int x1, int y1, int x2, int y2)
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
	      dot (x1, i);
	  }
	else if (abs (x1 - x2) >= abs (y1 - y2))
	  {
	    k = 1.0 * (y2 - y1) / (x2 - x1);
	    if (x1 > x2)	//swap x,y 
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
		dot (i, (y1 + (i - x1) * k));
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
		dot ((x1 + (i - y1) * k), i);
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
	      dot (x1, i);
	  }
	else if (abs (x1 - x2) >= abs (y1 - y2))
	  {
	    k = 1.0 * (y2 - y1) / (x2 - x1);
	    if (x1 > x2)	//swap x,y
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
		dot (i, (y1 + (i - x1) * k));
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
		dot ((x1 + (i - y1) * k), i);
	      }
	  }

      }
    }
}

void
rect (int x1, int y1, int x2, int y2)
{
  printf ("Drawing a rectangle.\n");
  line (x1, y1, x1, y2);
  line (x1, y1, x2, y1);
  line (x1, y2, x2, y2);
  line (x2, y1, x2, y2);
}

void
mysin (int length)
{
  int x, y = 0, ybefore = 0;
  printf ("Drawing a sin curve.\n");
  for (x = 0; x < length; x++)
    {
      y = 100 * sin (x / 3.14 * 40) + 120;
      line (x, ybefore, x + 1, y);
      ybefore = y;
    }
}

void
display_bmp (char *path)
{
  int file, x, y;
  char buf[3] = { 0 };
  file = open (path, O_RDONLY, 0666);
  if (file < 0)
    {
      perror ("open");
      exit (1);
    }
  lseek (file, 54, SEEK_SET);
  for (y = 479; y >= 0; y--)
    {
      for (x = 0; x <= 799; x++)
	{
	  read (file, buf, 3);
	  color =
	    (unsigned short) ((buf[2] >> 3) << 11) | (unsigned
						      short) ((buf[1] >> 2) <<
							      5) | (unsigned
								    short)
	    (buf[0] >> 3);
	  dot (x, y);
	}
    }
  close (file);
}

void
printlcd (int x, int y, char *s)
{
  register int i, j, k;
  FILE *fpa, *fph;
  char buffera[16], bufferh[32];
  unsigned char qh, wh;
  unsigned long location;

  if ((fpa = fopen ("./asc16", "rb")) == NULL)
    perror ("open asc16");
  if ((fph = fopen ("./hzk16", "rb")) == NULL)
    perror ("open hzk16");
  printf ("Printing string: %s\n", s);
  while (*s)
    {
      if ((char) (*s) > 0)
	{
	  fseek (fpa, (*s) * 16, 0);
	  fread (buffera, 1, 16, fpa);
	  for (i = 0; i < 16; i++)
	    for (j = 0; j < 8; j++)
	      {
		if (((buffera[i] >> (7 - j)) & 0x01) != 0)
		  dot (j + x, i + y);
	      }
	  s++;
	  x += 8;
	}
      else
	{
	  qh = *s - 0xa0;
	  wh = *(s + 1) - 0xa0;
	  printf ("wh=%d\n", wh);
	  location = (94 * (qh - 1) + (wh - 1)) * 32L;
	  fseek (fph, location, SEEK_SET);
	  fread (bufferh, 32, 1, fph);
	  for (i = 0; i < 16; i++)
	    for (j = 0; j < 2; j++)
	      for (k = 0; k < 8; k++)
		if (((bufferh[i * 2 + j] >> (7 - k)) & 0x01) != 0)
		  dot (x + 8 * j + k, y + i);
	  s += 2;
	  x += 16;
	}
    }
  fclose (fpa);
  fclose (fph);
}

void
_draw_circle_8 (int xc, int yc, int x, int y)
{
  dot (xc + x, yc + y);
  dot (xc - x, yc + y);
  dot (xc + x, yc - y);
  dot (xc - x, yc - y);
  dot (xc + y, yc + x);
  dot (xc - y, yc + x);
  dot (xc + y, yc - x);
  dot (xc - y, yc - x);
}

void
draw_circle (int xc, int yc, int r)
{

  printf ("Drawing a circle. radius=%d\n", r);
  if (xc + r < 0 || xc - r >= 800 || yc + r < 0 || yc - r >= 480)
    return;

  int x = 0, y = r, yi, d;
  d = 3 - 2 * r;

  while (x <= y)
    {
      for (yi = x; yi <= y; yi++)
	_draw_circle_8 (xc, yc, x, yi);

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
}
