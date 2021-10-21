#ifndef _DISASM_H
#define _DISASM_H

#include <stdio.h>
#include <stdlib.h>
#include <capstone/capstone.h>
#include <ctype.h>

#include "runcmd.h"
#include "break.h"
#define DISASM_LEN 10

int dis_asm(int, char[][100]);
int dump(int, char[][100]);

#endif

