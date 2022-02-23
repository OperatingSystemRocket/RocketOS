#include "tar.h"


static uint8_t num_of_files = 0u;
static struct file_header* file_headers[32] = {NULL};

void parse_headers(const uint32_t address) {
    /*for(struct file_header* file_header = (struct file_header*)address; file_header->size != 0; file_header = (struct file_header*)((uint32_t)file_header + file_header->size)) {
        kprintf("%s, %u\n", file_header->filename, file_header->size);
        identity_map_page((uint32_t)get_default_page_directory(), (uint32_t)file_header, PT_PRESENT | PT_RW, PD_PRESENT | PD_RW);
        if(num_of_files >= 32) {
            kprintf("Too many files in file list!\n");
            break;
        }
        file_headers[num_of_files++] = file_header;
    }*/
    //struct file_header *const file_header = (struct file_header*)address;
    unsigned char *const file_header = (struct file_header*)address;
    for(uint32_t i = 0u; i < 128u; ++i) {
        kprintf("%c", file_header[i]);
    }
    //kprintf("%s, %u\n", file_header->filename, file_header->size);
}

void print_file(const char *const filename) {
    kprintf("print_file: %s\n", filename);
    for(uint8_t i = 0; i < num_of_files; i++) {
        if(kstrcmp(filename, file_headers[i]->filename) == 0) {
            for(uint32_t j = 0; j < file_headers[i]->size - sizeof(struct file_header); ++j) {
                kprintf("%c", file_headers[i]->file[j]);
            }
            break;
        }
    }
}
