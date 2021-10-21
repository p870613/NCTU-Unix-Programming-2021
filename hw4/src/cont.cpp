#include "cont.h"

int cont(int argc, char argv[][100]) {
    if(state != STATE_RUN) {
        printf("** state must be RUNNING.\n");
        return 0;
    }
   
    if (program.hit_breakpoint != -1) {
        char t[][100] = {NULL};
        si(0, t);
    }

    ptrace(PTRACE_CONT, program.pid, NULL, NULL);
    check_breakpoint();
    return 0;
}

void one_disasm(unsigned long long address) {
    long long offset = 0;
    if(program.dynamic_flag == 1) {
        offset = address - program.base_address;
    } else {
        offset = program.asm_file.offset + (address - program.asm_file.cur_virtual_address);
    }
    unsigned char* pos = (unsigned char*)program.asm_file.data + offset;
    csh h;
    cs_insn* insn;
    cs_open(CS_ARCH_X86, CS_MODE_64, &h);
    cs_disasm(h, pos, 100, address, 0, &insn);
        
    printf("\t0x%lx:\t", insn[0].address);
            
    for (size_t j = 0; j < 12; j++) {
        if (j < insn[0].size) {
            printf("%02hhx ", insn[0].bytes[j]);
        }else{
            printf("   ");
        }
    }
    printf("\t%s\t%s\n", insn[0].mnemonic,
                insn[0].op_str);
    cs_close(&h);
}

void show_bp_msg(unsigned long long address) {
    printf("** breakpoint @");
    one_disasm(address);
}

unsigned long long get_reg_in_cont(char* reg) {
    if(ptrace(PTRACE_GETREGS, program.pid, 0, &program.regs) < 0) {
        printf("** cont error: get rip error\n");
        return 0;
    }
    for(int i = 0; i < NREGS; i++) {
        if(strcmp(reg, REGS[i].name) == 0) {
            return REGS[i].get(&program.regs);
        }
    }
    printf("** cont error: reg name error\n");
    return 0;
}

int check_breakpoint() {
    int status;
    waitpid(program.pid, &status, 0);
    if(WIFSTOPPED(status)) {
        if(WSTOPSIG(status) != SIGTRAP) {
            printf("** %d stop by signal", program.pid);
            return -1;
        }
        if(program.hit_breakpoint != -1)
            return 0;
        
        for(auto &x: program.bp) {
            unsigned long long tmp = x.address;
            if(program.dynamic_flag == 1 && x.fix_flag == false) {
                tmp = tmp + program.base_address;
            }
            if(tmp == get_reg_in_cont((char*)"RIP") - 1) {
                program.hit_breakpoint = x.id;
                program.hit_address = tmp;
                show_bp_msg(tmp);
                change_byte(tmp, x.origin_cmd); 
                program.regs.rip --;
                ptrace(PTRACE_SETREGS, program.pid, NULL, &program.regs);
                return 1;
            }

        }
        return -1;
    }

    if(WIFEXITED(status)) {
        if (WIFSIGNALED(status))
            printf("** %d stop by signal", program.pid);
        else
           printf("** chlid process %d terminiated normally (code %d)\n", program.pid, status);
       program.pid = 0;
       state = STATE_LOAD;
       return -1;
    }
    return -1;
}

int si(int argc, char argv[][100]) {
    if(state != STATE_RUN) {
        printf("** si error: must in running state\n");
        return 0;
    }
    ptrace(PTRACE_SINGLESTEP, program.pid, NULL, NULL);
    if(check_breakpoint() == 0 && program.hit_breakpoint != -1) {
        for(auto &x : program.bp) {
            if(x.id == program.hit_breakpoint) {
                unsigned char tmp = change_byte(x.address, 0xcc);
                x.origin_cmd = tmp;
                program.hit_breakpoint = -1;
                break;
            }
        }
    }
    return 0;
}
