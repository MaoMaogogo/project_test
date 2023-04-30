#define _CRT_SECURE_NO_WARNINGS 1
#include<sys/stat.h>
//#include<fcntl.h>
//#include<stdio.h>
//#include<unistd.h>
//#include<string.h>
//#include<sys/mman.h>
//

////画圆球
//int main()
//{
//	//1.打开fb0文件
//	int lcd_fd = 0;
//	lcd_fd = open("/dev/fb0", O_RDWR);
//	if (lcd_fd == -1)
//	{
//		printf("open fb0 failed!\n");
//	}
//
//	//2.准备颜色数据
//	int black = 0x00000000;//ARGB 调rgb透明
//	int red = 0x00ff0000;
//	int yellow = 0x00ffff00;
//	int white = 0x00000000;
//
//	//3.画圆球
//	int x = 0;
//	int y = 0;
//	int x0 = 400;
//	int y0 = 480;
//	int r = 120;
//
//	for (y = 0; y < 480; y++)
//	{
//		for (x = 0; x < 800; x++)
//		{
//			if ((x-x0)*(x-x0)+(y-y0)*(y-y0)<=r*r)//圆内染色
//			{
//				write(lcd_fd, &yellow, 4);
//			}
//			else
//			{
//				write(lcd_fd, &white, 4);
//			}
//		}
//	}
//
//	//4.关闭文件
//	close(lcd_fd);
//	return 0;
//}

#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h> 
#include<stdio.h>
#include<sys/mman.h>//window系统没有 
int main()
{
	int lcd_fd;
	lcd_fd = open("/dev/fb0", O_RDWR);
	if (lcd_fd == -1)
	{
		printf("open fb0 failed!\n");
		return -1;
	}

	int* p = mmap(NULL, 800 * 480 * 4, PROT_READ | PROT_WRITE, MAP_SHARED, lcd_fd, 0);//内存映射 

	int red = 0x00ff0000;
	int green = 0x0000ff00;
	int blue = 0x000000ff;
	int white = 0x00ffffff;//定义颜色数据 

	int x0 = 400, y0 = 240, r = 60;//圆心、半径 
	int x, y;
	int mode_x0 = 0, mode_y0 = 0;
	while (1)
	{


		for (y = 0; y < 480; y++)
		{
			for (x = 0; x < 800; x++)
			{
				if ((x - x0) * (x - x0) + (y - y0) * (y - y0) <= r * r)//判断是否在圆内 
				{
					*(p + 800 * y + x) = blue;
				}
				else
				{
					*(p + 800 * y + x) = red;
				}
			}
		}


		if (y0 == r)
		{
			mode_y0 = 1;
		}

		if (y0 == 479 - r)//下边 
		{
			mode_y0 = 0;

		}
		if (x0 == r)//左边 
		{
			mode_x0 = 1;
		}
		if (x0 == 799 - r)
		{
			mode_x0 = 0;
		}
		if (mode_x0 == 0) x0--;
		if (mode_x0 == 1) x0++;
		if (mode_y0 == 0)  y0--;
		if (mode_y0 == 1) y0++;
	}

	munmap(p, 800 * 480 * 4);//取消映射 

	close(lcd_fd);//关闭文件 
	return 0;


}
