#include "start.h"

void get_base_address () {
    char path[100];
    snprintf(path, 100, "/proc/%d/stat", program.pid);
    FILE* f = fopen(path, "r");
    if(f == NULL) {
        printf("** fopen error");
        return;
    }
    char tmp[100];
    for(int i = 0; i < 25; i++){
        fscanf(f, "%s", tmp);
    }
    unsigned long long address;
    fscanf(f, "%llu", &address);
    program.base_address = address;
    fclose(f);
}

int start(int argc, char argv[][100]) {
    if(state != STATE_LOAD){
        printf("** start must be loaded\n");
        return 0;
    }
    if(program.pid != 0) {
        printf("** program is already running\n");
        return 0;
    }
    program.pid = fork();
    if(program.pid < 0) {
        printf("** start fork error\n");
        return 0;
    } else if(program.pid == 0) {//child process
       if(ptrace(PTRACE_TRACEME, 0, NULL, NULL) < 0) {
            printf("** start ptrace error");
            return 0;
       }
       char** cmd_argv={NULL};
       if(execvp(program.program_name, cmd_argv) == -1){
           printf("** execvp error");
           exit(0);
       }
    } else { //parent 
        int status;
        waitpid(program.pid, &status, 0);
        ptrace(PTRACE_SETOPTIONS, program.pid, 0, PTRACE_O_EXITKILL);
        state = STATE_RUN;
        
        if(program.dynamic_flag == 1)
            get_base_address();
        else 
            program.base_address = 0;

        for(auto &x : program.bp) {
            if(x.fix_flag == false) {
                unsigned long long tmp = x.address;
                if(program.dynamic_flag == true)
                    tmp += program.base_address;
                unsigned char cmd = change_byte(tmp, 0xcc);
                x.origin_cmd = cmd;
            }
        }
        printf("** pid %d\n", program.pid);
    }
    return 0;
}
