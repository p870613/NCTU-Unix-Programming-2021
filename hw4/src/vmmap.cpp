#include "vmmap.h"

#define VMMAP_FORMAT "%016llx-%016llx %c%c%c %010llx %s\n"
int vmmap(int argc, char argv[][100]) {
    if(state == STATE_NONE) {
        printf("** Error, the program must be loaded or started before vmmap\n");
        return 0;
    }

    if(state == STATE_LOAD) {
        printf(VMMAP_FORMAT, program.load.entry, program.load.entry + program.load.size, 'r', '-', 'x', program.load.offset, program.program_name);
        return 0;
    } else if (state == STATE_RUN){
        char path[100];
        snprintf(path, 100, "/proc/%d/maps", program.pid);
        FILE* f = fopen(path, "r");
        unsigned long long addr, endaddr, offset, inode;
        char premission[100], device[100], filename[100];
       
        while(true) {
            int ret = fscanf(f, "%llx-%llx %s %llx %s %llx", &addr, &endaddr, premission ,&offset, device, &inode);
            if(!(ret != 0 && ret != EOF))
                break;
            if(ret > 0 && ret != EOF && inode != 0){
                ret += fscanf(f, "%s\n", filename);
                if(!(ret != 0 && ret != EOF))
                    break;
            } else {
              char buf[255];
              filename[0] = '\0';	/* no filename */
              fgets(buf, 255, f);
              sscanf(buf, "%s\n", filename);
            } 
            printf(VMMAP_FORMAT, addr, endaddr, premission[0], premission[1], premission[2], inode, filename);

        }
        fclose(f);
        return 0;
    }
    return -1;
}

