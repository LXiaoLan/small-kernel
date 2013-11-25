/*
 * =====================================================================================
 *
 *       Filename:  debug.h
 *
 *    Description:  调试相关的函数
 *
 *        Version:  1.0
 *        Created:  2013年10月21日 15时12分48秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Hurley (LiuHuan), liuhuan1992@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#ifndef INCLUDE_DEBUG_H_
#define INCLUDE_DEBUG_H_

#include "elf.h"
#include "panic.h"
#include "printk.h"

// 打印当前的段存器值
void print_cur_status();

#define assert(x)                                       	\
	do {                                                	\
		if (!(x)) {                                     \
			panic("assertion failed: %s", #x);      \
		}                                               \
	} while (0)

// 编译期间静态检测
#define static_assert(x)                                	\
	switch (x) { case 0: case (x): ; }


#endif 	// INCLUDE_DEBUG_H_
