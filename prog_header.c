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
		if (header->vaddr != header->offset % header->align) {
			free(header);
			return PROG_HEADER_MISMATCH_ALIGN_VADDR;
		}
	}

	return header;
}
