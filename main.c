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

	uint8_t* header;
	fread(header, 1, 52, exec);
	if (header == NULL) {
		fprintf(stderr, "Failed to read from file: `%s`\n", argv[1]);
		return -1;
	}

	for (int i = 0; i < 52; i++)
		putchar(header[i]);

	return 0;
}
