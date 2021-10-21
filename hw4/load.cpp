#include "load.h"


unsigned long long get_value(unsigned long long size, unsigned long long offset, unsigned char* buf) {
    unsigned long long ret = 0;
    for(int i = size + offset - 1; i >= (int)offset; i--) {
        ret = ret * 256 + buf[i];
    }
    return ret;
}

int elf_parse(char* path) {
    int fd = open(path, O_RDONLY);
    if(fd == -1) {
        return -1;
    }
    unsigned char elf[0x40];
    read(fd, elf, 0x40);
    
    /*get entry point*/
    unsigned long long address = get_value(ELF_ENTRY_POINT_SIZE, ELF_ENTRY_POINT_OFFSET, elf);
    program.load.entry = address;
    program.load.virtual_address = address;

    
    /*dynamic flag*/
    unsigned long long is_dynamic = get_value(E_TYPE_SIZE, E_TYPE_OFFSET, elf);
    if(is_dynamic == 0x03)
        program.dynamic_flag = 1;
    else 
        program.dynamic_flag = 0;

    /*sh*/
    unsigned long long shnum = get_value(E_SHNUM_SIZE, E_SHNUM_OFFSET, elf);
    unsigned long long shoff = get_value(E_SHOFFSET_SIZE, E_SHOFFSET_OFFSET, elf);
    unsigned long long shsize = get_value(E_SHSIZE_SIZE, E_SHSIZE_OFFSET, elf);
    unsigned long long shstr = get_value(E_SHSTRNDX_SIZE, E_SHSTRNDX_OFFSET, elf);
    
    unsigned char sec[shsize * shnum];
    unsigned char *sec_ptr = sec;
    lseek(fd, shoff, SEEK_SET);
    read(fd, sec, shsize * shnum);
    
    unsigned char* str_ptr = shstr * shsize + sec;
    unsigned long long shstr_size = get_value(SH_SIZE_SIZE, SH_SIZE_OFFSET, str_ptr);
    unsigned long long shstr_offset = get_value(SH_OFFSET_SIZE, SH_OFFSET_OFFSET, str_ptr);
    char name[shstr_size];
    lseek(fd, shstr_offset, SEEK_SET);
    read(fd,name, shstr_size);

    for(unsigned long long i = 0; i < shnum; i++) {
        unsigned long long sh_name = get_value(SH_NAME_SIZE, SH_NAME_OFFSET, sec_ptr);

        unsigned long long sh_offset = get_value(SH_OFFSET_SIZE, SH_OFFSET_OFFSET, sec_ptr);
        unsigned long long sh_size = get_value(SH_SIZE_SIZE, SH_SIZE_OFFSET, sec_ptr);
        unsigned long long vir = get_value(SH_ADDRESS_SIZE, SH_ADDRESS_OFFSET, sec_ptr);
        sec_ptr += shsize;
        if(strncmp(".text", name+sh_name, 5) == 0) {
            program.load.offset = sh_offset;
            program.load.size = sh_size;
            //program.asm_file.offset = sh_offset;
            program.asm_file.offset = 0;
            
            program.asm_file.size = sh_size;
            program.asm_file.cur_virtual_address = vir;
            lseek(fd, sh_offset, SEEK_SET);
            char* code = (char*)malloc(sh_size);
            program.asm_file.data = code;
            read(fd, program.asm_file.data, sh_size);
        }
    }
    program.fd = dup(fd);
    return 0;
}

int load_program(int argc, char argv[][100]) {
    if(state != STATE_NONE) {
        printf("** Error, program %s has already been loaded\n", program.program_name);
        return 0;
    }
    if(elf_parse(argv[0]) != 0) {
        printf("** load program error: parse error\n");
        return 0;
    }
    state = STATE_LOAD;
    program.program_name = strdup(argv[0]);
    printf("** program '%s' loaded. entry point 0x%-6llx\n", program.program_name, program.load.virtual_address);
    return 0;
}
