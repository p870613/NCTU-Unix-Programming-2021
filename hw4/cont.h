#ifndef _CONT_H
#define _CONT_H

#include <sys/ptrace.h>
#include <unistd.h>
#include <sys/wait.h>
#include <capstone/capstone.h>

#include "reg.h"
#include "runcmd.h"
#include "disasm.h"

int cont(int, char [][100]);
int check_breakpoint();
int si(int, char[][100]);

#endif
