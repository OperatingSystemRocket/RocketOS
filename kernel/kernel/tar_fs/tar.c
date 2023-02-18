#include <tar_fs/tar.h>

#include "scheduler.h"


static uint8_t num_of_files = 0u;
static struct file_header* file_headers[32] = {NULL};

void parse_headers(const uint32_t address) {
    for(struct file_header* file_header = (struct file_header*)address; file_header->size != 0; file_header = (struct file_header*)((uint32_t)file_header + file_header->size)) {
        kprintf("%s, %u\n", file_header->filename, file_header->size);
        for(uint32_t i = 0u; i < ((file_header->size/PAGE_SIZE)+(file_header->size%PAGE_SIZE)); ++i) {
            identity_map_page_in_kernel_addr(((uint32_t)file_header&0xFFFFF000) + (i*PAGE_SIZE), PT_PRESENT, PD_PRESENT);
        }
        for(uint32_t i = 0u; i < file_header->size-sizeof(struct file_header); ++i) {
            kprintf("%c", file_header->file[i]);
        }
        kprintf("\n");
        if(num_of_files >= 32) {
            kprintf("Too many files in file list!\n");
            break;
        }
        struct file_header *const new_file_header = kmalloc(file_header->size);
        kmemcpy(new_file_header, file_header, file_header->size);
        file_headers[num_of_files++] = new_file_header;
    }
}

struct file_header* get_file_header_from_list(const char *const filename, struct file_header *const *const file_list, const uint8_t file_list_len) {
    for(uint8_t i = 0u; i < file_list_len; ++i) {
        if(kstrcmp(filename, file_list[i]->filename) == 0) {
            return file_list[i];
        }
    }
    return NULL;
}

struct file_header* get_file_header(const char *const filename) {
    return get_file_header_from_list(filename, file_headers, num_of_files);
}

void print_file(const char *const filename) {
    kprintf("print_file: %s\n", filename);
    struct file_header *const file_header = get_file_header_from_list(filename, file_headers, num_of_files);
    if(file_header == NULL) {
        kprintf("File not found!\n");
        return;
    }
    for(uint32_t i = 0u; i < file_header->size-sizeof(struct file_header); ++i) {
        kprintf("%c", file_header->file[i]);
    }
}

void print_elf_file(const char *const filename) {
    kprintf("print_file: %s\n", filename);
    struct file_header *const file_header = get_file_header_from_list(filename, file_headers, num_of_files);
    if(file_header == NULL) {
        kprintf("ELF file not found!\n");
        return;
    }
    for(uint32_t i = 0u; i < file_header->size-sizeof(struct file_header); ++i) {
        kprintf("%c", file_header->file[i]);
    }
}

bool is_valid_elf_sig(struct Elf32_Ehdr *const elf_header) {
    return elf_header->e_ident[EI_MAG0] == ELFMAG0 && elf_header->e_ident[EI_MAG1] == ELFMAG1 && elf_header->e_ident[EI_MAG2] == ELFMAG2 && elf_header->e_ident[EI_MAG3] == ELFMAG3;
}

static const char* get_section_type_name(const uint32_t type) {
    switch(type) {
        case SHT_NULL: return "SHT_NULL";
        case SHT_PROGBITS: return "SHT_PROGBITS";
        case SHT_SYMTAB: return "SHT_SYMTAB";
        case SHT_STRTAB: return "SHT_STRTAB";
        case SHT_RELA: return "SHT_RELA";
        case SHT_HASH: return "SHT_HASH";
        case SHT_DYNAMIC: return "SHT_DYNAMIC";
        case SHT_NOTE: return "SHT_NOTE";
        case SHT_NOBITS: return "SHT_NOBITS";
        case SHT_REL: return "SHT_REL";
        case SHT_SHLIB: return "SHT_SHLIB";
        case SHT_DYNSYM: return "SHT_DYNSYM";
    }
    return "Unknown";
}

