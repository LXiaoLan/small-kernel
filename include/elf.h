/*
 * =====================================================================================
 *
 *       Filename:  elf.h
 *
 *    Description:  ELF 格式的部分定义
 *
 *        Version:  1.0
 *        Created:  2013年11月10日 17时01分22秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  leiyu, leiyujike1107@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#ifndef INCLUDE_ELF_H_
#define INCLUDE_ELF_H_

#include "common.h"
#include "multiboot.h"

#define ELF32_ST_TYPE(i) ((i)&0xf)

//ELF 格式区段头
typedef
struct elf_section_header_t
{
	u32int name;
	u32int type;
	u32int flags;
	u32int addr;
	u32int offset;
	u32int size;
	u32int link;
	u32int info;
	u32int addralign;
	u32int entsize;
}__attribute__((packed)) elf_section_header_t;

//ELF 格式符号
typedef
struct elf_symbol_t
{
	u32int name;
	u32int value;
	u32int size;
	u8int  info;
	u8int  other;
	u16int shndx;
} __attribute__((packed)) elf_symbol_t;

//ELF信息
typedef
struct elf_t
{
	elf_symbol_t *symtab;
	u32int        symtabsz;
	const char   *strtab;
	u32int        strtabsz;
} elf_t;

//从 multiboot_t 结构获取ELF信息
elf_t elf_from_multiboot(multiboot_t *mb);

//查看ELF的符号信息
const char *elf_lookup_symbol(u32int addr, elf_t *elf);
#endif
