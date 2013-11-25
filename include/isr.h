/*
 * =====================================================================================
 *
 *       Filename:  isr.h
 *
 *    Description:  内部中断处理函数的头文件
 *
 *        Version:  1.0
 *        Created:  2013年10月23日 16时58分05秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  leiyu, leiyujike1107@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#include "common.h"

typedef struct registers
{
   u32int ds;                                     // 数据段选择子
   u32int edi, esi, ebp, esp, ebx, edx, ecx, eax; // 寄存器全入栈
   u32int int_no, err_code;                       // 中断错误代码和错误号
   u32int eip, cs, eflags, useresp, ss;           // 将进程的现场自己压栈
}  registers_t;

//定义中断处理函数指针
typedef void (*isr_t)(registers_t *);

//注册中断处理函数
void register_interrupt_handler(u8int n, isr_t handler);
