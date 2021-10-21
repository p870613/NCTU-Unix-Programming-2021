#include "help.h"

int help(int argc, char argv[][100]) {
    for(int i = 0; i < command_len; i++) {
        printf("%s\n", Commands[i].help_msg);
    }
    return 0;
}
