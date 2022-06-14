#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "kstring.h"
#include "kstdio.h"
#include "kassert.h"
#include <utils/algorithms/comparison.h>
#include <mem/paging.h>
#include <optional.h>


struct file_header {
    uint32_t size; //includes both the file and header, a value of `0` indicates the end of the file list
    char filename[100]; //null terminated
    char reserved[24]; //should just be zeroed out
    char file[];
};


typedef uint32_t Elf32_Addr;
typedef uint16_t Elf32_Half;
typedef uint32_t Elf32_Off;
typedef int32_t Elf32_Sword;
typedef uint32_t Elf32_Word;

#define EI_NIDENT 16

struct Elf32_Ehdr {
    unsigned char e_ident[EI_NIDENT];
    Elf32_Half e_type;
    Elf32_Half e_machine;
    Elf32_Word e_version;
    Elf32_Addr e_entry;
    Elf32_Off e_phoff;
    Elf32_Off e_shoff;
    Elf32_Word e_flags;
    Elf32_Half e_ehsize;
    Elf32_Half e_phentsize;
    Elf32_Half e_phnum;
    Elf32_Half e_shentsize;
    Elf32_Half e_shnum;
    Elf32_Half e_shstrndx;
};

enum Elf32_Type {
    ET_NONE = 0,
    ET_REL = 1,
    ET_EXEC = 2,
    ET_DYN = 3,
    ET_CORE = 4,
    ET_LOPROC = 0xFF00,
    ET_HIPROC = 0xFFFF,
};

enum Elf32_Machine {
    EM_NONE = 0,
    EM_M32 = 1,
    EM_SPARC = 2,
    EM_386 = 3,
    EM_68K = 4,
    EM_88K = 5,
    EM_860 = 7,
    EM_MIPS = 8,
};

enum Elf32_Version {
    EV_NONE = 0,
    EV_CURRENT = 1,
};

enum Elf32_Identification_Index {
    EI_MAG0 = 0,
    EI_MAG1 = 1,
    EI_MAG2 = 2,
    EI_MAG3 = 3,
    EI_CLASS = 4,
    EI_DATA = 5,
    EI_VERSION = 6,
    EI_PAD = 7,
};

#define ELFMAG0 0x7F
#define ELFMAG1 'E'
#define ELFMAG2 'L'
#define ELFMAG3 'F'

#define ELFCLASSNONE 0
#define ELFCLASS32 1
#define ELFCLASS64 2

#define ELFDATANONE 0
#define ELFDATA2LSB 1
#define ELFDATA2MSB 2

enum Elf32_Special_Section {
    SHN_UNDEF = 0,
    SHN_LORESERVE = 0xFF00,
    SHN_LOPROC = 0xFF00,
    SHN_HIPROC = 0xFF1F,
    SHN_ABS = 0xFFF1,
    SHN_COMMON = 0xFFF2,
    SHN_HIRESERVE = 0xFFFF,
};

struct Elf32_Shdr {
    Elf32_Word sh_name;
    Elf32_Word sh_type;
    Elf32_Word sh_flags;
    Elf32_Addr sh_addr;
    Elf32_Off sh_offset;
    Elf32_Word sh_size;
    Elf32_Word sh_link;
    Elf32_Word sh_info;
    Elf32_Word sh_addralign;
    Elf32_Word sh_entsize;
};

enum Elf32_Section_Type {
    SHT_NULL = 0,
    SHT_PROGBITS = 1,
    SHT_SYMTAB = 2,
    SHT_STRTAB = 3,
    SHT_RELA = 4,
    SHT_HASH = 5,
    SHT_DYNAMIC = 6,
    SHT_NOTE = 7,
    SHT_NOBITS = 8,
    SHT_REL = 9,
    SHT_SHLIB = 10,
    SHT_DYNSYM = 11,
};

#define SHT_LOPROC 0x70000000u,
#define SHT_HIPROC 0x7FFFFFFFu,
#define SHT_LOUSER 0x80000000u,
#define SHT_HIUSER 0xFFFFFFFFu,

enum Elf32_Section_Attribute_Flags {
    SHF_WRITE = 0x1,
    SHF_ALLOC = 0x2,
    SHF_EXECINSTR = 0x4,
};

#define SHF_MASKPROC 0xF0000000

#define STN_UNDEF 0

struct Elf32_Sym {
    Elf32_Word st_name;
    Elf32_Addr st_value;
    Elf32_Word st_size;
    unsigned char st_info;
    unsigned char st_other;
    Elf32_Half st_shndx;
};

enum ELF32_ST_BIND {
    STB_LOCAL = 0,
    STB_GLOBAL = 1,
    STB_WEAK = 2,
    STB_LOPROC = 13,
    STB_HIPROC = 15,
};

enum ELF32_ST_TYPE {
    STT_NOTYPE = 0,
    STT_OBJECT = 1,
    STT_FUNC = 2,
    STT_SECTION = 3,
    STT_FILE = 4,
    STT_LOPROC = 13,
    STT_HIPROC = 15,
};

struct Elf32_Rel {
    Elf32_Addr r_offset;
    Elf32_Word r_info;
};

struct Elf32_Rela {
    Elf32_Addr r_offset;
    Elf32_Word r_info;
    Elf32_Sword r_addend;
};

#define ELF32_R_SYM(info) ((info) >> 8)
#define ELF32_R_TYPE(info) ((unsigned char) (info))
#define ELF32_R_INFO(sym, type) (((sym) << 8) + (unsigned char) (type))

struct Elf32_Phdr {
    Elf32_Word p_type;
    Elf32_Off p_offset;
    Elf32_Addr p_vaddr;
    Elf32_Addr p_paddr;
    Elf32_Word p_filesz;
    Elf32_Word p_memsz;
    Elf32_Word p_flags;
    Elf32_Word p_align;
};

enum Elf32_Program_Header_Type {
    PT_NULL = 0,
    PT_LOAD = 1,
    PT_DYNAMIC = 2,
    PT_INTERP = 3,
    PT_NOTE = 4,
    PT_SHLIB = 5,
    PT_PHDR = 6,
};

#define PT_LOPROC 0x70000000,
#define PT_HIPROC 0x7FFFFFFF,

struct Elf32_Dyn {
    Elf32_Sword d_tag;
    union {
        Elf32_Word d_val;
        Elf32_Addr d_ptr;
    } d_un;
};

extern struct Elf32_Dyn DYNAMIC[];


bool is_valid_elf_sig(struct Elf32_Ehdr* elf_header);
void parse_headers(uint32_t address);

GENERATE_OPTIONAL(uint32_t)
struct OPTIONAL_NAME(uint32_t) parse_elf_file(const char* filename);

struct file_header* get_file_header_from_list(const char* filename, struct file_header *const * file_list, uint8_t file_list_len);
struct file_header* get_file_header(const char* filename);
void print_file(const char* filename);
void print_elf_file(const char* filename);

