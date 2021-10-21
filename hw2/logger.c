#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdarg.h>
#include <string.h>
#include "print_log.h"
#include <fcntl.h>
/*#define BUFFER_SIZE 80000*/
static int (* old_chmod)(const char*, mode_t) = NULL;
static int (* old_chown)(const char *, uid_t, gid_t) = NULL;
static int (* old_close)(int) = NULL;
static int (* old_creat)(const char*, mode_t) = NULL;
static int (* old_fclose)(FILE *) = NULL;
static FILE* (* old_fopen)(const char*, const char*) = NULL;
static size_t (* old_fread)(void *, size_t , size_t , FILE *) = NULL;
static size_t (* old_fwrite)(const void *, size_t , size_t , FILE *) = NULL;
static int (* old_open)(const char*, int, ...) = NULL;
static ssize_t (* old_read)(int, void *, size_t ) = NULL;
static int (* old_remove)(const char *) = NULL;
static int (* old_rename)(const char *, const char *) = NULL;
static FILE* (*old_tmpfile)(void) = NULL;
static ssize_t (* old_write)(int , const void *, size_t) = NULL;


static FILE* (*old_tmpfile64)(void) = NULL;
static int (* old_open64)(const char*, int, ...) = NULL;
static int (* old_creat64)(const char*, mode_t) = NULL;
static FILE* (* old_fopen64)(const char*, const char*) = NULL;
FILE* output_file = NULL;

void find_fptr(void** fun, const char* libc_fun) {
    if(*fun == NULL){
        *fun = dlsym(RTLD_NEXT, libc_fun);
    }

    if(output_file == NULL) {
        if(getenv("output_file") != NULL) {
            int f = atoi(getenv("output_file"));
            output_file = fdopen(f, "w");
        }else {
            output_file = stderr;
        }
    }
    
}

int chmod(const char *pathname, mode_t mode) {
    find_fptr((void**)&old_chmod, "chmod");
    int re;
    if(old_chmod != NULL){
        re = old_chmod(pathname, mode);
        print_chmod(pathname, mode, re);
    }
    return re;
}

int chown(const char *pathname, uid_t owner, gid_t group) {
    find_fptr((void**)&old_chown, "chown");
    int re;
    if(old_chown != NULL){
        re = old_chown(pathname, owner, group);
        print_chown(pathname, owner, group, re);
    } 
    return re;
}

int close(int fd) {
    find_fptr((void**)&old_close, "close");
    
    int re;
    if(old_close != NULL) {
        int err;
        if(fd == 2)
            err = dup(2);

        char buf[500];
        get_fd(buf, fd);
        re = old_close(fd);
        
        if(fd == 2)
            dup2(err, 2);
        
        print_close(buf, re);

    } 
    return re;
}

int creat64(const char *path, mode_t mode) {
    if(old_creat64 == NULL)
        find_fptr((void**)&old_creat64, "creat64");
    
    int re;
    if(old_creat64 != NULL) {
        re = old_creat64(path, mode);
        print_creat64(path, mode, re);
    }
    return re;
}
int creat(const char *path, mode_t mode) {
    if(old_creat == NULL)
        find_fptr((void**)&old_creat, "creat");
    
    int re;
    if(old_creat != NULL) {
        re = old_creat(path, mode);
        print_creat(path, mode, re);
    }
    return re;
}

int fclose(FILE *fp) {
    if(old_fclose == NULL)
        find_fptr((void**)&old_fclose, "fclose");

    int re;
    if(old_fclose != NULL) {
        int err, close_fd = fileno(fp), out_fd = fileno(output_file);

        if(close_fd == 2){
            err = dup(2);
        }
        char buf[500];
        get_file_pointer(fp, buf);
        re = old_fclose(fp);
        
        if(out_fd == 2){
            dup2(err, 2);
            stderr = fdopen(2, "w");
            output_file = stderr;
        } 
        print_fclose(buf, re);
    }
    return re;
}

