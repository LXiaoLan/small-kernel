/*
 * =====================================================================================
 *
 *       Filename:  debug.c
 *
 *    Description:  调试相关函数
 *
 *        Version:  1.0
 *        Created:  2013年11月10日 17时30分17秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  leiyu, leiyujike1107@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#include "debug.h"
#include "printk.h"

void print_cur_status()
{
	static int round = 0;
	u16int reg1, reg2, reg3, reg4;

	asm volatile ("mov %%cs, %0;"
		      "mov %%ds, %1;"
		      "mov %%es, %2;"
		      "mov %%ss, %3;"
		      : "=r"(reg1), "=m"(reg2), "=m"(reg3), "=m"(reg4));

	//打印当前的运行级别
	printk("%d: @ring %d\n", round, reg1, reg1 & 0x3);
	printk("%d:  cs = %x\n", round, reg1);
	printk("%d:  ds = %x\n", round, reg2);
	printk("%d:  es = %x\n", round, reg3);
	printk("%d:  ss = %x\n", round, reg4);
	++round;
}

