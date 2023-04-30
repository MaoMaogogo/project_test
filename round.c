#define _CRT_SECURE_NO_WARNINGS 1
#include<sys/stat.h>
//#include<fcntl.h>
//#include<stdio.h>
//#include<unistd.h>
//#include<string.h>
//#include<sys/mman.h>
//

////��Բ��
//int main()
//{
//	//1.��fb0�ļ�
//	int lcd_fd = 0;
//	lcd_fd = open("/dev/fb0", O_RDWR);
//	if (lcd_fd == -1)
//	{
//		printf("open fb0 failed!\n");
//	}
//
//	//2.׼����ɫ����
//	int black = 0x00000000;//ARGB ��rgb͸��
//	int red = 0x00ff0000;
//	int yellow = 0x00ffff00;
//	int white = 0x00000000;
//
//	//3.��Բ��
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
//			if ((x-x0)*(x-x0)+(y-y0)*(y-y0)<=r*r)//Բ��Ⱦɫ
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
//	//4.�ر��ļ�
//	close(lcd_fd);
//	return 0;
//}

#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h> 
#include<stdio.h>
#include<sys/mman.h>//windowϵͳû�� 
int main()
{
	int lcd_fd;
	lcd_fd = open("/dev/fb0", O_RDWR);
	if (lcd_fd == -1)
	{
		printf("open fb0 failed!\n");
		return -1;
	}

	int* p = mmap(NULL, 800 * 480 * 4, PROT_READ | PROT_WRITE, MAP_SHARED, lcd_fd, 0);//�ڴ�ӳ�� 

	int red = 0x00ff0000;
	int green = 0x0000ff00;
	int blue = 0x000000ff;
	int white = 0x00ffffff;//������ɫ���� 

	int x0 = 400, y0 = 240, r = 60;//Բ�ġ��뾶 
	int x, y;
	int mode_x0 = 0, mode_y0 = 0;
	while (1)
	{


		for (y = 0; y < 480; y++)
		{
			for (x = 0; x < 800; x++)
			{
				if ((x - x0) * (x - x0) + (y - y0) * (y - y0) <= r * r)//�ж��Ƿ���Բ�� 
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

		if (y0 == 479 - r)//�±� 
		{
			mode_y0 = 0;

		}
		if (x0 == r)//��� 
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

	munmap(p, 800 * 480 * 4);//ȡ��ӳ�� 

	close(lcd_fd);//�ر��ļ� 
	return 0;


}