struct OPTIONAL_NAME(uint32_t) parse_elf_file(const char *const filename) {
    struct file_header *const file_header = get_file_header_from_list(filename, file_headers, num_of_files);
    struct Elf32_Ehdr *const elf_header = (struct Elf32_Ehdr*)file_header->file;
    if(!is_valid_elf_sig(elf_header)) {
        kprintf("Not an elf file!\n");
        return (struct OPTIONAL_NAME(uint32_t)){0u, false};
    }
    if(elf_header->e_ident[EI_CLASS] != ELFCLASS32) {
        kprintf("Not a 32-bit elf file!\n");
        return (struct OPTIONAL_NAME(uint32_t)){0u, false};
    }
    if(elf_header->e_ident[EI_DATA] != ELFDATA2LSB) {
        kprintf("Not a little-endian elf file!\n");
        return (struct OPTIONAL_NAME(uint32_t)){0u, false};
    }
    if(elf_header->e_ident[EI_VERSION] != EV_CURRENT) {
        kprintf("Not a current elf file!\n");
        return (struct OPTIONAL_NAME(uint32_t)){0u, false};
    }

    if(elf_header->e_type != ET_REL && elf_header->e_type != ET_EXEC) {
        kprintf("Not a REL or EXEC elf file!\n");
        return (struct OPTIONAL_NAME(uint32_t)){0u, false};
    }
    if(elf_header->e_machine != EM_386) {
        kprintf("Not a x86 elf file!\n");
    }
    if(elf_header->e_version != EV_CURRENT) {
        kprintf("Not a current elf file!\n");
        return (struct OPTIONAL_NAME(uint32_t)){0u, false};
    }

    const uint32_t entry_point = elf_header->e_entry;
    const uint32_t program_header_offset = elf_header->e_phoff;
    const uint32_t program_header_size = elf_header->e_phentsize;
    const uint32_t program_header_count = elf_header->e_phnum;
    const uint32_t section_header_offset = elf_header->e_shoff;
    const uint32_t section_header_size = elf_header->e_shentsize;
    const uint32_t section_header_count = elf_header->e_shnum;
    const uint32_t section_header_strtab_offset = elf_header->e_shstrndx;
    kassert(elf_header->e_ehsize == sizeof(struct Elf32_Ehdr), ((struct OPTIONAL_NAME(uint32_t)){0u, false}));


    kprintf("Entry point: %X\n", entry_point);
    kprintf("Program header offset: %u\n", program_header_offset);
    kprintf("Program header size: %u\n", program_header_size);
    kprintf("Program header count: %u\n", program_header_count);
    kprintf("Section header offset: %u\n", section_header_offset);
    kprintf("Section header size: %u\n", section_header_size);
    kprintf("Section header count: %u\n", section_header_count);
    kprintf("Section header strtab offset: %u, SHN_UNDEF: %u\n", section_header_strtab_offset, (uint32_t)SHN_UNDEF);

    //assume for now that that `program_header_offset` is non-zero and that it is a valid offset
    struct Elf32_Phdr *const program_headers = (struct Elf32_Phdr*)(file_header->file + program_header_offset);
    //assume for now that that `section_header_offset` is non-zero and that it is a valid offset
    struct Elf32_Shdr *const section_header = (struct Elf32_Shdr*)(file_header->file + section_header_offset);
    //assume for now that that `section_header_strtab_offset` is non-zero and that it is a valid offset
    struct Elf32_Shdr *const section_header_strtab = (struct Elf32_Shdr*)(&section_header[section_header_strtab_offset]);
    kassert(section_header_strtab->sh_type == SHT_STRTAB, ((struct OPTIONAL_NAME(uint32_t)){0u, false}));
    char *const section_header_strtab_data = (char*)(file_header->file + section_header_strtab->sh_offset);
    kprintf("Section header strtab data: %s\n", section_header_strtab_data);
    for(uint32_t i = 0u; i < section_header_count; ++i) {
        kprintf("Section header %u:%s: %s\n", i, get_section_type_name(section_header[i].sh_type), section_header_strtab_data + section_header[i].sh_name);
    }

