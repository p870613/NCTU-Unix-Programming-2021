#ifndef _START_H
#define _START_H

#include <sys/ptrace.h>
#include <unistd.h>
#include <sys/wait.h>

#include "runcmd.h"

int start(int, char[][100]);

#endif
