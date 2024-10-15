#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "elf_header.h"

ELFHeader* parse_elf_header(uint8_t* header_data) {
	ELFHeader* header = malloc(sizeof(ELFHeader));
	int offset = 0;

	// Checking file type
	// If it's not an ELF file, then we return an error
	memcpy(header->ident.mag, header_data, 4);
	offset += 4;
	if (memcmp(header->ident.mag, "\x7f" "ELF", 4)) {
		return ELF_HEADER_NOT_ELF;
	}

	// Checking target architecture class (32 vs 64 bits)
	// For now, we're only supporting 32 bit programs, so any 64 bit ones will return an error
	if (header_data[offset] != ELF_BITS_32) {
		return ELF_HEADER_NOT_32BITS;
	}
	header->ident.ei_class = header_data[offset++];

	// Checking data endianness 
	if (header_data[offset] != ELF_ENCODING_LITTLE_ENDIAN && header_data[offset] != ELF_ENCODING_BIG_ENDIAN) {
		 header->ident.data = ELF_ENCODING_INVALID;
		 offset++;
	} else {
		header->ident.data = header_data[offset++];
	}

	// Checking ELF version
	if (header_data[offset] != ELF_VERSION_CURRENT) {
		header->ident.version = ELF_VERSION_INVALID;
		offset++;
	} else {
		header->ident.version = header_data[offset++];
	}

	// Checking target OS ABI
	if (header_data[offset] == 5 || header_data[offset] > ELF_ABI_STRATUS_TECHNOLOGIES_OPEN_VOS) {
		return ELF_HEADER_INVALID_ABI;	
	}
	header->ident.osabi = header_data[offset++];

	// Checking ABI version, interpretation is ABI dependant
	header->ident.abiversion = header_data[offset++];	

	// Skip 7 padding bytes
	// Wikipedia: Reserved padding bytes. Currently unused. Should be filled with zeros and ignored when read.
	offset += 7;

	// Checking file type
	memcpy(&header->type, header_data+offset, 2);
	if (header->type > ELF_TYPE_CORE) {
		return ELF_HEADER_INVALID_TYPE;	
	}
	offset += 2;

	// Checking target architecture
	// Only supported architectures at the moment are: ARM, AMD64, AARCH64, RISCV and "none"
	memcpy(&header->machine, header_data+offset, 2);
	if (header->machine != ELF_MACHINE_NONE &&
		header->machine != ELF_MACHINE_I386 &&
		header->machine != ELF_MACHINE_ARM &&
		header->machine != ELF_MACHINE_AMD64 &&
		header->machine != ELF_MACHINE_AARCH64 &&
		header->machine != ELF_MACHINE_RISCV) {
		return ELF_HEADER_UNSUPPORTED_MACHINE;
	}
	offset += 2;

	// Checking ELF version
	if (header_data[offset] != ELF_VERSION_CURRENT) {
		header->version = ELF_VERSION_INVALID;
		offset += 4;
	} else {
		memcpy(&header->version, header_data+offset, 4);
		offset += 4;
	}

	// Getting program entry point
	memcpy(&header->entry, header_data+offset, 4);
	offset += 4;

	// Getting program headers offset
	memcpy(&header->phoff, header_data+offset, 4);
	offset += 4;

	// Getting section headers offset
	memcpy(&header->shoff, header_data+offset, 4);
	offset += 4;

	// Getting flags 
	memcpy(&header->flags, header_data+offset, 4);
	offset += 4;

	// Getting elf header size
	memcpy(&header->ehsize, header_data+offset, 2);
	offset += 2;

	// Getting size per single program header 
	memcpy(&header->phentsize, header_data+offset, 2);
	offset += 2;

	// Getting number of program headers
	memcpy(&header->phnum, header_data+offset, 2);
	offset += 2;

	// Getting size per single section header 
	memcpy(&header->shentsize, header_data+offset, 2);
	offset += 2;

	// Getting number of section headers
	memcpy(&header->shnum, header_data+offset, 2);
	offset += 2;

	// Getting index of the names section
	memcpy(&header->shstrndx, header_data+offset, 2);
	offset += 2;

	return header;
}

void print_elf_header (ELFHeader h) {
	printf("ELF IDENTIFIER:\n");
	printf("\tFile format: %#x, %c%c%c\n", h.ident.mag[0], h.ident.mag[1], h.ident.mag[2], h.ident.mag[3]);
	printf("\tELF class: %#x\n", h.ident.ei_class);
	printf("\tELF version: %#x\n", h.ident.version);
	printf("\tTarget OS ABI: %#x\n", h.ident.osabi);
	printf("\tABI version: %#x\n", h.ident.abiversion);

	printf("\nBASIC FILE INFORMATION:\n");
	printf("\tFile type: %#x\n", h.type);
	printf("\tTarget architecture: %#x\n", h.machine);
	printf("\tProgram entry point: %#x\n", h.entry);
	printf("\tFlags: %#x\n", h.flags);
	printf("\tELF header size: %#x\n", h.ehsize);

	printf("\nPROGRAM:\n");
	printf("\tProgram headers offset: %#x\n", h.phoff);
	printf("\tProgram header size: %#x\n", h.phentsize);
	printf("\tProgram headers count: %#x\n", h.phnum);

	printf("\nSECTIONS:\n");
	printf("\tSection headers offset: %#x\n", h.shoff);
	printf("\tSection header size: %#x\n", h.shentsize);
	printf("\tSection headers count: %#x\n", h.shnum);
	printf("\tSection names location in section header table: %#x\n", h.shstrndx);
}

