#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "elf_header.h"
#include "prog_header.h"
#include "section_header.h"

int main (int argc, char** argv) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s [file]\n", argv[0]);
		return -1;
	}

	FILE* obj = fopen(argv[1], "r");
	if (obj == NULL) {
		fprintf(stderr, "Failed to open file: `%s`\n", argv[1]);
		return -1;
	}

	ELFHeader elf_header;
	switch (parse_elf_header(obj, &elf_header)) {
		case FILE_NO_READ_PERM:
			fprintf(stderr, "Error: file `%s` contents could not be read.\n", argv[1]);
			return -1;
		case ELF_HEADER_NOT_ELF:
			fprintf(stderr, "Error: file `%s` is not an ELF file.\n", argv[1]);
			return -1;
		case ELF_HEADER_NOT_32BITS:
			fprintf(stderr, "Error: file `%s` target architecture is not 32-bits. Only 32-bit ELF files are supported.\n", argv[1]);
			return -1;
		case ELF_HEADER_INVALID_ABI:
			fprintf(stderr, "Error: file `%s` invalid ABI.\n", argv[1]);
			return -1;
		case ELF_HEADER_INVALID_TYPE:
			fprintf(stderr, "Error: file `%s` invalid ELF type.\n", argv[1]);
			return -1;
	};

	print_elf_header(&elf_header);

	ProgHeader* prog_headers = malloc(sizeof(ProgHeader) * elf_header.phnum);
	for (int i = 0; i < elf_header.phnum; i++) {
		printf("\n==========\nProg header %d:\n", i);
	
		switch(parse_prog_header(obj, elf_header.phoff + elf_header.phentsize * i, prog_headers + i)) {
			case FILE_NO_READ_PERM:
				fprintf(stderr, "Error: file %s contents could not be read.\n", argv[1]);
				return -1;
			case PROG_HEADER_INVALID_TYPE:
				fprintf(stderr, "Error: invalid program header type.\n");
				return -1;
			case PROG_HEADER_INVALID_FLAG:
				fprintf(stderr, "Error: invalid program header flag.\n");
				return -1;
			case PROG_HEADER_NOT_POWER_OF_2_ALIGN:
				fprintf(stderr, "Error: program header not properly aligned.\n");
				return -1;
			case PROG_HEADER_MISMATCH_ALIGN_VADDR:
				fprintf(stderr, "Error: program header alignment and vaddr mismatched.\n");
				return -1;
		};
		print_prog_header(prog_headers+i);
	}

	SectionHeader* section_headers = malloc(sizeof(SectionHeader) * elf_header.shnum);
	for (int i = 0; i < elf_header.shnum; i++) {
		printf("\n==========\nSection header %d:\n", i);
		switch(parse_section_header(obj, elf_header.shoff + elf_header.shentsize * i, section_headers + i)) {
			case FILE_NO_READ_PERM:
				fprintf(stderr, "Error: file %s contents could not be read.\n", argv[1]);
				return -1;
			case SECTION_HEADER_INVALID_TYPE:
				fprintf(stderr, "Error: invalid section header type.\n");
				return -1;
			case SECTION_HEADER_NOT_POWER_OF_2_ADDR_ALIGN:
				fprintf(stderr, "Error: section header not properly aligned.\n");
				return -1;
		};
		print_section_header(section_headers+i);
	}

	return 0;
}
