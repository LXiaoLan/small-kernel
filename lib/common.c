/*
 * =====================================================================================
 *
 *       Filename:  common.c
 *
 *    Description:  屏幕打印函数
 *
 *        Version:  1.0
 *        Created:  2013年10月11日 12时11分03秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  leiyu, leiyujike1107@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#include "common.h"

//向指定的端口写一个字节
void outb(u16int port, u8int value)
{
	asm volatile("outb %1, %0" : : "dN" (port), "a" (value));
}

//从指定端口读一个字节
u8int inb(u16int port)
{
	u8int ret;
	asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

//从指定端口读一个字
u16int inw(u16int port)
{
	u16int ret;
	asm volatile("inw %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

