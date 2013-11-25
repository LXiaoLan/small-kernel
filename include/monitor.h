/*
 * =====================================================================================
 *
 *       Filename:  monitor.h
 *
 *    Description:  定义显示器接口
 *
 *        Version:  1.0
 *        Created:  2013年10月11日 12时17分43秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  leiyu, leiyujike1107@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#ifndef INCLUDE_MONITOR_H_

#define INCLUDE_MONITOR_H_

#include "common.h"

typedef
enum real_color {
	rc_black = 0,
	rc_blue = 1,
	rc_green = 2,
	rc_cyan = 3,
	rc_red = 4,
	rc_magenta = 5,
	rc_brown = 6,
	rc_light_grey = 7,
	rc_dark_grey = 8,
	rc_light_blue = 9,
	rc_light_green = 10,
	rc_light_cyan = 11,
	rc_light_red = 12,
	rc_light_magenta = 13,
	rc_light_brown  = 14, 	// yellow
	rc_white = 15
}real_color_t;

// 清屏操作
void monitor_clear();

//输出一个字符
void monitor_put(char c);

// 屏幕输出一个字符(带颜色)
void monitor_putc_color(char c, real_color_t back, real_color_t fore);

// 屏幕打印一个以 \0 结尾的字符串(默认黑底白字)
void monitor_write(char *cstr);

// 屏幕打印一个以 \0 结尾的字符串(带颜色)
void monitor_write_color(char *cstr, real_color_t back, real_color_t fore);

// 屏幕输出一个十六进制的整型数
void monitor_write_hex(u32int n, real_color_t back, real_color_t fore);

// 屏幕输出一个十进制的整形数
void monitor_write_dec(u32int n, real_color_t back, real_color_t fore);

#endif           //MONITOR_H

