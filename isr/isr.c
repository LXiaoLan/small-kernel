/*
 * =====================================================================================
 *
 *       Filename:  isr.c
 *
 *    Description:  中断处理程序
 *
 *        Version:  1.0
 *        Created:  2013年10月23日 16时57分11秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  leiyu, leiyujike1107@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */


#include "common.h"
#include "isr.h"
#include "monitor.h"
#include "printk.h"

//中断处理函数的指针数组
isr_t interrupt_handlers[256];

// 内部中断处理函数
/*void isr_handler(registers_t *regs)
{
   monitor_put('\n');
   monitor_write("recieved interrupt: ");
   monitor_write_dec(regs->int_no, rc_black, rc_red);
   monitor_put('\n');
}*/

//调用中断处理函数
void idt_handler(registers_t *regs)
{
	if (interrupt_handlers[regs->int_no])
	{
		interrupt_handlers[regs->int_no](regs);
	}
	else
	{
		printk_color(rc_black, rc_blue, "Unhandler interrupt: %d\n", regs->int_no);
	}
}

// IRQ 处理函数
void irq_handler(registers_t *regs)
{
	// 发送中断结束信号给 PICs
	// 按照我们的设置，从 32 号中断起为用户自定义中断
	// 因为单片的 Intel 8259A 芯片只能处理 8 级中断
	// 故大于等于 40 的终端号是由从片处理的
	if (regs->int_no >= 40) {
		// 发送重设信号给从片
		outb(0xA0, 0x20);
	}
	// 发送重设信号给主片
	outb(0x20, 0x20);

	if (interrupt_handlers[regs->int_no]) {
		isr_t handler = interrupt_handlers[regs->int_no];
		handler(regs);
	
	}
}

//注册一个中断处理函数
void register_interrupt_handler(u8int n, isr_t handler)
{
	interrupt_handlers[n] = handler;
}
