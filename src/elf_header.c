#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "elf_header.h"

// returns 1 on success
// otherwise, returns 0 or negative number depending on the type of the error
int parse_elf_header(FILE* const file, ELFHeader* const header) {
	uint8_t header_data[52];
	if (fread(header_data, 52, 1, file) == 0) {
		return FILE_NO_READ_PERM;
	}
	int offset = 0;

	// Checking file type
	// If it's not an ELF file, then we return an error
	memcpy(header->ident.mag, header_data, 4);
	offset += 4;
	if (memcmp(header->ident.mag, "\x7f" "ELF", 4)) {
		free(header);
		return ELF_HEADER_NOT_ELF;
	}

	// Checking target architecture class (32 vs 64 bits)
	// For now, we're only supporting 32 bit programs, so any 64 bit ones will return an error
	if (header_data[offset] != ELF_BITS_32) {
		free(header);
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
		free(header);
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
		free(header);
		return ELF_HEADER_INVALID_TYPE;	
	}
	offset += 2;

	// Checking target architecture
	// Only supported architectures at the moment are: ARM, AMD64, AARCH64, and RISCV
	memcpy(&header->machine, header_data+offset, 2);
	if (header->machine != ELF_MACHINE_I386 &&
		header->machine != ELF_MACHINE_ARM &&
		header->machine != ELF_MACHINE_AMD64 &&
		header->machine != ELF_MACHINE_AARCH64 &&
		header->machine != ELF_MACHINE_RISCV) {
		header->machine = ELF_MACHINE_UNSUPPORTED;
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

	return 1;
}

struct data_map {
	int i;
	char* s;
};

void print_elf_header (const ELFHeader header) {
	// Mapping each field value to its corresponding interpretation
	const struct data_map elf_class[] = {
		{ELF_BITS_INVALID, "invalid"},
		{ELF_BITS_32, "32 bit"},
		{ELF_BITS_64, "64 bit"}
	},
	elf_encoding[] = {
		{ELF_ENCODING_INVALID, "invalid"},
		{ELF_ENCODING_LITTLE_ENDIAN, "little endian"},
		{ELF_ENCODING_BIG_ENDIAN, "big endian"}
	},
	elf_osabi[] = {
		{ELF_ABI_SYSTEM_V, "System V"},
		{ELF_ABI_HP_UX, "HP UX"},
		{ELF_ABI_NET_BSD, "NetBSD"},
		{ELF_ABI_LINUX, "Linux"},
		{ELF_ABI_GNU_HURD, "GNU Hurd"},
		{ELF_ABI_SOLARIS, "Solaris"},
		{ELF_ABI_AIX, "AIX"},
		{ELF_ABI_IRIX, "IRIX"},
		{ELF_ABI_FREE_BSD, "FreeBSD"},
		{ELF_ABI_TRU64, "Tru64"},
		{ELF_ABI_NOVELL_MODESTO, "Novell Modesto"},
		{ELF_ABI_OPEN_BSD, "OpenBSD"},
		{ELF_ABI_OPEN_VMS, "OpenVMS"},
		{ELF_ABI_NONSTOP_KERNEL, "NonStop Kernel"},
		{ELF_ABI_AROS, "AROS"},
		{ELF_ABI_FENIX_OS, "Fenix OS"},
		{ELF_ABI_NUXI_CLOUD_ABI, "Nuxi CloudABI"},
		{ELF_ABI_STRATUS_TECHNOLOGIES_OPEN_VOS, "Stratus Technologies OpenVOS"},
	},
	elf_type[] = {
		{ELF_TYPE_NONE, "none"},
		{ELF_TYPE_REL, "relocatable"},
		{ELF_TYPE_EXEC, "executable"},
		{ELF_TYPE_DYN, "shared object file"},
		{ELF_TYPE_CORE, "core dump"}
	},
	elf_machine[] = {
		{ELF_MACHINE_UNSUPPORTED, "unknown"},
		{ELF_MACHINE_I386, "i386"},
		{ELF_MACHINE_ARM, "arm"},
		{ELF_MACHINE_AMD64, "amd64/x86_64"},
		{ELF_MACHINE_AARCH64, "aarch64"},
		{ELF_MACHINE_RISCV, "riscv"}
	};


	printf("ELF IDENTIFIER:\n");
	printf("\tFile format: %#x, %c%c%c\n", header.ident.mag[0], header.ident.mag[1], header.ident.mag[2], header.ident.mag[3]);
	
	for (int i = 0; i < sizeof(elf_class)/sizeof(struct data_map); i++) {
		if (header.ident.ei_class == elf_class[i].i) {
			printf("\tELF class: %s\n", elf_class[i].s);
			break;
		}
	}
	
	for (int i = 0; i < sizeof(elf_encoding)/sizeof(struct data_map); i++) {
		if (header.ident.data == elf_encoding[i].i) {
			printf("\tData encoding: %s\n", elf_encoding[i].s);
			break;
		}
	}

	printf("\tELF version: %s\n", header.ident.version ? "current" : "invalid");
	
	for (int i = 0; i < sizeof(elf_osabi)/sizeof(struct data_map); i++) {
		if (header.ident.osabi == elf_osabi[i].i) {
			printf("\tTarget OS ABI: %s\n", elf_osabi[i].s);
			break;
		}
	}
	printf("\tABI version: %d\n", header.ident.abiversion);

	printf("\nBASIC FILE INFORMATION:\n");

	for (int i = 0; i < sizeof(elf_type)/sizeof(struct data_map); i++) {
		if (header.type == elf_type[i].i) {
			printf("\tFile type: %s\n", elf_type[i].s);
			break;
		}
	}

	for (int i = 0; i < sizeof(elf_machine)/sizeof(struct data_map); i++) {
		if (header.machine == elf_machine[i].i) {
			printf("\tTarget architecture: %s\n", elf_machine[i].s);
			break;
		}
	}
	printf("\tProgram entry point: %#x\n", header.entry);
	printf("\tFlags: %#x\n", header.flags);
	printf("\tELF header size: %#x\n", header.ehsize);

	printf("\nPROGRAM:\n");
	printf("\tProgram headers offset: %#x\n", header.phoff);
	printf("\tProgram header size: %#x\n", header.phentsize);
	printf("\tProgram headers count: %#x\n", header.phnum);

	printf("\nSECTIONS:\n");
	printf("\tSection headers offset: %#x\n", header.shoff);
	printf("\tSection header size: %#x\n", header.shentsize);
	printf("\tSection headers count: %#x\n", header.shnum);
	printf("\tSection names location in section header table: %#x\n", header.shstrndx);
}

