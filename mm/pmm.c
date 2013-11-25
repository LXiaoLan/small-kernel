/*
 * =====================================================================================
 *
 *       Filename:  pmm.h
 *
 *    Description:  页内存管理
 *
 *        Version:  1.0
 *        Created:  2013年11月07日 21时59分49秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  leiyu, leiyujike1107@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#include "common.h"
#include "mm.h"
#include "panic.h"

//当前申请到的位置
u32int pmm_stack_loc = PMM_STACK_ADDR;

//内存申请的界限位置
u32int pmm_stack_max = PMM_STACK_ADDR;

//没有开启分页机制时的内存管理方案采用的管理指针
u32int pmm_location;

//是否开启内存分页
char mm_paging_active = 0;

void init_pmm(multiboot_t *mboot_ptr)
{
#ifdef SHOW_MEM_MAP
	// 打印 GRUB 提供的 由 BIOS 的反馈内存布局
	printk("mem_lower: 0x%X\n", mboot_ptr->mem_lower * 1024);
	printk("mem_upper: 0x%X\n\n", mboot_ptr->mem_upper * 1024);

	mmap_entry_t *mmap;
	printk("mmap_addr = 0x%X, mmap_length = 0x%X\n\n", (unsigned)mboot_ptr->mmap_addr, (unsigned)mboot_ptr->mmap_length);
	for (mmap = (mmap_entry_t *) mboot_ptr->mmap_addr;
		(unsigned long) mmap < mboot_ptr->mmap_addr + mboot_ptr->mmap_length; 
		mmap = (mmap_entry_t *)((unsigned long)mmap + mmap->size + sizeof(mmap->size))) {
	      printk("  size = 0x%X, base_addr = 0x%X%X,"
				      " length = 0x%X%X, type = 0x%X\n",
				      (unsigned)mmap->size,
				      (unsigned)mmap->base_addr_high,
				      (unsigned)mmap->base_addr_low,
				      (unsigned)mmap->length_high,
				      (unsigned)mmap->length_low,
				      (unsigned)mmap->type);
	}
	printk("\n");
#endif

	// 简单起见，内存 0～640 KB 是空闲的
	// 我们直接把原始的物理内存页管理起始地址设为 0
	pmm_location = 0;
}

void init_page_pmm(multiboot_t *mboot_ptr)
{	
	u32int i = mboot_ptr->mmap_addr;

	while (i < mboot_ptr->mmap_addr + mboot_ptr->mmap_length) {
		mmap_entry_t *map_entry = (mmap_entry_t *)i;
		
		// 如果是可用内存(按照协议，1 表示可用内存，其它数字指保留区域)
		// BIOS 探测出的可用内存是包含了我们的内核所在空间的，我们直接舍弃 1 MB 低端区域
		if (map_entry->type == 1 && map_entry->base_addr_low == 0x100000) {
			
			// 把这些内存段，按页存储到页管理栈里
			u32int j = map_entry->base_addr_low;

			// 我们的内核从 0x100000 开始加载，暂时我们粗略的递增 1 MB 跳过内核所在
			// 当然，这样的方法实在太过于简陋，暂且保留，我们是要改进的
			j += 0x100000;

			while (j < map_entry->base_addr_low + map_entry->length_low && j <= PMM_MAX) {
				pmm_free_page(j);
				j += 0x1000;
			}
		}
		// multiboot 规范中大小这项数据不包含指针自身的大小
		// 所以我们要再加上一个指针大小，真是奇怪的规范 - -
		i += map_entry->size + sizeof(u32int);
	}
}


u32int pmm_alloc_page()
{
	//根据是否开启分页决定对内存的管理策略
	if (mm_paging_active)
	{
		//确定栈地址没有下溢
		//在开启分页以后，需要先收集空闲内存放入管理栈才可以进一步申请内存
		//否则之后调用申请内存会出错
		if (pmm_stack_loc == PMM_STACK_ADDR)
		{
			panic("Error: Out of Merroy!");
		}
		//相当于出栈
		pmm_stack_loc -= sizeof(u32int);
		u32int *stack = (u32int *)pmm_stack_loc;

		return *stack;
	}

	pmm_location += 0x1000;
	return pmm_location;
}

void pmm_free_page(u32int p)
{
	if (p < pmm_location)
	{
		return;
	}

	//此处意为存储空闲页的内存页写满了
	//需要映射一页内存来存储索引
	if (pmm_stack_loc == pmm_stack_max)
	{
		map(pmm_stack_max, p, PAGE_PRESENT | PAGE_WRITE);
		pmm_stack_max += 4096;
	}
	else
	{
		//将这个空闲的内存页地址直接存入管理栈
		u32int *stack = (u32int *)pmm_stack_loc;
		*stack = p;
		pmm_stack_loc += sizeof(u32int);
	}
}

