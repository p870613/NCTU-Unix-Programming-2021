#include "print_log.h"
/*#define BUFFER_SIZE 80000*/
void get_filename(const char* path, char* buf) {
	realpath(path, buf);
}

void get_file_pointer(FILE* fptr, char* buf) {
    int f = fileno(fptr);
    get_fd(buf, f);
}

void get_fd(char* buf, int fd) {
    char path[500], buf2[500];
    memset(buf2, '\0', 500);
    memset(buf, '\0', 500);
    memset(path, '\0', 500);
    snprintf(path, sizeof(path),"/proc/%d/fd/%d", getpid(), fd);
    readlink(path, buf2, 499);
    snprintf(buf, 499, "%s", buf2);
    buf[499] = '\0';
}

void check_string(char* str, int count) {
    for(int i = 0; i < count; i++){
        if(!isprint(str[i])){
            fprintf(output_file, "%c", '.');
            fflush(output_file);
        }
        else{
            fprintf(output_file, "%c", str[i]);
            fflush(output_file);
        } 
    }
}

void print_chmod(const char* path, mode_t mode, int re) {
    char buf[500];
    get_filename(path, buf);
    fprintf(output_file, "[logger] chmod(\"%s\", %o) = %d\n", buf, mode, re);
}

void print_chown(const char* path, uid_t o, gid_t g, int re){
    char buf[500];
    get_filename(path, buf);
    fprintf(output_file, "[logger] chown(\"%s\", %o, %o) = %d\n", buf, o, g, re);
}

void print_close(char* buf, int re) {
    fprintf(output_file, "[logger] close(\"%s\") = %d\n", buf, re);
}

void print_creat64(const char* path, mode_t mode, int re) {
    char buf[500];
    get_filename(path, buf);
    fprintf(output_file, "[logger] creat64(\"%s\", %o) = %d\n", buf, mode, re);
}
void print_creat(const char* path, mode_t mode, int re) {
    char buf[500];
    get_filename(path, buf);
    fprintf(output_file, "[logger] creat(\"%s\", %o) = %d\n", buf, mode, re);
}

void print_fclose(char* buf, int re) {
    fprintf(output_file, "[logger] fclose(\"%s\") = %d\n", buf, re);
}

void print_fopen64(const char* path, const char* mode, FILE* re) {
    char buf[500];
    get_filename(path, buf);
    fprintf(output_file, "[logger] fopen64(\"%s\", \"%s\") = %p\n", buf, mode, re);
}
void print_fopen(const char* path, const char* mode, FILE* re) {
    char buf[500];
    get_filename(path, buf);
    fprintf(output_file, "[logger] fopen(\"%s\", \"%s\") = %p\n", buf, mode, re);
}

void print_fread(char* buf, size_t s, size_t n, FILE* fp, size_t re) {
    char buf2[500];
    get_file_pointer(fp, buf2);
    
    char *t = "\0";
    fprintf(output_file, "[logger] fread(\"%s", t);
    fflush(output_file);
    check_string((char*)buf, s);
    fprintf(output_file, "\", %lu, %lu, \"%s\") = %lu\n", s, n, buf2, re);
    fflush(output_file);
}

void print_fwrite(char* buf, size_t s, size_t n, FILE* fp, size_t re) {
    char buf2[500];
    get_file_pointer(fp, buf2);
    
    char *t = "\0";
    fprintf(output_file, "[logger] fwrite(\"%s", t);
    fflush(output_file);
    check_string((char*)buf, n);
    fprintf(output_file, "\", %lu, %lu, \"%s\") = %lu\n", s, n, buf2, re);
    fflush(output_file);
}

void print_open64(const char* path, int flag, mode_t mode, int re){
    char buf[500];
    get_filename(path, buf);
    fprintf(output_file, "[logger] open64(\"%s\", %o, %o) = %d\n", buf, flag, mode, re);
}
void print_open(const char* path, int flag, mode_t mode, int re){
    char buf[500];
    get_filename(path, buf);
    fprintf(output_file, "[logger] open(\"%s\", %o, %o) = %d\n", buf, flag, mode, re);
}

void print_read(int fd, void* buf, size_t count, ssize_t re) {
    char buf2[500];
    get_fd(buf2, fd);
    /*fprintf(output_file, "[logger] read(\%s\", \"%s\", %lu) = %ld\n", buf2, (char*)buf, count, re);*/
    fprintf(output_file, "[logger] read(\"%s\", \"", buf2);
    fflush(output_file);
    check_string((char*)buf, count);
    fprintf(output_file, "\", %lu) = %ld\n",  count, re);
    fflush(output_file);
}

void print_remove(char* buf, int re) {
    fprintf(output_file, "[logger] remove(\"%s\") = %d\n", buf, re);
}

void print_rename(char *old, char *n, int re) {
    fprintf(output_file, "[logger] rename(\"%s\", \"%s\") = %d\n", old, n, re);
}

void print_tmpfile64(FILE* re) {
    fprintf(output_file, "[logger] tmpfile64() = %p\n", re);    
}
void print_tmpfile(FILE* re) {
    fprintf(output_file, "[logger] tmpfile() = %p\n", re);    
}

void print_write(int fd, const void* buf, size_t count, ssize_t re) {
    char buf2[500];
    get_fd(buf2, fd);
 
    /*fprintf(output_file, "[logger] read(\%s\", \"%s\", %lu) = %ld\n", buf2, (char*)buf, count, re);*/
    fflush(output_file);
    fprintf(output_file, "[logger] write(\"%s\", \"", buf2);
    fflush(output_file);
    check_string((char*)buf, count);
    fprintf(output_file, "\", %lu) = %ld\n",  count, re);
    fflush(output_file);
}
