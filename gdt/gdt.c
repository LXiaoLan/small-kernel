/*
 * =====================================================================================
 *
 *       Filename:  gdt.c
 *
 *    Description:  全局描述符表
 *
 *        Version:  1.0
 *        Created:  2013年10月22日 22时27分20秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  leiyu, leiyujike1107@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#include "gdt.h"

//全局描述符表的长度
#define GDT_LEN 5

//段描述符的初始化
static void gdt_set_gate(s32int, u32int, u32int, u8int, u8int);

//全局描述符表的定义
gdt_entry_t gdt_entries[GDT_LEN];  

//GDTR
gdt_ptr_t   gdt_ptr;               


//初始化全局描述符表
void init_gdt()
{
	//全局描述符界限就是表内最后一个字节的偏移量，因为第一个字节的偏移量是从0开始的
	gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
	gdt_ptr.base = (u32int)&gdt_entries;

	//采用平坦模式
	gdt_set_gate(0, 0, 0, 0, 0);                      //空段
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);       //代码段
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);       //数据段
        gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);       //用户模式数据段
	gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);       //用户模式代码段

	gdt_flush((u32int)&gdt_ptr);
}

static void gdt_set_gate(s32int num, u32int base, u32int limit, u8int access, u8int gran)
{
	gdt_entries[num].base_low = (base & 0xFFFF);
	gdt_entries[num].base_middle = (base >> 16) & 0xFF;
	gdt_entries[num].base_high = (base >> 24) & 0xFF;

	gdt_entries[num].limit_low = (limit & 0xFFFF);
	gdt_entries[num].granularity = (limit >> 16) & 0x0F;

	gdt_entries[num].granularity |=  gran & 0xF0;
	gdt_entries[num].access = access;
}


