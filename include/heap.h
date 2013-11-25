/*
 * =====================================================================================
 *
 *       Filename:  heap.h
 *
 *    Description:  简单的内核堆管理
 *
 *        Version:  1.0
 *        Created:  2013年11月18日 15时36分42秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  leiyu, leiyujike1107@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#ifndef INCLUDE_HEAD_H_
#define INCLUDE_HEAD_H_
#include "common.h"

//堆的开始地址
#define HEAP_START 0xD0000000

//堆的结束地址
#define HEAP_END   0xFFBFF000

//内存块管理结构
typedef
struct header
{
	struct header *prev;         //前后内存块管理结构指针
	struct header *next;       
	u32int allocated : 1;       //该内存块是否已经被申请
	u32int length : 31;         //当前内存块的长度
} header_t;

//初始化堆
void init_heap();

//内存申请
void *kmalloc(u32int len);

//内存的释放
void kfree(void *p);

//测试内核堆申请释放
void test_heap();

#endif

