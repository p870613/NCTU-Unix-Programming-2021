#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "runcmd.h"
#include "load.h"
int state = 0;
struct program_content program;

void state_clean () {
    state = STATE_NONE;
    close(program.fd);
    free(program.program_name);
}

void program_init() {
    program.base_address = 0;
    program.break_num = 0;
    program.pid = 0;
    program.hit_breakpoint = -1;
    program.disasm_previous_address = 0;
    program.dump_previous_address = 0;
    program.dump_previous_flag = false;
    program.disasm_previous_flag = false;
}

void program_destory() {
    free(program.asm_file.data);
    close(program.fd);
}

int main(int argc, char *argv[]) {
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    program_init();
    
    char* script_path;
    FILE* input = stdin;
    if(argc != 1) {
        if(argc == 2) {
            char tmp[100][100];
            snprintf(tmp[0], 100, "%s", argv[1]);
            load_program(1, tmp);
        }else if(argc == 4 || argc == 3){
            char c;
            while((c = getopt(argc, argv, "-s:")) != -1) {
                switch(c) {
                    case 's':
                        script_path = optarg;
                        input = fopen(script_path, "r");
                        break;
                    default:
                        char *path = optarg;
                        char tmp[100][100];
                        snprintf(tmp[0], 100, "%s", path);
                        printf("%s\n", tmp[0]);
                        load_program(1, tmp); 
                }
            }
        } else {
            printf("usage: ./hw4 [-s script] [program]\n");
            return 0;
        }
    }
    while(true) {
        printf("sdb> ");

        char* cmd = NULL;
        size_t cmd_len = 0;
        
        if(getline(&cmd, &cmd_len, input) != -1) {
            if(strcmp(cmd, "\n") == 0)
                continue;
            int ret = run_command(cmd);
            if(ret == -1) {
                printf("** run command error\n");
            }else if (ret == 2) {
                return 0;
            }
        }else {
            return -1;
        }
    }
    program_destory();
}
