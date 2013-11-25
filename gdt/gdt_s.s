;-------------------------------------------------
;                 将GDT地址装入 GDTR  
;------------------------------------------------

[GLOBAL gdt_flush]

gdt_flush:
         mov eax, [esp+4]                ;参数存入eax寄存器
	 lgdt [eax]                      ;加载到GDTR[修改原来GRUB设置]

	 mov ax, 0x10                    ;加载数据段描述符
	 mov ds, ax                      ;加载所有段寄存器 
	 mov es, ax
	 mov fs, ax
	 mov gs, ax
	 mov ss, ax
	 jmp 0x08:.flush                  ;远眺转，0x08是我们的代码段描述符
	                                  ;远眺目的是清空流水线并串行化处理器 
.flush:
         ret
.end
