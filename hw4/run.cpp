#include "run.h"

int run(int argc, char argv[][100]) {
    if(state == STATE_RUN) {
        printf("** program %s is already running.\n", program.program_name);
        char t[][100] = {NULL};
        cont(0, t);
        return 0;
    } else if(state == STATE_LOAD) {
        char tmp[][100] = {NULL};
        start(0, tmp);
        cont(0, tmp);
        return 0;
    } else {
        printf("** state must be LOADED or RUNNING.\n");
        return 0;
    }
}
