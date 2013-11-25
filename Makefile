#!Makefile
#
# --------------------------------------------------------
#
#    Leiyu 这个小内核的 Makefile
#    默认使用的C语言编译器是 GCC、汇编语言编译器是 nasm
#
# --------------------------------------------------------
#

# patsubst 处理所有在 C_SOURCES 字列中的字（一列文件名），如果它的 结尾是 '.c'，就用 '.o' 把 '.c' 取代
C_SOURCES = $(shell find . -name "*.c")
C_OBJECTS = $(patsubst %.c, %.o, $(C_SOURCES))
S_SOURCES = $(shell find . -name "*.s")
S_OBJECTS = $(patsubst %.s, %.o, $(S_SOURCES))

#定义变量（类似于C语言中的宏，其中的字符串都会被替代为相应的文本串）
CC = gcc
LD = ld
ASM = nasm

C_FLAGS = -c -Wall -m32 -ggdb -gstabs+ -nostdlib -nostdinc -fno-builtin -fno-stack-protector -I include
LD_FLAGS = -T tools/link.ld -m elf_i386
ASM_FLAGS = -f elf

#伪目标all来作为最终目标(生成多个可执行程序)
all: $(S_OBJECTS) $(C_OBJECTS) link

# The automatic variable `$<' is just the first prerequisite
# 自动变量:$@表示一个规则中的目标文件,$<表示规则中的第一个依赖文件名
#$表示使用变量或函数调用,如果在一条命令前加上@,则表示执行本行命令时不在屏幕上打印输出命令的内容
.c.o:
	@echo 编译代码文件 $< ...
	$(CC) $(C_FLAGS) $< -o $@   

.s.o:
	@echo 编译汇编文件 $< ...
	nasm $(ASM_FLAGS) $<

link:
	@echo 链接内核文件...
	ld $(LD_FLAGS) $(S_OBJECTS) $(C_OBJECTS)  -o kernel

#将目标"clean"声明为伪目录（将一个目录声明为伪目录的方法是将它作为特殊目标.PHONY的依赖）,接着是规则定义
.PHONY:clean
clean:                           
	$(RM) $(S_OBJECTS) $(C_OBJECTS) kernel

.PHONY:update_image
update_image:
	sudo mount a.img /mnt/kernel
	sudo cp kernel /mnt/kernel/kernel
	sleep 1
	sudo umount /mnt/kernel

.PHONY:mount_image
mount_image:
	sudo mount a.img /mnt/kernel

.PHONY:umount_image
umount_image:
	sudo umount /mnt/kernel

.PHONY:run
run:
	bochs -f tools/bochsrcrun

.PHONY:debug
debug:
	gnome-terminal -e "bochs -f tools/bochsrc"
	sleep 3
	cgdb -x tools/gdbinit