FILE* fopen64(const char *path, const char *mode) {
    if(old_fopen64 == NULL)
        find_fptr((void**)&old_fopen64, "fopen64");

    FILE* re;
    if(old_fopen64 != NULL) {
        re = old_fopen64(path, mode);
        print_fopen64(path, mode, re);  
    }
    return re;
}

FILE* fopen(const char *path, const char *mode) {
    if(old_fopen == NULL)
        find_fptr((void**)&old_fopen, "fopen");

    FILE* re;
    if(old_fopen != NULL) {
        re = old_fopen(path, mode);
        print_fopen(path, mode, re);  
    }
    return re;
}

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    if(old_fread == NULL) 
        find_fptr((void**)&old_fread, "fread");
    size_t re;
    if(old_fread != NULL) {
        re = old_fread(ptr, size, nmemb, stream);
        print_fread((char*)ptr, size, nmemb, stream, re);
    }
    return re;
}

size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) {
    if(old_fwrite == NULL) 
        find_fptr((void**)&old_fwrite, "fwrite");
    
    size_t re;
    if(old_fwrite != NULL) {
        re = old_fwrite(ptr, size, nmemb, stream);
        print_fwrite((char*)ptr, size, nmemb, stream, re);
    }
    return re;
}

int open64(const char *path, int oflag, ...) {
    find_fptr((void**)&old_open64, "open64");
    
    int mode = 0;
    if (__OPEN_NEEDS_MODE (oflag)){
      va_list arg;
      va_start (arg, oflag);
      mode = va_arg (arg, int);
      va_end (arg);
    }

    int re;
    if(old_open64 != NULL){
        re = old_open64(path, oflag, mode);
        print_open64(path, oflag, mode, re); 
    }
    return re;
}

int open(const char *path, int oflag, ...) {
    find_fptr((void**)&old_open, "open");
    
    int mode = 0;
    if (__OPEN_NEEDS_MODE (oflag)){
      va_list arg;
      va_start (arg, oflag);
      mode = va_arg (arg, int);
      va_end (arg);
    }

    int re;
    if(old_open != NULL){
        re = old_open(path, oflag, mode);
        print_open(path, oflag, mode, re); 
    }
    return re;
}

ssize_t read(int fd, void *buf, size_t count) {
    find_fptr((void**)&old_read, "read");

    ssize_t re;
    if(old_read != NULL){
        re = old_read(fd, buf, count);
        print_read(fd, buf, count, re);
    }
    return re;
}

int remove(const char *pathname) {
    if(old_remove == NULL)
        find_fptr((void**)&old_remove, "remove");

    int re;
    if(old_remove != NULL) {
        char buf[500];
        get_filename(pathname, buf);
        re = old_remove(pathname); 
        print_remove(buf, re);
    }
    return re;
}

int rename(const char *old, const char *n) {
    if(old_rename == NULL)
        find_fptr((void**) &old_rename, "rename");

    int re;
    if(old_rename != NULL) {
        char buf1[500], buf2[500];
        get_filename(old, buf1);
        re = old_rename(old, n);
        get_filename(n, buf2);
        print_rename(buf1, buf2, re);
    }
    return re;
}


FILE* tmpfile64(void) {
    if(old_tmpfile64 == NULL)  
        find_fptr((void**)&old_tmpfile64, "tmpfile64");
    
    FILE* re;
    if(old_tmpfile64 != NULL) {
        re = old_tmpfile64();
        print_tmpfile64(re);
    }
    return re;
    
}
FILE* tmpfile(void) {
    if(old_tmpfile == NULL)  
        find_fptr((void**)&old_tmpfile, "tmpfile");
    
    FILE* re;
    if(old_tmpfile != NULL) {
        re = old_tmpfile();
        print_tmpfile(re);
    }
    return re;
    
}

ssize_t write(int fd, const void *buf, size_t count) {
    find_fptr((void**)&old_write, "write");

    ssize_t re;
    /*char tmp[BUFFER_SIZE];*/
    if(old_read != NULL){
        re = old_write(fd, buf, count);
        /*strcpy(tmp, buf);*/
        print_write(fd, buf, count, re);
    }
    return re;
}

