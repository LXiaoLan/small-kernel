/*
 * =====================================================================================
 *
 *       Filename:  common.h
 *
 *    Description:  定义typedef和一些全局的函数
 *
 *        Version:  1.0
 *        Created:  2013年10月11日 12时04分03秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  leiyu, leiyujike1107@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#ifndef COMMON_H
#define COMMON_H

#ifndef NULL
        #define NULL 0
#endif

#ifndef TRUE
        #define TRUE  1
        #define FALSE 0
#endif
//一些类型重命名，跨平台使用，这些类型为32-bit x86而写

typedef unsigned int   u32int;
typedef          int   s32int;
typedef unsigned short u16int;
typedef          short s16int;
typedef unsigned char  u8int;
typedef          char  s8int;

//向指定端口写一个字节
void outb(u16int port, u8int value);

//从指定端口读一个字节
u8int inb(u16int port);

//从指定端口读一个字
u16int inw(u16int port);

#endif
