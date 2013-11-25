
/*这个脚本告诉链接器如何设置内核映像。首先告诉链接器二进制文件的起始位置
 * 为start标志，然后是代码段部分起始地址为0x100000(1MB),接着是已经初始化
 * 的数据段和未初始化的数据段。已初始化的数据段中还包含.rodata(只读数据段
 * )，这些段是连续的。该格式是链接脚本的格式，并且每个段按照4096对齐.
 * 链接脚本解释文件
 */


ENTRY(start)
SECTIONS
{
  .text 0x100000 :
  {
    code = .; _code = .; __code = .;
    *(.text)
    . = ALIGN(4096);
  }

  .data :
  {
     data = .; _data = .; __data = .;
     *(.data)
     *(.rodata)
     . = ALIGN(4096);
  }

  .bss :
  {
    bss = .; _bss = .; __bss = .;
    *(.bss)
    . = ALIGN(4096);
  }

  .stab :
  {
	  stab = .; _stab = .; __stab = .;
	  *(.stab)
	  . = ALIGN(4096);
  }

  .stabstr :
  {
	  stabstr = .; _stabstr = .; __stabstr = .;
	  *(.stabstr)
	  . = ALIGN(4096);
  }

  end = .; _end = .; __end = .;

  /DISCARD/ : { *(.comment) * (.eh_frame)}
}
