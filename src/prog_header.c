#include <stdio.h>

#include "map.h"
#include "prog_header.h"
#include "elf_header.h"

#define PROG_HEADER_32BIT_SIZE	32

int parse_prog_header (FILE* file, const int off, ProgHeader* header) {
	fseek(file, off, SEEK_SET);	
	if (fread(header, PROG_HEADER_32BIT_SIZE, 1, file) == 0) {
		return FILE_NO_READ_PERM;
	}

	if (header->type > PROG_TYPE_HIPROC) {
		return PROG_HEADER_INVALID_TYPE;	
	}

	// Getting flags
	uint32_t flags_mask = PROG_FLAG_R | PROG_FLAG_W | PROG_FLAG_X;
	if ((header->flags & flags_mask) == 0) {
		return PROG_HEADER_INVALID_FLAG;
	}
	
	// Getting segment alignment
	if (header->align <= 1) {
		header->align = 0;
	} else {
		// check if it's not a power of 2
		if ((header->align & (header->align - 1)) != 0) {
			return PROG_HEADER_NOT_POWER_OF_2_ALIGN;
		}

		// check if the vaddr, offset and align equation doesn't hold
		if ((header->vaddr - header->offset) % header->align != 0) {
			return PROG_HEADER_MISMATCH_ALIGN_VADDR;
		}
	}

	return 1;
}

void print_prog_header (const ProgHeader* h) {
	const struct map prog_type[] = {
		{PROG_TYPE_NULL, "unused"},
		{PROG_TYPE_LOAD, "loadable"},
		{PROG_TYPE_DYNAMIC, "dynamic linking information"},
		{PROG_TYPE_INTERP, "interpreter information"},
		{PROG_TYPE_NOTE, "auxiliary information"},
		{PROG_TYPE_SHLIB, "reserved, non-ABI conformant"},
		{PROG_TYPE_PHDR, "program header table"},
		{PROG_TYPE_TLS, "thread local storage template"},
	}, prog_flag[] = {
		{PROG_FLAG_X, "executable"},
		{PROG_FLAG_W, "writeable"},
		{PROG_FLAG_R, "readable"}
	};

	for (int i = 0; i < sizeof(prog_type)/sizeof(struct map); i++) {
		if (h->type == prog_type[i].i) {
			printf("\tSegment type: %s\n", prog_type[i].s);
			break;
		}
	}
	// otherwise, if OS or processor specific
	if (h->type >= PROG_TYPE_LOOS && h->type <= PROG_TYPE_HIOS) {
		printf("\tSegment type: %#08x, OS specific\n", h->type);
	} else if (h->type >= PROG_TYPE_LOPROC){
		printf("\tSegment type: %#08x, processor specific\n", h->type);
	}
	
	printf("\tSegment offset: %#x\n", h->offset);
	printf("\tSegment virtual address: %#x\n", h->vaddr);
	printf("\tSegment physical address: %#x\n", h->paddr);
	printf("\tSegment size in file image: %#x\n", h->filesz);
	printf("\tSegment size in memory: %#x\n", h->memsz);

	printf("\tSegment flags: ");
	for (int i = 0; i < sizeof(prog_flag)/sizeof(struct map); i++) {
		if (h->flags & prog_flag[i].i) {
			printf("%s ", prog_flag[i].s);
		}
	}

	printf("\n\tSegment alignment: %#x\n", h->align);
}
