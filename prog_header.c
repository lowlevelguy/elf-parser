#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "prog_header.h"

ProgHeader* parse_prog_header (const uint8_t* header_data) {
	ProgHeader* header = malloc(sizeof(*header));
	
	int offset = 0;

	// Getting segment type
	memcpy(&header->type, header_data+offset, 4);
	if (header->type > PROG_TYPE_HIPROC) {
		free(header);
		return PROG_HEADER_INVALID_TYPE;	
	}
	offset += 4;

	// Getting segment offset
	memcpy(&header->offset, header_data+offset, 4);
	offset += 4;

	// Getting segment vaddr
	memcpy(&header->vaddr, header_data+offset, 4);
	offset += 4;

	// Getting segment paddr
	memcpy(&header->paddr, header_data+offset, 4);
	offset += 4;

	// Getting segment size in file
	memcpy(&header->filesz, header_data+offset, 4);
	offset += 4;

	// Getting segment size in memory
	memcpy(&header->memsz, header_data+offset, 4);
	offset += 4;

	// Getting flags
	memcpy(&header->flags, header_data+offset, 4);
	if (header->flags > PROG_TYPE_TLS) {
		// checking if it's in the OS specific range
		if (header->flags >= PROG_TYPE_LOOS && header->flags <= PROG_TYPE_HIOS) {	
			header->flags = PROG_TYPE_OS_SPECIFIC;
		} else {
			// checking if it's in the processor specific range
			if (header->flags >= PROG_TYPE_LOPROC && header->flags <= PROG_TYPE_HIPROC) {	
				header->flags = PROG_TYPE_PROC_SPECIFIC;
			} else {
				free(header);
				return PROG_HEADER_INVALID_FLAG;
			}

		}
	}
	offset += 4;
	
	// Getting segment alignment
	memcpy(&header->align, header_data+offset, 4);
	if (header->align <= 1) {
		header->align = 0;
	} else {
		// check if it's not a power of 2
		if ((header->align & (header->align - 1)) != 0) {
			free(header);
			return PROG_HEADER_NOT_POWER_OF_2_ALIGN;
		}

		// check if the vaddr, offset and align equation doesn't hold
		if ((header->vaddr - header->offset) % header->align != 0) {
			free(header);
			return PROG_HEADER_MISMATCH_ALIGN_VADDR;
		}
	}

	return header;
}

struct data_map {
	int i;
	char* s;
};

void print_prog_header (const ProgHeader header) {
	const struct data_map prog_type[] = {
		{PROG_TYPE_NULL, "unused"},
		{PROG_TYPE_LOAD, "loadable"},
		{PROG_TYPE_DYNAMIC, "dynamic linking information"},
		{PROG_TYPE_INTERP, "interpreter information"},
		{PROG_TYPE_NOTE, "auxiliary information"},
		{PROG_TYPE_SHLIB, "reserved, non-ABI conformant"},
		{PROG_TYPE_PHDR, "program header table"},
		{PROG_TYPE_TLS, "thread local storage template"},
		{PROG_TYPE_OS_SPECIFIC, "OS specific"},
		{PROG_TYPE_PROC_SPECIFIC, "processor specific"}
	}, prog_flag[] = {
		{PROG_FLAG_X, "executable"},
		{PROG_FLAG_W, "writeable"},
		{PROG_FLAG_R, "readable"}
	};

	for (int i = 0; i < sizeof(prog_type)/sizeof(struct data_map); i++) {
		if (header.type == prog_type[i].i) {
			printf("\tSegment type: %s\n", prog_type[i].s);
			break;
		}
	}
	
	printf("\tSegment offset: %#x\n", header.offset);
	printf("\tSegment virtual address: %#x\n", header.vaddr);
	printf("\tSegment physical address: %#x\n", header.paddr);
	printf("\tSegment size in file image: %#x\n", header.filesz);
	printf("\tSegment size in memory: %#x\n", header.memsz);

	printf("\tSegment flags: ");
	for (int i = 0; i < sizeof(prog_flag)/sizeof(struct data_map); i++) {
		if (header.flags | prog_flag[i].i) {
			printf("%s ", prog_flag[i].s);
		}
	}

	printf("\n\tSegment alignment: %#x\n", header.align);
}
