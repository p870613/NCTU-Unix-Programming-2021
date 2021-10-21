#include "break.h"

bool address_in_text(unsigned long long address) {
    if(state == STATE_RUN && program.dynamic_flag == 1) {
        unsigned long long tmp = program.base_address + program.load.virtual_address;
        return (address >= tmp && address <= (tmp + program.load.size));
    }
    return (address >= program.load.virtual_address && address < (program.load.virtual_address + program.load.size));
}

unsigned char change_byte(unsigned long long address, unsigned char cmd) {
    auto code = ptrace(PTRACE_PEEKTEXT, program.pid, address, NULL);
    ptrace(PTRACE_POKETEXT, program.pid, address, (code & 0xffffffffffffff00) | (cmd & 0xff));
    return code & 0xff;
}

int brk(int argc, char argv[][100]) {
    if(argc == 0) {
        printf("** break argc: no parameter");
        return 0;
    }
    unsigned long long address = strtol(argv[0], NULL, 16);
    if(state == STATE_LOAD) {
        if(address_in_text(address)) {
           program.bp.push_back({program.break_num ++, address, 0, false}); 
           return 0;
        }
        printf("** address must be in the text segment. (LOADED state)\n");
        return 0;
    } else if(state == STATE_RUN) {
        if(address_in_text(address)) {
            unsigned char t = 0xcc;
            unsigned char tmp = change_byte(address, t);
            program.bp.push_back({program.break_num ++, address, tmp, true}); 
        } else {
            printf("** address must be in the text segment. (running state)\n");
        }
        return 0;
    } else {
        printf("** break error: state error");
        return 0;
    }
}

int list(int, char[][100]) {
    for(auto &x: program.bp) {
        printf("%3d: %llx\n", x.id, x.address);
    }
    return 0;
}

int del(int argc, char argv[][100]) {
    int id = strtol(argv[0], NULL, 10);
    for(size_t i = 0; i < program.bp.size(); i++) {
        if(program.bp[i].id == id) {
            change_byte(program.bp[i].address, program.bp[i].origin_cmd);
            program.bp.erase(program.bp.begin() + i);
            printf("** breakpoint %d deleted.\n", id);
            return 0;
        }
    }
    printf("** no breakpoint number %d.\n", id);
    return 0;
}