    kprintf("Section headers:\n");
    struct Elf32_Shdr* symbol_table = NULL;
    struct Elf32_Shdr* string_table = NULL; //for symbols
    for(uint32_t i = 0u; i < section_header_count; ++i) {
        struct Elf32_Shdr *const shdr = &section_header[i];
        char *const section_data = (char*)(file_header->file + shdr->sh_offset);
        const uint32_t section_size = shdr->sh_size;
        if(shdr->sh_type == SHT_SYMTAB) {
            symbol_table = shdr;
            kprintf("Symbol table: %s\n", section_header_strtab_data + symbol_table->sh_name);
        }
        else if(shdr->sh_type == SHT_STRTAB && i != section_header_strtab_offset) {
            string_table = shdr;
            kprintf("String table: %s\n", section_header_strtab_data + string_table->sh_name);
        }
    }
    char *const string_table_data = (char*)(file_header->file + string_table->sh_offset);

    struct Elf32_Sym *const symbol_table_data = (struct Elf32_Sym*)(file_header->file + symbol_table->sh_offset);
    const uint32_t num_of_symbols = symbol_table->sh_size / sizeof(struct Elf32_Sym);
    kprintf("num_of_symbols: %u:\n", num_of_symbols);
    for(uint32_t i = 0u; i < num_of_symbols; ++i) {
        struct Elf32_Sym *const sym = &symbol_table_data[i];
        kprintf("Symbol %u: %s\n", i, string_table_data + sym->st_name);
    }

    uint32_t memsz = 0u;
    uint32_t align = 1u;
    uint32_t physical_base_addr = 0u;
    uint32_t virtual_base_addr = 0u;
    struct Elf32_Phdr* phdr = NULL;
    for(uint32_t i = 0u; i < program_header_count; ++i) {
        phdr = &program_headers[i];
        if(phdr->p_type == PT_LOAD) {
            kprintf("PT_LOAD: %u\n", i);
            kprintf("p_offset: %X\n", phdr->p_offset);
            kprintf("p_vaddr: %X\n", phdr->p_vaddr);
            kprintf("p_paddr: %X\n", phdr->p_paddr);
            kprintf("p_filesz: %X\n", phdr->p_filesz);
            kprintf("p_memsz: %X\n", phdr->p_memsz);
            kprintf("p_flags: %X\n", phdr->p_flags);
            kprintf("p_align: %X\n", phdr->p_align);
            memsz = max(memsz, phdr->p_memsz);
            align = max(align, phdr->p_align);
            kprintf("memsz: %X\n", memsz);
            kprintf("align: %X\n", align);
            physical_base_addr = phdr->p_paddr;
            virtual_base_addr = phdr->p_vaddr;
        }
    }

    const uint32_t num_of_pages = memsz/PAGE_SIZE + (memsz%PAGE_SIZE > 0u);
    char *const process_memory = (char*)virtual_base_addr;
    void *const physical_memory = global_phys_allocator_allocate_pages(num_of_pages);
    //for(uint32_t i = 0u; i < num_of_pages; ++i) {
    //    allocate_virtual_page(process_memory + i*PAGE_SIZE, PT_PRESENT | PT_RW | PT_USER, PD_PRESENT | PD_RW | PD_USER);
    //}
    map_pages_in_kernel_addr(process_memory, physical_memory, num_of_pages, PT_PRESENT | PT_RW | PT_USER, PD_PRESENT | PD_RW | PD_USER);
    kmemcpy(process_memory, file_header->file + phdr->p_offset, memsz);

    kprintf("before call\n");

    void (*entry_start)(void) = (void (*)(void))(entry_point);
    //entry_start();

    return (struct OPTIONAL_NAME(uint32_t)){entry_point, true};
}
