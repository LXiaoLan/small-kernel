/*
 * =====================================================================================
 *
 *       Filename:  vmm.c
 *
 *    Description:  虚拟内存管理
 *
 *        Version:  1.0
 *        Created:  2013年11月08日 11时00分20秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  leiyu, leiyujike1107@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#include "string.h"
#include "debug.h"
#include "mm.h"

//页目录地址
u32int *page_directory = (u32int *) PAGE_DIR_VIRTUAL_ADDR;

//页表地址
u32int *page_tables = (u32int *)PAGE_TABLE_VIRTUAL_ADDR;

//当前页目录地址
page_directory_t *current_directory;

void init_vmm()
{
	int i;
	u32int cr0;

	//注册页错误中断处理函数（14 是页故障的中断号）
	register_interrupt_handler(14, &page_fault);

	//创建一个页目录，此时未开启分页(pd指针指向一个页表)
	page_directory_t *pd = (page_directory_t *)pmm_alloc_page();

	//清空整个页目录的数据为0
	bzero(pd, 0x1000);

	//页目录第一项我们来映射最开始的4MB 内存
	pd[0] = pmm_alloc_page() | PAGE_PRESENT | PAGE_WRITE;

	u32int *pt = (u32int *)(pd[0] & PAGE_MASK);

	//乘以 0x1000 等价与左移12位
	//我们通过这个循环很巧妙的将物理内存的前4MB 映射并且启动分页后的虚拟地址相同
	//就是我们开启分页后，至少对前 4MB 内存的访问使用原先的物理地址就行
	//从某种意义上说，按照我们的设计，内核自己访问的虚拟地址和物理地址相同
	for (i = 0; i < 1024; i++)
	{
		pt[i] = i * 0x1000 | PAGE_PRESENT | PAGE_WRITE;
	}

	//我们再映射 4Ｇ 地址空间最后的地址
	pd[1022] = pmm_alloc_page() | PAGE_PRESENT | PAGE_WRITE;
	pt = (u32int *)(pd[1022] & PAGE_MASK);

	bzero(pt, 0x1000);

	//这样做的结果就是 0xFFBFF000这个页目录的虚拟地址正好被映射到了页目录实际的物理地址
	//0xFFBFF 拆开就是 1111111110 1111111111 正好是 1022 1023
	//即是第1022号页表的第1023,正好是物理页目录的地址
	pt[1023] = (u32int)pd | PAGE_PRESENT | PAGE_WRITE; //页目录的虚拟地址等于物理地址

	//页表的虚拟地址 0xFFC00000 就是 1111111111 0000000000 转换后就是1023 0
	//就是第1023号页表的第0项， 正好是内核的4MB 页表的地址
	pd[1023] = (u32int)pd | PAGE_PRESENT | PAGE_WRITE;  //页表的虚拟地址等于物理地址

	//以上的设置解决了分页机制模型下我们可以通过 0xFFBFF000访问到虚拟页目录
	//我们也可以用0cFFC00000 访问到0号页表的地址
	//而且，我们使用 0xFFC00000 这个虚拟地址页表部分的偏移就可以顺利
	//找到页目录每一项所指的页表的数据块了
	
	//设置号当前的页目录地址
	switch_page_directory(pd);

	//启用分页，将 cr0 寄存器的分页位置（第31位）设置为1
	asm volatile("mov %%cr0, %0" : "=r" (cr0));
	cr0 |= 0x80000000;
	asm volatile("mov %0, %%cr0" : : "r" (cr0));

	//PMM_STACK_ADDR 0xFF000000 物理内存管理的栈地址
	//必须在分页模式开启之前给该项分配内存， 否则就会直接引起异常
	//异常的原因就是调用了pmm_free_page 后触发的 map 调用，map会调用pmm_alloc_page
	//但是pmm_alloc_page 此时无法找到可用的内存，导致触发下溢条件 panic 结束
	//这一步找到的该地址应该在页目录项目
	u32int pt_idx = PAGE_DIR_IDX((PMM_STACK_ADDR >> 12));

	//给该页项目分配内存
	page_directory[pt_idx] = pmm_alloc_page() | PAGE_PRESENT | PAGE_WRITE;

	//给该内存页数据清0, 注意这里乘以1024相当于左移10位
	//注意这里的数组的类型是 4 字节类型， 所以计算数组地址偏移的时候乘以4
	//所以我们只要乘以1024, 一共乘以4096, 等一左移12位
	//这样就自动计算了目标地址的页表自身数据结构的偏移地址
	
	bzero((void *)&page_tables[pt_idx * 1024], 0x1000);

	//设置分页模式开启标志
	mm_paging_active = 1;
	
}

void switch_page_directory(page_directory_t *pd)
{
	current_directory = pd;
	asm volatile("mov %0, %%cr3" : : "r" (pd));
}

void map(u32int va, u32int pa, u32int flags)
{
	u32int virtual_page = va / 0x1000;
	u32int pt_idx = PAGE_DIR_IDX(virtual_page);

	//找到虚拟地址va 对应的描述符，如果它没被使用的话
	if (page_directory[pt_idx] == 0)
	{
		page_directory[pt_idx] = pmm_alloc_page() | PAGE_PRESENT | PAGE_WRITE;

		bzero((void *)&page_tables[pt_idx * 1024], 0x1000);
	}

	//创建好之后设置页表项，让这个地址所处的那一页内存指向目标物理内存页
	page_tables[virtual_page] = (pa & PAGE_MASK) | flags;

	//通知CPU更新页表缓存
	asm volatile("invlpg (%0)" : : "a" (va));
}

void unmap(u32int va)
{
	u32int virtual_page = va / 0x1000;

	page_tables[virtual_page] = 0;

	//通知 CPU 更新页表缓存
	asm volatile("invlpg (%0)" : : "a" (va));
}

char get_mapping(u32int va, u32int *pa)
{
	u32int virtual_page = va / 0x1000;
	u32int pt_idx = PAGE_DIR_IDX(virtual_page);

	//如果当前地址没有被映射直接返回0
	if (page_directory[pt_idx] == 0)
	{
		return 0;
	}

	//如果指针有效而且指针不为NULL，则返回地址
	if (page_tables[virtual_page] != 0 && pa)
	{
		*pa = page_tables[virtual_page] & PAGE_MASK;
		return 1;
	}
	return -1;
}


