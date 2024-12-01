#include <stdint.h>

#include "elf_header.h"
#include "section_header.h"

#define SECTION_HEADER_32BIT_SIZE	0x28

int parse_section_header(FILE* file, const int off, SectionHeader* header) {
	fseek(file, off, SEEK_SET); 
	if (fread(header, SECTION_HEADER_32BIT_SIZE, 1, file) == 0) {
		return FILE_NO_READ_PERM;	
	}

	// Checking for valid section type
	if ((header->type > SECTION_TYPE_DYNSYM &&
			header->type < SECTION_TYPE_INIT_ARRAY) ||
		(header->type > SECTION_TYPE_NUM &&
		 	header->type < SECTION_TYPE_LOOS)) {
		return SECTION_HEADER_INVALID_TYPE;
	}

	// Checking that addralign is a power of 2
	if ((header->addralign & (header->addralign - 1)) != 0) {
		return SECTION_HEADER_NOT_POWER_OF_2_ADDR_ALIGN;
	}

	return 1;
}

struct data_map {
	int i;
	char* s;
};

void print_section_header (const SectionHeader* h) {
	const struct data_map section_type[] = {
		{SECTION_TYPE_NULL, "unused"},
		{SECTION_TYPE_PROGBITS, "program data"},
		{SECTION_TYPE_SYMTAB, "symbol table"},
		{SECTION_TYPE_STRTAB, "string table"},
		{SECTION_TYPE_RELA, "relocation entries with addends"},
		{SECTION_TYPE_HASH, "symbol hash table"},
		{SECTION_TYPE_DYNAMIC, "dynamic linking information"},
		{SECTION_TYPE_NOTE, "notes"},
		{SECTION_TYPE_NOBITS, "program space with no data (bss)"},
		{SECTION_TYPE_REL, "relocation entries with no addends"},
		{SECTION_TYPE_SHLIB, "reserved"},
		{SECTION_TYPE_DYNSYM, "dynamic linker symbol table"},
		{SECTION_TYPE_INIT_ARRAY, "array of constructors"},
		{SECTION_TYPE_FINI_ARRAY, "array of destructors"},
		{SECTION_TYPE_PREINIT_ARRAY, "array of pre-constructors"},
		{SECTION_TYPE_GROUP, "section group"},
		{SECTION_TYPE_SYMTAB_SHNDX, "extended section of indices"},
		{SECTION_TYPE_NUM, "number of defined types"}
	}, section_flag[] = {
		{SECTION_FLAG_WRITE, "writable"},
		{SECTION_FLAG_ALLOC, "allocatable"},
		{SECTION_FLAG_EXECINSTR, "executable"},
		{SECTION_FLAG_MERGE, "mergeable"},
		{SECTION_FLAG_STRINGS, "strings"},
		{SECTION_FLAG_INFO_LINK, "info_link"},
		{SECTION_FLAG_LINK_ORDER, "link_order"},
		{SECTION_FLAG_OS_NONCONFORMING, "non-conforming"},
		{SECTION_FLAG_GROUP, "group"},
		{SECTION_FLAG_TLS, "tls"},
		{SECTION_FLAG_MASKOS, "os-specific"},
		{SECTION_FLAG_MASKPROC, "proc-specific"},
		{SECTION_FLAG_ORDERED, "solaris_ordered"},
		{SECTION_FLAG_EXCLUDE, "solaris_excluded"}
	}; 

	printf("\tSection name offset: %#010x\n", h->name);
	
	if (h->type >= SECTION_TYPE_LOOS) {
		printf("\tSection type: %#010x, os specific\n", h->type);
	} else {
		for (int i = 0; i < sizeof(section_type)/sizeof(struct data_map); i++) {
			if (h->type == section_type[i].i) {
				printf("\tSection type: %s\n", section_type[i].s);
				break;
			}
		}
	}

	printf("\tSection flags: ");
	for (int i = 0; i < sizeof(section_flag)/sizeof(struct data_map); i++) {
		if (h->flags & section_flag[i].i) {
			printf("%s ", section_flag[i].s);
		}
	}

	printf("\n\tSection vaddr: %#010x\n", h->addr);
	printf("\tSection offset: %#010x\n", h->offset);
	printf("\tSection size: %#010x\n", h->size);
	printf("\tAssociated section index: %#010x\n", h->link);
	printf("\tSection info: %#010x\n", h->info);
	printf("\tSection alignment: %#010x\n", h->addralign);
	if (h->entsize != 0) {
		printf("\tSection entry size: %#010x\n", h->entsize);
	} else {
		printf("\tSection entry size: variable\n");
	}
}
