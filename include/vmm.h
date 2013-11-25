/*
 * =====================================================================================
 *
 *       Filename:  vmm.h
 *
 *    Description:  虚拟内存管理
 *
 *        Version:  1.0
 *        Created:  2013年11月08日 10时17分06秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  leiyu, leiyujike1107@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */
#ifndef INCLUDE_VMM_H_
#define INCLUDE_VMM_H_

#include "common.h"
#include "idt.h"
#include "isr.h"

//页目录虚拟地址
#define PAGE_DIR_VIRTUAL_ADDR         0xFFBFF000

//页表虚拟地址
#define PAGE_TABLE_VIRTUAL_ADDR       0xFFC00000

//这两个宏用于页表和页目录的索引获取
#define PAGE_DIR_IDX(x)               ((u32int)x / 1024) 
#define PAGE_TABLE_IDX(x)             ((u32int)x % 1024)


/**
 * 分段和分页需要硬件上的支持，x86提供这项服务的是集成在 CPU 内部的 MMU 组件
 * 以下是页表项的宏定义
 */

/**
 * 如果P位被设置则表明该页在内存中，可以被映射到;如果Ｐ位等于0,则表示该为不*
 *在内存，这样会引发page fault,触发14号中断，然后CPU转去执行中断处理函数，会
 *将该页从外存调回主存
 */
#define PAGE_PRESENT   0x1

/**
 * R/W 位表示读写标识位。如果等于1,表示页面可以被读、写，如果为0,则页面只读
 * 当处理器运行在超级用户特权级时（0、1、2）时，则R/W位不起作用
 * 页目录项中的R/W位对其所映射的所有页面起作用
 */
#define PAGE_WRITE     0x2


/**
 * U/S是用户/超级用户标志
 * 如果为1,那么运行在任何特权级上的程序都可以访问该页面
 * 如果为0,那么页面只能被运行在超级用户特权级上的程序访问
 */
#define PAGE_USER      0x4

//页掩码，用于4KB对齐
#define PAGE_MASK     0xFFFFF000

//页目录数据结构类型
typedef u32int page_directory_t;

//初始化虚拟内存管理
void init_vmm();

//更换当前的页目录
void switch_page_directory(page_directory_t *pd);

//使用flags指出页权限， 把物理地址pa映射到虚拟地址va
void map(u32int va, u32int pa, u32int flags);

//如果虚拟地址 va 映射到物理地址则返回1
//同时如果 pa 不是空指针则把物理地址写入 pa 参数
char get_mapping(u32int va, u32int *pa);

//页错误中断函数处理
void page_fault(registers_t *regs);

#endif


