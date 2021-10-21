#include "disasm.h"

int capstone_disasm(unsigned char* pos, unsigned long long *address, int* len, int max_len) {
    csh h;
    cs_insn* insn;
    size_t count;
    if (cs_open(CS_ARCH_X86, CS_MODE_64, &h) != CS_ERR_OK) {
        printf("** cap error: init error \n");
        return 0 ;
    }
    count = cs_disasm(h, pos, 100, *address, 0, &insn);
    if (count > 0) {
        unsigned long long t = *address; 
        for (size_t i = 0; i < count && *(len) < max_len; i++, (*len)++) {
            printf("\t0x%lx\t", insn[i].address);
            
            for (size_t j = 0; j < 12; j++) {
                if (j < insn[i].size) {
                    printf("%02hhx ", insn[i].bytes[j]);
                }else{
                    printf("   ");
                }
            }
            printf("\t%s\t%s\n", insn[i].mnemonic,
                        insn[i].op_str);
            t = t + count;
            *address = insn[i].address;
            if(insn[i].address >= program.load.virtual_address + program.load.size - 1) {
                *address = insn[i].address + 1;
                return -1;
            }
        }
        
    } else {
       printf("** disasm error: failed to disassemble code \n"); 
    }
    cs_close(&h);
    return count;
}

int dis_asm(int argc, char argv[][100]) {
    struct disasm* dmp;
    if(state != STATE_LOAD && state != STATE_RUN) {
        printf("** disasm error: state not in load or run\n");
        return 0;
    }

    unsigned long long address = strtol(argv[0], NULL, 16);
    if(argc == 0) {
        if(program.disasm_previous_flag == true) {
            address = program.disasm_previous_address;
        } else {
            printf("** no addr given. \n");
            return 0;
        }
    } else {
        address = strtol(argv[0], NULL, 16);
    }
    
    dmp = &program.asm_file;
    if(state == STATE_LOAD) {
        if(!(address >= program.load.virtual_address && 
           address <= program.load.virtual_address + program.load.size - 1)) {
            printf("** disasm error: address out of bound\n");
            return 0;
        }

        for(int i = 0; i < DISASM_LEN; i++) {
            unsigned char* pos = (unsigned char*)dmp->data + dmp->offset + (address - dmp->cur_virtual_address);
            fflush(stdout);
            int count = capstone_disasm(pos, &address, &i, 10);
            i --;
            if(count == 0 || count == -1) {
                break;
            }
            
        }
    }else if(state == STATE_RUN) {
        if(!(address >= program.load.virtual_address && 
           address <= program.load.virtual_address + program.load.size - 1)) {
            printf("** disasm error: address out of bound\n");
            return 0;
        }
        
        long long offset = 0;
        if(program.dynamic_flag == 1) {
            offset = address - program.base_address;
        } else {
            offset = program.asm_file.offset + (address - program.asm_file.cur_virtual_address);
        }

        for(int i = 0; i < DISASM_LEN; i++) {
            unsigned char* pos = (unsigned char*)dmp->data + offset;
            int count = capstone_disasm(pos, &address, &i, 10);
            i --;
            if(count == 0 || count == -1) {
                break;
            }
        }
    }
    program.disasm_previous_flag = true;
    program.disasm_previous_address = address;
    return 0;
}

int dump(int argc, char argv[][100]) {
    if(state != STATE_RUN) {
        printf("** dump error: must in running state. \n");
        return 0;
    }
   
    unsigned long long address;
    if(argc == 0) {
        if(program.dump_previous_flag == true) {
            address = program.dump_previous_address;
        } else {
            printf("** no addr given. \n");
            return 0;
        } 
    } else {
        address = strtol(argv[0], NULL, 16);
    }

    
    unsigned char data[80];
    unsigned char* dataptr = data;
    for(int i = 0; i < 5; i++) {
        printf("      %llx: ", address);
        for(int j = 0; j < 16; j ++) {
            *dataptr = ptrace(PTRACE_PEEKDATA, program.pid, address, NULL);
            address = address + 1;
            dataptr ++;
            printf("%02x ", data[16 * i + j]);
        }
        
        printf("|"); 
        for(int j = 0; j < 16; j ++) {
            char c = data[16*i+j];
            if(isprint(c) != 0) {
                printf("%c", c);
            }else {
                printf(".");
            }
        }
        printf("|"); 
        printf("\n");
    }
    program.dump_previous_flag = true;
    program.dump_previous_address = address;
    return 0;
}
