#ifndef _BREAK_H
#define _BREAK_H

#include <stdlib.h>
#include <stdio.h>

#include "runcmd.h"

struct breakpoint {
    int id;
    unsigned long long address;
    unsigned char origin_cmd;
    bool fix_flag;
    breakpoint(int _i = -1, unsigned long long _a = 0, unsigned long long _o = '\0', bool _f = false)
        : id(_i), address(_a), origin_cmd(_o), fix_flag(_f) {
        }
};

bool address_in_text(unsigned long long);
unsigned char change_byte(unsigned long long, unsigned char);
int brk(int, char[][100]);
int list(int, char[][100]);
int del(int, char [][100]);

#endif
