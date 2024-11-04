#pragma once

#include <stdint.h>

#define PROG_HEADER_INVALID_TYPE	 		(void*) -1
#define PROG_HEADER_INVALID_FLAG			(void*) -2
#define PROG_HEADER_NOT_POWER_OF_2_ALIGN 	(void*) -3
#define PROG_HEADER_MISMATCH_ALIGN_VADDR	(void*) -4

enum {
	PROG_TYPE_NULL,		// unused
	PROG_TYPE_LOAD,		// loadable segment
	PROG_TYPE_DYNAMIC,	// dynamic linking information
	PROG_TYPE_INTERP,	// interpreter information
	PROG_TYPE_NOTE,		// auxiliary information
	PROG_TYPE_SHLIB,	// reserved but has unspecified semantics, non ABI-conformant
	PROG_TYPE_PHDR,		// segment containing program header table itself
	PROG_TYPE_TLS,		// thread-local storage template
	
	// my own extra flags for the ranges stuff. these are unreserved values in the standard
	PROG_TYPE_OS_SPECIFIC,
	PROG_TYPE_PROC_SPECIFIC,

	// reserved inclusive range, OS specific
	PROG_TYPE_LOOS = 0x60000000,	// low
	PROG_TYPE_HIOS = 0x6fffffff,	// high
	
	// reserved inclusive range, processor specific
	PROG_TYPE_LOPROC = 0x70000000,	// low
	PROG_TYPE_HIPROC = 0x7fffffff	// high
};

enum {
	PROG_FLAG_X = 1,		// executable
	PROG_FLAG_W,			// writeable
	PROG_FLAG_R = 4		// readable
};

typedef struct {
	uint32_t type,		// program header type
			 offset,	// offset of the segment in the file image
			 vaddr,		// virtual address of the segment in memory
			 paddr,		// physical address of the segment in memory, on systems where it matters
			 filesz,	// size of the segment in the file image, may be 0
			 memsz,		// size of the segment in memory, may be 0
			 flags,		// segment-dependant flags
			 align;		// 0 and 1 specify no alignment, otherwise should be a power of 2, with vaddr ≡ offset [align]
} ProgHeader; 

ProgHeader* parse_prog_header (const uint8_t* header_data);
void print_prog_header (const ProgHeader h);
