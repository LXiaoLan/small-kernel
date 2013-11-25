/*
 * =====================================================================================
 *
 *       Filename:  printk.h
 *
 *    Description:  内核打印函数
 *
 *        Version:  1.0
 *        Created:  2013年11月10日 17时17分14秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  leiyu, leiyujike1107@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#ifndef INCLUDE_PRINTK_H_
#define INCLUDE_PRINTK_H_

#include "monitor.h"
#include "common.h"
#include "vargs.h"

//内核打印函数
void printk(const char *format, ...);

void printk_color(real_color_t back, real_color_t fore, const char *format, ...);

int vsprintf(char *buff, const char *format, va_list args);

#endif

