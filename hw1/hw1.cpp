#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/stat.h>
#include <ctype.h>
#include <string.h>
#include <regex.h>
#include <map>

#define BUF_SIZE 1024
#define CMDLINE_SIZE 100
#define USER_SIZE 100
#define PATH_SIZE 100
#define FORMAT "%-50s %-10s %-20s %-10s %-10s %-10s %s %s\n"

bool cmdline_flag = false;
bool filename_flag = false;
bool type_flag = false;
char cmdline_arg[100];
char filename_arg[100];
char type_arg[100];

void dump_info(char*);
bool is_pid(char*);

struct pid_info {
    pid_t pid;
    char user[USER_SIZE];
    char cmdline[CMDLINE_SIZE];
    char path[PATH_SIZE];
    ssize_t len;
};

void print_header(){
    printf(FORMAT, "COMMAND", "PID", "USER", "FD", "TYPE", "NODE", "NAME", "");
}

bool is_pid(char* str){
    for(int i = 0; i < int(strlen(str)); i++)
        if(isdigit(str[i]))
            return false;
    return true;
}

void get_cmdline(pid_info* info){
    char path[150];
    snprintf(path, sizeof(path), "%s%s", info->path, "comm");
    
    int fd = open(path, O_RDONLY);
    if(fd == 0){
        fprintf(stderr, "Couldn't read %s\n", path);
        return;
    }

    char loss[10];
    char input[100];
    int STDIN = dup(STDIN_FILENO);

    dup2(fd, STDIN_FILENO);   
    scanf("%s", input);
    strcpy(info->cmdline, input);
    
    fflush(stdin); 
    fflush(stdout); 
    
    dup2(STDIN, STDIN_FILENO);
    close(fd);
}

bool regex_check(const char *exp, const char *str){
    regex_t regex;
    regcomp(&regex, exp, 0);
    int reti = regexec(&regex, str, 0, NULL, 0);
    if (reti == 0) 
        return true;
    else
        return false;
}

void get_user(char* path, char user[][USER_SIZE]){
    struct stat pid_stat;
    struct passwd *pw;
    
    if(!stat(path, &pid_stat)){
        pw = getpwuid(pid_stat.st_uid);
        if(pw){
            strncpy(*user, pw->pw_name, USER_SIZE);           
        }
    }
}

ino_t get_ino(const char* path){
    struct stat s;
    if(stat(path, &s) != 0)
        return 0;
    return s.st_ino;
}

int is_dir(const char* path){
    struct stat s;
    if(stat(path, &s) != 0)
        return 0;
    return S_ISDIR(s.st_mode);
}

int is_chr(const char* path){
    struct stat s;
    if(stat(path, &s) != 0)
        return 0;
    return S_ISCHR(s.st_mode);
}
int is_fifo(const char* path){
    struct stat s;
    if(stat(path, &s) != 0)
        return 0;
    return S_ISFIFO(s.st_mode);
}
int is_sock(const char* path){
    struct stat s;
    if(stat(path, &s) != 0)
        return 0;
    return S_ISSOCK(s.st_mode);
}
int is_reg(const char* path){
    struct stat s;
    if(stat(path, &s) != 0)
        return 0;
    return S_ISREG(s.st_mode);
}

void get_type(char* link_dest, char *type){
    if(is_dir(link_dest))
        strcpy(type, "DIR");
    else if(is_reg(link_dest))
        strcpy(type, "REG");
    else if(is_chr(link_dest))
        strcpy(type, "CHR");
    else if(is_fifo(link_dest))
        strcpy(type, "FIFO");
    else if(is_sock(link_dest))
        strcpy(type, "SOCK");
    else 
        strcpy(type, "unknown");
}

