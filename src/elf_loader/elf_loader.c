#include <kstdio.h>

unsigned char* e_ident;
uint16_t e_type;
uint16_t e_machine;
uint32_t e_version;
uint32_t e_entry;
uint32_t e_phoff;
uint32_t e_shoff;
uint32_t e_flags;
uint16_t e_ehsize;
uint16_t e_phentsize;
uint16_t e_phnum;
uint16_t e_shentsize;
uint16_t e_shnum;
uint16_t e_shstrndx;

int load_elf_file(const uint8_t* const file) {
    e_ident = file; //e_ident is an array of length 16, indices 0-7 are important and 8-15 are padding set to 0
    e_type = *((const uint16_t*)file + 16);
    e_machine = *((const uint16_t*)file + 18);
    e_version = *((const uint16_t*)file + 20);
    e_entry = *((const uint32_t*)file + 24); 
    e_phoff = *((const uint32_t*)file + 28); 
    e_shoff = *((const uint32_t*)file + 32); 
    e_flags = *((const uint32_t*)file + 36); 
    e_ehsize = *((const uint16_t*)file + 40); 
    e_phentsize = *((const uint16_t*)file + 42); 
    e_phnum = *((const uint16_t*)file + 44); 
    e_shentsize = *((const uint16_t*)file + 46); 
    e_shnum = *((const uint16_t*)file + 48); 
    e_shstrndx = *((const uint16_t*)file + 50); 

    if(e_ident[0] != 0x7f || e_ident[1] != 'E' || e_ident[2] != 'L' || e_ident[3] != 'F') {
        kprintf("Not an ELF file!\n");
        return 1;
    }
    if(e_ident[4] != 1) {
        kprintf("Must be 32 bit!\n");
        return 2;
    }
    if(e_ident[5] != 1) {
        kprintf("Must use little endian encoding!\n"); //TODO add support for big endian encoding
        return 3;
    } 
    if(e_ident[6] != 1) {
        kprintf("Something is wrong with that elf file, should be set to 1!\n");
        return 4;
    }
    if(e_ident[7] != 0) {
        kprintf("OS ABI should be set to 0!\n");
        return 5;
    }
    if(e_type != 2) {
        kprintf("Not an executable file!\n");
        return 6;
    }
    if(e_machine != 0x03) {
        kprintf("Not x86!\n");
        return 7;
    }
    if(e_version != 1) {
        kprintf("Must be ELF version 1!\n");
        return 8;
    }

    for(uint8_t i = 0; i < e_phnum; ++i) {
        process_program_header(e_phoff + (i * (e_phentsize)));
    }

    return 0;
}

int process_program_header(const uint8_t* const program_header, const uint8_t* const file) {
    uint32_t type  = *((const uint32_t*)program_header);
    uint32_t offset= *((const uint32_t*)program_header + 4);
    uint32_t vaddr = *((const uint32_t*)program_header + 8);
    uint32_t paddr = *((const uint32_t*)program_header + 12);
    uint32_t filesz= *((const uint32_t*)program_header + 16);
    uint32_t memsz = *((const uint32_t*)program_header + 20);
    uint32_t flags = *((const uint32_t*)program_header + 24);
    uint32_t align = *((const uint32_t*)program_header + 28);

    if(type == 1) {
        uint8_t* mem = allocate_virtual_page(vaddr, flags, flags);
        kmemcpy(mem, file + offset, filesz);
    }

    return 0;
}
    
