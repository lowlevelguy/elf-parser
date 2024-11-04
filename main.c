#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "elf_header.h"
#include "prog_header.h"

int main (int argc, char** argv) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s [file]\n", argv[0]);
		return -1;
	}

	FILE* exec = fopen(argv[1], "r");
	if (exec == NULL) {
		fprintf(stderr, "Failed to open file: `%s`\n", argv[1]);
		return -1;
	}

	uint8_t* header = malloc(sizeof(uint8_t) * 52);
	fread(header, 1, 52, exec);
	if (header == NULL) {
		fprintf(stderr, "Failed to read from file: `%s`\n", argv[1]);
		return -1;
	}

	ELFHeader* elf_header = parse_elf_header(header);
	if ((long int)elf_header < 0) {
		switch ((long int)elf_header) {
			case (long int)ELF_HEADER_NOT_ELF:
				fprintf(stderr, "Error: file `%s` is not an ELF file.\n", argv[1]);
				break;
			case (long int)ELF_HEADER_NOT_32BITS:
				fprintf(stderr, "Error: file `%s` target architecture is not 32-bits. Only 32-bit ELF files are supported.\n", argv[1]);
				break;
			case (long int)ELF_HEADER_INVALID_ABI:
				fprintf(stderr, "Error: file `%s` invalid ABI.\n", argv[1]);
				break;
			case (long int)ELF_HEADER_INVALID_TYPE:
				fprintf(stderr, "Error: file `%s` invalid ELF type.\n", argv[1]);
				break;
		};
		return -1;
	} 

	print_elf_header(*elf_header);

	for (int i = 0; i < elf_header->phnum; i++) {
		printf("\n==========\nProg header %d:\n", i);
		
		fread(header, 1, elf_header->phentsize, exec);

		ProgHeader* prog_header = parse_prog_header(header);
		print_prog_header(*prog_header);
	}

	return 0;
}