void print_type(const char* fd, pid_info *info){
    char path[100] = "", type[10] = "", node[10] = "", pid[10] = "", name[200] = "", err_msg[50] = ""; 
    char link_dest[PATH_MAX];
    ssize_t link_dest_size;
    
    snprintf(path, sizeof(path), "%s%s", info->path, fd);
    snprintf(pid, sizeof(pid), "%d", info->pid);
    
    if((link_dest_size = readlink(path, link_dest, sizeof(link_dest)-1)) < 0) {
        snprintf(name, sizeof(name), "%s", path);
        snprintf(err_msg, sizeof(err_msg), "(%s)", "readlink: Permission denied");
        strcpy(type, "unknown");
        node[0] = '\0';
    } else {
        link_dest[link_dest_size] = '\0';
        get_type(path, type);
        snprintf(node, sizeof(node), "%lu", get_ino(path));
        strcpy(name, link_dest);
    }

    if(type_flag == true)
        if(strcmp(type, type_arg))
            return;
    
    if(filename_flag == true)
        if(!regex_check(filename_arg, name))
            return;
    
    printf(FORMAT, info->cmdline, pid, info->user, fd, type, node, name, err_msg);
}   

void map_parse(char* line, char* ino, char* name){
    int space_count = 0;
    int ino_index = 0, name_index = 0;
    for(int i = 0; i < strlen(line); i++){
        if(space_count == 4)
            ino[ino_index++] = line[i];
        
        if(line[i] == ' ')
            space_count ++;

        if(space_count > 4){
            if(line[i] == ' ' || line[i] == '\n')
                continue;
            
            name[name_index++] = line[i];
        }
    }
    if(ino[ino_index] != '\0')
        ino[ino_index] = '\0';
    if(name[name_index] != '\0')
        name[name_index] = '\0';
}

bool check_del(char* name){
    if(regex_check("deleted", name)){
         name[strlen(name)-9] = '\0';
         return true;
    }
    return false;
}

void print_maps(pid_info *info){
    char path[110] = "", name[100], ino[100], pid[10], fd[10], type[10], err_msg[50];
    std::map<int, int> inode;
    FILE *map_fd;

    snprintf(path, sizeof(path), "%s%s", info->path, "maps");
    snprintf(pid, sizeof(pid), "%d", info->pid);
    
    map_fd = fopen(path, "r");
    if(!map_fd)
        return;
    else{
        char line[200];
       
        while (fgets(line, sizeof(line), map_fd)) {
            err_msg[0] = '\0';
            map_parse(line, ino, name);
            
            if(inode.find(atoi(ino)) != inode.end() || atoi(ino) == 0)
                continue;
            inode[atoi(ino)] = 1; 

            if(check_del(name) == true){
                strcpy(fd, "del");
                strcpy(type, "unknown");
                snprintf(err_msg, sizeof(err_msg), "(%s)", "deleted");
            }else{
                strcpy(fd, "mem");
                get_type(name, type);
            }
            
            if(type_flag == true){
                if(strcmp(type, type_arg))
                    continue;
            } 
            
            if(filename_flag == true){
                if(!regex_check(filename_arg, name))
                    continue;
            }

            printf(FORMAT, info->cmdline, pid, info->user, fd, type, ino, name, err_msg);
        }
        fclose(map_fd);
    }
}

void get_fd(char* path, char* filename, char* fd){
    struct stat s;
    if(lstat(path, &s) == -1)
        return;

    if((s.st_mode & S_IREAD) && (s.st_mode & S_IWRITE))
        snprintf(fd, sizeof(fd), "%s%s", filename, "u");
    else if(s.st_mode & S_IRUSR)
        snprintf(fd, sizeof(fd), "%s%s", filename, "r");
    else if(s.st_mode & S_IWUSR)
        snprintf(fd, sizeof(fd), "%s%s", filename, "w");
}

void socket_pipe_get_type(char* type, char* path){ 
    if(regex_check("socket", path)){
        strcpy(type, "SOCK");
    }else if(regex_check("pipe", path)){
        strcpy(type, "FIFO");
    }
}

