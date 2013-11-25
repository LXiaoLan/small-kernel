
;内核从这里开始，这里还有根据GRUB Multiboot规范的一些定义
;

;符合Multiboot规范的OS规范的OS映像总是包含一个magic Multiboot头，这样引导程
;就不必理解种类繁多的可执行文件格式

MBOOT_PAGE_ALIGN    equ 1<<0          ; flags的0号位表示所有的引导模块将按页（4KB）边界对齐
MBOOT_MEM_INFO      equ 1<<1          ; flags的1号位通过Multiboot信息结构的mem_*域包括可用内存的信息（一般的ELF文件）
MBOOT_HEADER_MAGIC  equ 0x1BADB002    ; 多引导启动魔数,由规范定义

;注意：我们并没有加上第16位（MBOOT_AOUT_KLUDGE）,这意味着GRUB不会使用我们的符号表
;将flags的0号位和1号位都设置为1
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO

;域checksum是一个32位的无符号值，当与其它的magic域（也就是magic和flags相加时)，
;结果必须是32位的无符号值0（magic + flags + checksum = 0）

MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)


[BITS 32]                       ; 所有代码以32位方式编译

[GLOBAL mboot]                  ; 定义一些段符号
[EXTERN code]                 
[EXTERN bss]                   
[EXTERN end]                    

mboot:
  dd  MBOOT_HEADER_MAGIC        ;dd指的是往文件中写入4个字节的指令，grub会通过这个魔数判断映像是否被支持
  dd  MBOOT_HEADER_FLAGS        ; grub的一些加载时选项
  dd  MBOOT_CHECKSUM            ; 检测数值
   
  dd  mboot                     ; Location of this descriptor
  dd  code                      ; Start of kernel '.text' (code) section.
  dd  bss                       ; End of kernel '.data' section.
  dd  end                       ; End of kernel.
  dd  start                     ; Kernel entry point (initial EIP).

[GLOBAL start]                  ; 内核的入口
[EXTERN Ly_main]                   ; 内核C代码的入口

start:
  push    ebx                   ; 调用内核main函数的参数，struct multiboot *mboot_ptr

  ; 执行内核:
  cli                          ; 关闭中断，启动内核
  call Ly_main                    ; 调用main函数
  jmp $                        ; 进入死循环 
