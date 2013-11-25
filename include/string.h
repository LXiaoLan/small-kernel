/*
 * =====================================================================================
 *
 *       Filename:  string.h
 *
 *    Description:  字符串处理函数
 *
 *        Version:  1.0
 *        Created:  2013年10月23日 17时32分57秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  leiyu, leiyujike1107@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */


#ifndef INCLUDE_STRING_H_
#define INCLUDE_STRING_H_

#include "common.h"

void memcpy(u8int *dest, const u8int *src, u32int len);
//置数
void memset(void *dest, u8int val, u32int len);
//清0
void bzero(void *dest, u32int len);

int strcmp(const char *str1, const char *str2);

char *strcpy(char *dest, const char *src);

char *strcat(char *dest, const char *src);

int strlen(const char *src);

#endif
