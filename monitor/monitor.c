/*
 * =====================================================================================
 *
 *       Filename:  monitor.c
 *
 *    Description:  屏幕显示函数的实现
 *
 *        Version:  1.0
 *        Created:  2013年10月11日 12时23分01秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  leiyu, leiyujike1107@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#include "monitor.h"

//VGA(视频图形矩阵)的显示缓冲的其实地址 0xB8000
static u16int *video_memory = (u16int *)0xB8000;

//屏幕光标的坐标
static u8int cursor_x = 0;
static u8int cursor_y = 0;

//移动硬件指针
static void move_cursor()
{
	//屏幕的宽度为80个字符
	u16int cursorLocation = cursor_y*80 + cursor_x;
	outb(0x3D4, 14);                 
	outb(0x3D5, cursorLocation >> 8);              //取高8位
	outb(0x3D4, 15);
	outb(0x3D5, cursorLocation);                    
}

//实现屏幕上文本的滚动
static void scroll()
{
	u8int attributeByte = (0 /*black*/ << 4) | (15 /*white*/ &0x0F);
	u16int blank = 0x20  /*space*/ | (attributeByte << 8);

	if(cursor_y >= 25)
	{
		int i;
		for (i = 0*80; i < 24*80; i++)
		{
			video_memory[i] = video_memory[i+80];
		}

		for (i = 24*80; i < 25*80; i++)
		{
			video_memory[i] = blank;
		}
		cursor_y = 24;
	}
}

//向屏幕输出一个字符
void monitor_put(char c)
{
	u8int backColour = 0;
	u8int foreColour = 15;

	u8int attributeByte = (backColour << 4) | (foreColour & 0x0F);

	u16int attribute = attributeByte << 8;
	u16int *location;

	if (c == 0x08 && cursor_x)
	{
		cursor_x--;
	}
	else if (c == 0x09)
	{
		cursor_x = (cursor_x + 8) & ~(8 - 1);
	}
	else if (c == '\r')
	{
		cursor_x = 0;
	}
	else if (c == '\n')
	{
		cursor_x = 0;
		cursor_y++;
	}
	else if (c >= ' ')
	{
		location = video_memory + (cursor_y *80 + cursor_x);
		*location = c | attribute;
		cursor_x++;
	}

	if (cursor_x >= 80)
	{
		cursor_x = 0;
		cursor_y++;
	}

	scroll();
	move_cursor();
}

void monitor_clear()
{
	u8int attributeByte = (0 << 4) | (15 & 0x0F);
	u16int blank = 0x20 | (attributeByte << 8);

	int i;
	for (i = 0; i < 80*25; i++)
	{
		video_memory[i] = blank;
	}

	cursor_x = 0;
	cursor_y = 0;
	move_cursor();
}


// 屏幕输出一个字符(带颜色)
void monitor_putc_color(char c, real_color_t back, real_color_t fore)
{
	u8int back_color = (u8int)back;
	u8int fore_color = (u8int)fore;

	u8int attribute_byte = (back_color << 4) | (fore_color & 0x0F);
	u16int attribute = attribute_byte << 8;

	// 0x08 是 退格键 的 ASCII 码
	// 0x09 是 tab 键 的 ASCII 码
	if (c == 0x08 && cursor_x) {
	      cursor_x--;
	} else if (c == 0x09) {
	      cursor_x = (cursor_x+8) & ~(8-1);
	} else if (c == '\r') {
	      cursor_x = 0;
	} else if (c == '\n') {
		cursor_x = 0;
		cursor_y++;
	} else if (c >= ' ') {
		video_memory[cursor_y*80 + cursor_x] = c | attribute;
		cursor_x++;
	}

	// 每 80 个字符一行，满80就必须换行了
	if (cursor_x >= 80) {
		cursor_x = 0;
		cursor_y ++;
	}

	// 如果需要的话滚动屏幕显示
	scroll();

	// 移动硬件的输出 "光标"
	move_cursor();
}

// 屏幕打印一个以 \0 结尾的字符串(默认黑底白字)
void monitor_write(char *cstr)
{
	while (*cstr) {
	      monitor_putc_color(*cstr++, rc_black, rc_white);
	}
}

// 屏幕打印一个以 \0 结尾的字符串(带颜色)
void monitor_write_color(char *cstr, real_color_t back, real_color_t fore)
{
	while (*cstr) {
	      monitor_putc_color(*cstr++, back, fore);
	}
}

// 屏幕输出一个十六进制的整型数
void monitor_write_hex(u32int n, real_color_t back, real_color_t fore)
{
	int tmp;
	char noZeroes = 1;

	monitor_write_color("0x", back, fore);

	int i;
	for (i = 28; i >= 0; i -= 4) {
		tmp = (n >> i) & 0xF;
		if (tmp == 0 && noZeroes != 0) {
		      continue;
		}
		noZeroes = 0;
		if (tmp >= 0xA) {
		      monitor_putc_color(tmp-0xA+'a', back, fore);
		} else {
		      monitor_putc_color(tmp+'0', back, fore);
		}
	}
}

// 屏幕输出一个十进制的整形数
void monitor_write_dec(u32int n, real_color_t back, real_color_t fore)
{
	if (n == 0) {
		monitor_putc_color('0', back, fore);
		return;
	}

	u32int acc = n;
	char c[32];
	int i = 0;
	while (acc > 0) {
		c[i] = '0' + acc % 10;
		acc /= 10;
		i++;
	}
	c[i] = 0;

	char c2[32];
	c2[i--] = 0;

	int j = 0;
	while(i >= 0) {
	      c2[i--] = c[j++];
	}

	monitor_write_color(c2, back, fore);
}
