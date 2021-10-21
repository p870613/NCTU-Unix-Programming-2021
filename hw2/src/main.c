#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>
char output_file[500];
int output_file_flag = 0;
char ld_preload[500] = "./logger.so";
char *exe_arg[50];
int exe_index;

int main(int argc, char *argv[]) {
    if(argc < 2) {
        printf("no command given.\n");
        return 0;
    }
    
    char c;
    while ((c = getopt(argc, argv, "o:p:")) != -1) {
        switch (c) {
            case 'o':
                strcpy(output_file, optarg);
                output_file_flag = 1;
                break;
            case 'p':
                strcpy(ld_preload, optarg);
                break;
            default:
                printf("usage: ./logger [-o file] [-p sopath] [--] cmd [cmd args ...]\n");
                printf("-p: set the path to logger.so, default = ./logger.so\n");
                printf("-o: print output to file, print to \"stderr\" if no file specified\n");
                printf("--: separate the arguments for logger and for the command\n");
                break;
        }
    }
    
    for(int i = optind; i < argc; i++) {
        exe_arg[exe_index++] = argv[i];
    }
    /*int fd = open(output_file, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);*/
    /*dup2(fd, 2);*/
    /*setenv("LD_PRELOAD", ld_preload, 1);*/
    /*if(execvp(argv[optind], exe_arg) == -1) {*/
        /*exit(0);*/
    /*}*/
    pid_t pid = fork();
    if(pid == 0) {
        char t[10];
        if(output_file_flag == 1) {
            int fd = open(output_file, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
            snprintf(t, 10, "%d", fd);
            setenv("output_file", t, 1);
        }
        setenv("LD_PRELOAD", ld_preload, 1);
        if(execvp(argv[optind], exe_arg) == -1) {
            exit(0);
        }
    }else if(pid > 0) {
        while(waitpid(pid, NULL, WNOHANG) != -1);
    }
}
