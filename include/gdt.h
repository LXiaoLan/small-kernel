/*
 * =====================================================================================
 *
 *       Filename:  gdt.h
 *
 *    Description:  定义全局描述符表
 *
 *        Version:  1.0
 *        Created:  2013年10月22日 22时09分14秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  leiyu, leiyujike1107@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#ifndef INCLUDE_GDT_H_
#define INCLUDE_GDT_H_

#include "common.h"

//全局描述符（每个段描述符64位，前两个行是低32位）
struct gdt_entry_struct
{
	u16int limit_low;                 //16位段界限
	u16int base_low;                  //16位段基地址

	u8int  base_middle;               //8位段基址
	u8int  access;                    //段存在位、描述符特权级、描述符类型、描述符子类型 
	u8int  granularity;               //其他标志 
	u8int  base_high;                 //段基址
}__attribute__((packed));
typedef struct gdt_entry_struct gdt_entry_t;

//GDTR
struct gdt_ptr_struct
{
	u16int limit;                      //16位段限界
	u32int base;                       //32位段基址
}__attribute__((packed));
typedef struct gdt_ptr_struct gdt_ptr_t;

//初始化全局描述符表
void init_gdt();

//GDT加载到GDTR的函数（汇编实现）
void gdt_flush(u32int);

#endif

