#ifndef _RUNCOMMAND_H
#define _RUNCOMMAND_H

#include <string.h>
#include <stdio.h>
#include <vector>
#include <sys/user.h>

#include "load.h"
#include "vmmap.h"
#include "start.h"
#include "break.h"
#include "help.h"
#include "cont.h"
#include "disasm.h"
#include "run.h"
#include "reg.h"

#define STATE_NONE 0X00
#define STATE_RUN 0X01
#define STATE_LOAD 0X02
#define STATE_ANY 0X03

using namespace std;

typedef int (*fun)(int, char[][100]); 

struct Command {
    char* name;
    char* shortcut;
    char* help_msg;
    int (*cmd_fun)(int, char [][100]);
};

struct elf {
    unsigned long long entry;
    unsigned long long virtual_address;
    unsigned long long offset;
    size_t size;
};

struct disasm {
    char* data;
    int offset;
    int size;
    unsigned long long cur_virtual_address;
    int dumpped;
};

struct program_content {
    char* program_name;
    int fd;
    unsigned int pid;
    struct elf load;

    int dynamic_flag;
    unsigned long long base_address;

    struct disasm asm_file;

    int break_num;
    vector<struct breakpoint> bp;
    int hit_breakpoint;
    unsigned long long hit_address;

    struct user_regs_struct regs;

    unsigned long long disasm_previous_address;
    bool disasm_previous_flag;

    unsigned long long dump_previous_address;
    bool dump_previous_flag;
};

extern struct Command Commands[];
extern int command_len;
extern int state;
extern struct program_content program;

int run_command(char*);
int quit(int, char[][100]);
#endif
