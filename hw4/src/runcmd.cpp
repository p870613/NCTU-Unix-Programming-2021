#include "runcmd.h"

struct Command Commands[] = {
    {(char*)"break" , (char*)"b", (char*)"- break {instruction-address}: add a break point", brk},
    {(char*)"cont"  , (char*)"c", (char*)"- cont: continue execution", cont},
    {(char*)"delete", NULL, (char*)"- delete {break-point-id}: remove a break point", del},
    {(char*)"disasm", (char*)"d", (char*)"- disasm addr: disassemble instructions in a file or a memory region", dis_asm},
    {(char*)"dump"  , (char*)"x", (char*)"- dump addr [length]: dump memory content", dump},
    {(char*)"exit"  , (char*)"q", (char*)"- exit: terminate the debugger", quit},
    {(char*)"get"   , (char*)"g", (char*)"- get reg: get a single value from a register", getreg},
    {(char*)"getregs", NULL, (char*)"- getregs: show registers", getregs },
    {(char*)"help"  , (char*)"h", (char*)"- help: show this message", help},
    {(char*)"list"  , (char*)"l", (char*)"- list: list break points", list},
    {(char*)"load"  ,  NULL, (char*)"- load {path/to/a/program}: load a program", load_program},
    {(char*)"run"   , (char*)"r", (char*)"- run: run the program", run},
    {(char*)"vmmap" , (char*)"m", (char*)"- vmmap: show memory layout", vmmap},
    {(char*)"set"   , (char*)"s", (char*)"- set reg val: get a single value to a register", setreg},
    {(char*)"si"    , NULL, (char*)"- si: step into instruction", si},
    {(char*)"start" , NULL, (char*)"- start: start the program and stop at the first instruction", start},
};

int command_len = sizeof(Commands) / sizeof(struct Command);
void print_cmd(char* cmdline, char argv[][100], int len) {
    printf("%s\n", cmdline);
    for(int i = 0; i < len; i++)
        printf("%s\n", argv[i]);
}

void parse_cmd(char* cmd, char* cmdline, char argv[][100], int* len) {
    int flag = 0;
    int space_flag = 0;
    int index = 0;
    for(size_t i = 0; i < strlen(cmd); i++) {
        if(cmd[i] == '\n') 
            continue;
        if(cmd[i] == ' ' && space_flag == 0) {
            if(flag == 0) {
                flag = 1;
                cmdline[index] = '\0';
                index = 0;
                space_flag = 1; 
            } else if(flag == 1) {
                argv[(*len) ++][index] = '\0';
                index = 0;
                space_flag = 1; 
            }
            continue;
        }
           
        if(flag == 0) {
            cmdline[index ++] = cmd[i];
            space_flag = 0;
        }else if(flag == 1) {
           argv[(*len)][index ++] = cmd[i]; 
           space_flag = 0;
        }
    }
    if(flag == 0) {
        cmdline[index] = '\0';
    } else {
        argv[(*len) ++][index] = '\0';
    } 
    
}


fun find_cmd(char* cmdline) {
    for(size_t i = 0; i < sizeof(Commands) / sizeof(struct Command); i++) {
        if(strcmp(cmdline, Commands[i].name) == 0)
            return Commands[i].cmd_fun;
        if(Commands[i].shortcut == NULL) 
            continue;
        if(strcmp(cmdline, Commands[i].shortcut) == 0)
            return Commands[i].cmd_fun;
    }
    return NULL;
}

int run_command(char* cmd) {
    char cmdline[100];
    char argv[100][100];
    int argv_len = 0;
    parse_cmd(cmd, cmdline, argv, &argv_len);
    fun cmd_fun = find_cmd(cmdline);
    if(cmd_fun != NULL) {
        return cmd_fun(argv_len, argv);
    }else {
        printf("command not found \n");
        return 0;
    }
}

int quit(int argc, char argv[][100]) {
    if(program.pid != 0) {
        kill(program.pid, SIGTERM);
    }
    return 2;
}