void print_fd(pid_info *info){
    char path[110] = "", pid[10] = "", fd[10] = "", name[200] = "", ino[100] = "", type[10] = "", err_msg[50] = "";
     
    snprintf(path, sizeof(path), "%s%s", info->path, "fd"); 
    snprintf(pid, sizeof(pid), "%d", info->pid); 
    DIR* dir = opendir(path);
    if(!dir){
        snprintf(name, sizeof(name), "%s", path); 
        snprintf(err_msg, sizeof(err_msg), "(%s)" ,"opendir: Permission denied"); 
        
        strcpy(fd, "NOFD");
        
        if(type_flag == true)
            return;
        
        if(filename_flag == true){
            if(!regex_check(filename_arg, path))
                return;
        }
        ino[0] = '\0';
        printf(FORMAT, info->cmdline, pid, info->user, fd, type, ino, name, err_msg);
    }else{
        struct dirent* de;
        while((de = readdir(dir))){
            err_msg[0] = '\0';
            if(!strcmp(de->d_name, ".") || !strcmp(de->d_name, ".."))
                continue;
            
            char fd_path[500];
            char link_dest[100];
            int link_dest_size;

            snprintf(fd_path, sizeof(fd_path), "%s/%s", path, de->d_name);
            
            if((link_dest_size = readlink(fd_path, link_dest, sizeof(link_dest)-1)) < 0){
                snprintf(name, sizeof(name), "%s(%s)", path, strerror(errno));
                strcpy(type, "unknown");
            } else {
                link_dest[link_dest_size] = '\0';
                
                get_fd(fd_path, de->d_name, fd);
                get_type(fd_path, type);
                snprintf(ino, sizeof(ino), "%lu", get_ino(fd_path));
                strcpy(name, link_dest);
                
                if(!strcmp(type, "unknown")){
                   socket_pipe_get_type(type, name);
                   snprintf(ino, sizeof(ino), "%lu", get_ino(fd_path));
                }

                if(check_del(name) == true){
                    snprintf(err_msg, sizeof(err_msg), "(%s)", "deleted");
                }

                if(type_flag == true)
                    if(strcmp(type, type_arg))
                        continue;
                

                if(filename_flag == true)
                    if(!regex_check(filename_arg, name))
                        continue;
                
            }

            printf(FORMAT, info->cmdline, pid, info->user, fd, type, ino, name, err_msg);
        }
        closedir(dir);
    }
}

void dump_info(char* pid){
    struct pid_info info;

    info.pid = atoi(pid);
    snprintf(info.path, sizeof(info.path), "/proc/%d/", info.pid);
    info.len = strlen(info.path);
    
    get_user(info.path, &info.user);
    get_cmdline(&info);

    if(cmdline_flag == true)
        if(!regex_check(cmdline_arg, info.cmdline))
            return;

    
    print_type("cwd", &info);
    print_type("root", &info);
    print_type("exe", &info);
    print_maps(&info);
    print_fd(&info);
}

int main(int argc, char** argv){
    int opt;
    if(argc > 1){
        while((opt = getopt(argc, argv, "c:t:f:")) != -1){
            switch(opt) {
                case 'c':
                    cmdline_flag = true;
                    strcpy(cmdline_arg, optarg);
                    break;
                case 't':
                    if(!strcmp(optarg, "REG") || !strcmp(optarg, "CHR") || 
                       !strcmp(optarg, "DIR") || !strcmp(optarg, "FIFO") ||
                       !strcmp(optarg, "SOCK") || !strcmp(optarg, "unknown"))
                    {
                        type_flag = true;
                        if(optarg)
                            strcpy(type_arg, optarg);
                        else
                            return 1;
                           
                    }else{
                        printf("Invalid TYPE option.\n");
                        return 0;
                    }
                    break;
                case 'f':
                    filename_flag = true;
                    strcpy(filename_arg, optarg);
                    break;
            }
        }
    }

    DIR *dir = opendir("/proc");
    struct dirent* de;
    print_header();
    while((de = readdir(dir))){
        if(!strcmp(de->d_name, ".") || !strcmp(de->d_name, "..") || is_pid(de->d_name))
            continue;
        dump_info(de->d_name);            
    }
    closedir(dir);
}
