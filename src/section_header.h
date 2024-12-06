#pragma once

#include <stdio.h>
#include <stdint.h>

#define SECTION_HEADER_INVALID_TYPE					-1
#define SECTION_HEADER_NOT_POWER_OF_2_ADDR_ALIGN	-2

enum {
	SECTION_TYPE_NULL,				// unused
	SECTION_TYPE_PROGBITS,			// program data
	SECTION_TYPE_SYMTAB,			// symbol table
	SECTION_TYPE_STRTAB,			// string table
	SECTION_TYPE_RELA,				// relocation entries with addends
	SECTION_TYPE_HASH,				// symbol hash table
	SECTION_TYPE_DYNAMIC,			// dynamic linking information
	SECTION_TYPE_NOTE,				// notes
	SECTION_TYPE_NOBITS,			// program space with no data (bss)
	SECTION_TYPE_REL,				// relocation entries, no addends
	SECTION_TYPE_SHLIB,				// reserved
	SECTION_TYPE_DYNSYM,			// dynamic linker symbol table
	SECTION_TYPE_INIT_ARRAY = 0xe,	// array of constructors
	SECTION_TYPE_FINI_ARRAY,		// array of destructors
	SECTION_TYPE_PREINIT_ARRAY,		// array of pre-constructors
	SECTION_TYPE_GROUP,				// section group
	SECTION_TYPE_SYMTAB_SHNDX,		// extended section of indices
	SECTION_TYPE_NUM,				// number of defined types
	SECTION_TYPE_LOOS = 0x60000000 	// start of OS-specific
};

enum {
	SECTION_FLAG_WRITE = 1,					// writable
	SECTION_FLAG_ALLOC = 1 << 1,			// occupies memory during execution
	SECTION_FLAG_EXECINSTR = 1 << 2,		// executable
	SECTION_FLAG_MERGE = 1 << 4,			// might be merged
	SECTION_FLAG_STRINGS = 1 << 5,			// contains null-terminated strings
	SECTION_FLAG_INFO_LINK = 1 << 6,		// sh_info contains SHT index
	SECTION_FLAG_LINK_ORDER = 1 << 7,		// preserve order after combining
	SECTION_FLAG_OS_NONCONFORMING = 1 << 8,	// non-standard os-specific handling required
	SECTION_FLAG_GROUP = 1 << 9,			// section header is part of a group
	SECTION_FLAG_TLS = 1 << 10,				// section holds thread-local data
	SECTION_FLAG_MASKOS = 0x0ff00000,		// os-specific
	SECTION_FLAG_MASKPROC = 0xf0000000,		// processor specific
	SECTION_FLAG_ORDERED = 0x40000000,		// special ordering requirement (solaris)
	SECTION_FLAG_EXCLUDE = 0x80000000		// section is excluded unless references or allocated (solaris)
};

// Forcing compiler to not pad the struct to avoid undefined behaviour when reading from file into it
typedef struct __attribute__((packed)) {
	uint32_t name,			// offset to a string in the .shstrtab section that represents the section's name
			 type,			// section header type 
			 flags,			// section header flags
			 addr,			// vaddr of the section in memory, for sections that are loaded
			 offset,		// offset of section in file image
			 size,			// size in bytes of section, may be 0
			 link,			// contains section index of an associated section
			 info,			// contains extra information about the section
			 addralign,		// contains the section's required alignment, must be a power of 2
			 entsize		// size in bytes of each entry, for sections that contain fixed size entries. otherwise set to 0
} SectionHeader;

int parse_section_header(FILE* file, const int off, SectionHeader* header);
void print_section_header (const SectionHeader* h);
