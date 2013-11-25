/*
 * =====================================================================================
 *
 *       Filename:  timer.c
 *
 *    Description:  时钟中断(PIT相关)
 *
 *        Version:  1.0
 *        Created:  2013年10月24日 18时50分12秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  leiyu, leiyujike1107@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#include "timer.h"
#include "isr.h"
#include "idt.h"
#include "monitor.h"


u32int tick = 0;

void timer_callback(registers_t *regs)
{
	tick++;
	monitor_write("Tick: ");
	monitor_write_dec(tick, rc_black, rc_red);
	monitor_put('\n');
}

void init_timer(u32int frequency)
{
	register_interrupt_handler(IRQ0, timer_callback);

	u32int divisor = 1193180 / frequency;

	outb(0x43, 0x36);

	u8int l = (u8int)(divisor & 0xFF);
	u8int h = (u8int) ((divisor >> 8) & 0xFF);

	outb(0x40, l);
	outb(0x40, h);
}

