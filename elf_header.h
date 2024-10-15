#include <stdint.h>

#define ELF_HEADER_NOT_ELF 				(void*) -1
#define ELF_HEADER_NOT_32BITS			(void*) -2
#define ELF_HEADER_INVALID_ABI			(void*) -3
#define ELF_HEADER_INVALID_TYPE 		(void*) -4
#define ELF_HEADER_UNSUPPORTED_MACHINE 	(void*) -5

enum {
	ELF_BITS_INVALID,
	ELF_BITS_32,
	ELF_BITS_64
};

enum {
	ELF_ENCODING_INVALID,
	ELF_ENCODING_LITTLE_ENDIAN,
	ELF_ENCODING_BIG_ENDIAN
}; 

enum {
	ELF_VERSION_INVALID,
	ELF_VERSION_CURRENT
};

enum {
	ELF_ABI_SYSTEM_V,
	ELF_ABI_HP_UX,
	ELF_ABI_NET_BSD,
	ELF_ABI_LINUX,
	ELF_ABI_GNU_HURD,
	ELF_ABI_SOLARIS = 6,
	ELF_ABI_AIX,
	ELF_ABI_IRIX,
	ELF_ABI_FREE_BSD,
	ELF_ABI_TRU64,
	ELF_ABI_NOVELL_MODESTO,
	ELF_ABI_OPEN_BSD,
	ELF_ABI_OPEN_VMS,
	ELF_ABI_NONSTOP_KERNEL,
	ELF_ABI_AROS,
	ELF_ABI_FENIX_OS,
	ELF_ABI_NUXI_CLOUD_ABI,
	ELF_ABI_STRATUS_TECHNOLOGIES_OPEN_VOS
};

enum {
	ELF_TYPE_NONE,	// no file type
	ELF_TYPE_REL,	// relocatable file
	ELF_TYPE_EXEC,	// executable
	ELF_TYPE_DYN,	// shared object file
	ELF_TYPE_CORE 	// core dump
};

enum {
	ELF_MACHINE_NONE,
	ELF_MACHINE_I386 = 3,
	ELF_MACHINE_ARM = 40,
	ELF_MACHINE_AMD64 = 62,
	ELF_MACHINE_AARCH64 = 183,
	ELF_MACHINE_RISCV = 243
};

struct ELFIdentifier {
	uint8_t mag[4],			// constant signature (0x7f, "ELF
			ei_class,		// 32-bits / 64-bits
			data,			// data encoding
	 		version,		// elf version, 0 for invalid, 1 for current
	 		osabi, 			// operating system ABI
			abiversion;		// ABI version
};
typedef struct ELFIdentifier ELFIdentifier;

struct ELFHeader {
	ELFIdentifier ident;	// elf identifier

	uint16_t type,			// file type
			 machine;		// processor architecture

	uint32_t version,		// elf version again
			 entry,			// program entry point
			 phoff,			// prog headers offset
			 shoff,			// section headers offset
			 flags;			// interpretation depends on target architecture
	
	uint16_t ehsize,		// elf header size
			 phentsize,		// single prog header size
			 phnum,			// number of prog headers
			 shentsize,		// single section header size
			 shnum,			// number of section headers
			 shstrndx;		// index of section header table containing section names
};
typedef struct ELFHeader ELFHeader;

ELFHeader* parse_elf_header (uint8_t* header_data);
void print_elf_header (ELFHeader h);
