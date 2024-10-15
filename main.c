#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "elf_header.h"

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
	print_elf_header(*elf_header);

	return 0;
}
