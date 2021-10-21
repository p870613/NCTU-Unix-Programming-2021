#include "reg.h"

struct REGS REGS[] ={
    {(char*)"R15", get_r15, set_r15},
    {(char*)"R14", get_r14, set_r14},
    {(char*)"R13", get_r13, set_r13},
    {(char*)"R12", get_r12, set_r12},
    {(char*)"R11", get_r11, set_r11},
    {(char*)"R10", get_r10, set_r10},
    {(char*)"R9", get_r9,  set_r9},
    {(char*)"R8", get_r8,  set_r8},
    {(char*)"RAX", get_rax, set_rax},
    {(char*)"RBX", get_rbx, set_rbx},
    {(char*)"RCX", get_rcx, set_rcx},
    {(char*)"RDX", get_rdx, set_rdx},
    {(char*)"RDI", get_rdi, set_rdi},
    {(char*)"RSI", get_rsi, set_rsi},
    {(char*)"RBP", get_rbp, set_rbp},
    {(char*)"RSP", get_rsp, set_rsp},
    {(char*)"RIP", get_rip, set_rip},
    {(char*)"FLAGS", get_eflags, set_eflags}
};
int NREGS = (sizeof(REGS)/sizeof(struct REGS));

char* tolower(char* s, char* t) {
    for(size_t i = 0; i < strlen(s); i++) {
        t[i] = s[i] + 32;
    }
    return t;
}

int getreg(int argc, char argv[][100]) {
    if(state != STATE_RUN) {
        printf("** get error: must in running\n");
        return 0;
    }
    if(ptrace(PTRACE_GETREGS, program.pid, 0, &program.regs) < 0) {
        printf("** getreg error: ptrace error\n");
        return 0;
    }

    char t[10];
    for(int i = 0; i < NREGS; i++) {
        if(strncmp(argv[0], tolower(REGS[i].name, t), strlen(argv[0])) == 0) {
            printf("%s = %lld (0x%llx)\n",argv[0], REGS[i].get(&program.regs), REGS[i].get(&program.regs));
            return 0;
        }
        
    }
    printf("** get error: argv error\n");
    return 0;
}

int getregs(int argc, char argv[][100]) {
    if(state != STATE_RUN) {
        printf("** getregs error: must in running\n");
        return 0;
    }
    
    if(ptrace(PTRACE_GETREGS, program.pid, 0, &program.regs) < 0) {
        printf("** getregs error: ptrace error\n");
        return 0;
    }
    
    for(int i = 0; i < NREGS; i++) {
        if(i == 17)
            printf("%s = %016llx\n",REGS[i].name, REGS[i].get(&program.regs));
        else 
            printf("%s = %llx\n",REGS[i].name, REGS[i].get(&program.regs));
    }
    return 0;
}

int setreg(int argc, char argv[][100]) {
    if(state != STATE_RUN) {
        printf("** set reg error: must in running\n");
        return 0;
    }
    unsigned long long value = strtol(argv[1], NULL, 16);
    char t[10];
    for(int i = 0; i < NREGS; i++) {
        if(strncmp(argv[0], tolower(REGS[i].name, t), strlen(argv[0])) == 0) {
            REGS[i].set(&program.regs, value);
            ptrace(PTRACE_SETREGS, program.pid, NULL, &program.regs);
            return 0;
        }
        
    }
    printf("** get error: argv error\n");
    return 0;
}


