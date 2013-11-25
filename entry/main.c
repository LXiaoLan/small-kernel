/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  内核的入口函数
 *
 *        Version:  1.0
 *        Created:  2013年09月26日 17时39分14秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  leiyu, leiyujike1107@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#include "monitor.h"
#include "idt.h"
#include "gdt.h"
#include "timer.h"
#include "multiboot.h"
#include "mm.h"
#include "elf.h"
#include "printk.h"
#include "heap.h"

//定义 elf 相关信息数据
elf_t kernel_elf;

int Ly_main(multiboot_t *mboot_ptr)
{
	//从 grub 提供的信息中获取到内核符号表和代码地址信息
	kernel_elf = elf_from_multiboot(mboot_ptr);

	//初始化全局段描述符表
	init_gdt();

	//初始化全局段描述符表
	init_idt();

	//清屏
	monitor_clear();
	printk_color(rc_black, rc_red, "**************************\n");
	printk_color(rc_black, rc_red, "*      Hello Linuxer!    *\n");
	printk_color(rc_black, rc_red, "**************************\n");
	printk_color(rc_black, rc_red, "This is a simple OS kernel, just for study.(Leiyu studying)\n");

	//往屏幕上打印字符串
//	monitor_write("Hello world!");

	//测试时钟中断
//	init_timer(50);

	//内嵌汇编测试内部中断
	asm volatile("int $0x3");
	asm volatile("int $0x4");

	//初始化物理内存管理
 	init_pmm(mboot_ptr);
	//初始化虚拟内存管理
	init_vmm();
	//初始化物理内存可用页
	init_page_pmm(mboot_ptr);
	//初始化内核态堆管理
	init_heap();
	//测试内核堆函数
	test_heap();

	//解除对可屏蔽中断的屏蔽
	asm volatile("sti");
	
	return 0;
}


