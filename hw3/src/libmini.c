#include"libmini.h"

long errno;

#define WRAPPER_RETVAL(type) errno = 0; if(ret < 0) { errno = -ret; return -1;} return ((type) ret);
#define WRAPPER_RETPTR(type) errno = 0; if(ret < 0) { errno = -ret; return NULL;} return ((type) ret);

ssize_t read(int fd, char* buf, size_t count) {
    long ret = sys_read(fd, buf, count);
    WRAPPER_RETVAL(ssize_t);
}

ssize_t write(int fd, const void *buf, size_t count) {
    long ret = sys_write(fd, buf, count);
    WRAPPER_RETVAL(ssize_t);
}

int close(int fd) {
    long ret = sys_close(fd);
    WRAPPER_RETVAL(int);
}

void* mmap(void* address, size_t len, int prot, int flags, int fd, off_t off) {
    long ret = sys_mmap(address, len, prot, flags, fd, off);
    WRAPPER_RETPTR(void*);
}

int mprotect(void* address, size_t len, int prot) {
    long ret = sys_mprotect(address, len, prot);
    WRAPPER_RETVAL(int);
}

int munmap(void* address, size_t len) {
    long ret = sys_munmap(address, len);
    WRAPPER_RETVAL(int);
}

int pipe(int* fd) {
    long ret = sys_pipe(fd);
    WRAPPER_RETVAL(int);
}

int dup(int fd) {
    long ret = sys_dup(fd);
    WRAPPER_RETVAL(int);
}

int dup2(int oldfd, int newfd) {
    long ret = sys_dup2(oldfd, newfd);
    WRAPPER_RETVAL(int);
}

int pause() {
    long ret = sys_pause();
    WRAPPER_RETVAL(int);
}

int nanosleep(struct timespec* rqtp, struct timespec *rmtp) {
    long ret = sys_nanosleep(rqtp, rmtp);
    WRAPPER_RETVAL(int);
}

pid_t fork(void) {
    long ret = sys_fork();
    WRAPPER_RETVAL(pid_t);
}

void exit(int err_code) {
    sys_exit(err_code);
}

char* getcwd(char* buf, size_t size) {
    long ret = sys_getcwd(buf, size);
    WRAPPER_RETPTR(char*);
}

int chdir(const char* path) {
    long ret = sys_chdir(path);
    WRAPPER_RETVAL(int);
}

int rename(const char* old, const char* new) {
    long ret = sys_rename(old, new);
    WRAPPER_RETVAL(int);
}

int mkdir(const char* dirname, mode_t mode) {
    long ret = sys_mkdir(dirname, mode); 
    WRAPPER_RETVAL(int);
}

int rmdir(const char* path) {
    long ret = sys_rmdir(path);
    WRAPPER_RETVAL(int);
}

int creat(const char* path, mode_t mode) {
    long ret = sys_creat(path, mode);
    WRAPPER_RETVAL(int);
}

int link(const char* old, const char* new) {
    long ret = sys_link(old, new);
    WRAPPER_RETVAL(int);
}

int unlink(const char*  path) {
    long ret = sys_unlink(path);
    WRAPPER_RETVAL(int);
}

ssize_t readlink(const char* path, char* buf, size_t size) {
    long ret = sys_readlink(path, buf, size);
    WRAPPER_RETVAL(ssize_t);
}

int chmod(const char* filename, mode_t mode) {
    long ret = sys_chmod(filename, mode);
    WRAPPER_RETVAL(int);
}

int chown(const char* filename, uid_t uid, gid_t gid) {
    long ret = sys_chown(filename, uid, gid);
    WRAPPER_RETVAL(int);
}

int umask(int mask) {
    long ret = sys_umask(mask);
    WRAPPER_RETVAL(int);
}

int gettimeofday(struct timeval* tv, struct timezone* tz) {
    long ret = sys_gettimeofday(tv, tz);
    WRAPPER_RETVAL(int);
}

uid_t getuid() {
    long ret = sys_getuid();
    WRAPPER_RETVAL(uid_t);
}

gid_t getgid() {
    long ret = sys_getgid();
    WRAPPER_RETVAL(gid_t);
}

int setuid(uid_t uid) {
    long ret = sys_setuid(uid);
    WRAPPER_RETVAL(int);
}

int setgid(gid_t gid) {
    long ret = sys_setgid(gid);
    WRAPPER_RETVAL(int);
}

uid_t geteuid() {
    long ret = sys_geteuid();
    WRAPPER_RETVAL(uid_t);
}

gid_t getegid() {
    long ret = sys_getegid();
    WRAPPER_RETVAL(gid_t);
}

void bzero(void *s, size_t size) {
	char *ptr = (char *) s;
	while(size-- > 0) *ptr++ = '\0';
}

size_t strlen(const char *s) {
	size_t count = 0;
	while(*s++) count++;
	return count;
}

void* memset(void* address, int val, size_t size) {
    char* ptr = (char*) address;
    while(size --)
        *(ptr ++) = val;
    return address;
}

static const char *errmsg[] = {
	"Success",
	"Operation not permitted",
	"No such file or directory",
	"No such process",
	"Interrupted system call",
	"I/O error",
	"No such device or address",
	"Argument list too long",
	"Exec format error",
	"Bad file number",
	"No child processes",
	"Try again",
	"Out of memory",
	"Permission denied",
	"Bad address",
	"Block device required",
	"Device or resource busy",
	"File exists",
	"Cross-device link",
	"No such device",
	"Not a directory",
	"Is a directory",
	"Invalid argument",
	"File table overflow",
	"Too many open files",
	"Not a typewriter",
	"Text file busy",
	"File too large",
	"No space left on device",
	"Illegal seek",
	"Read-only file system",
	"Too many links",
	"Broken pipe",
	"Math argument out of domain of func",
	"Math result not representable"
};

void perror(const char *prefix) {
    const char *unknown = "Unknown";
    long backup = errno;
    if(prefix) {
        write(2, prefix, strlen(prefix));
        write(2, ": ", 2);
    }
    if(errno < PERRMSG_MIN || errno > PERRMSG_MAX) write(2, unknown, strlen(unknown));
    else write(2, errmsg[backup], strlen(errmsg[backup]));
    write(2, "\n", 1);
    return;
}

unsigned int alarm(unsigned int seconds) {
    long ret = sys_alarm(seconds);
    WRAPPER_RETVAL(unsigned int);
}

int sigpending(sigset_t* set) {
    long ret = sys_rt_sigpending(set, SIGSET_SIZE);
    WRAPPER_RETVAL(int);
}

int sigemptyset(sigset_t* set) {
    if(set == NULL) {
        set_errno(EINVAL);
        return -1;
    }
    memset(set, 0, SIGSET_SIZE);
    return 0;
}

int sigfillset(sigset_t* set) {
    if(set == NULL) {
        set_errno(EINVAL);
        return -1;
    }
    memset(set, 0xff, SIGSET_SIZE);
    return 0;
}

int sigaddset(sigset_t* set, int signal) {
    if(set == NULL || signal <= 0 || signal > SIGNAL_MAX) {
        set_errno(EINVAL);
        return -1;
    }
    *set = *set | (sigmask(signal));
    return 0;
}

int sigdelset(sigset_t* set, int signal) {
    if(set == NULL || signal <= 0 || signal > SIGNAL_MAX) {
        set_errno(EINVAL);
        return -1;
    }
    *set = *set & (~sigmask(signal));
    return 0;
}

int sigismember(const sigset_t* set, int signal) {
    if(set == NULL || signal <= 0 || signal > SIGNAL_MAX) {
        set_errno(EINVAL);
        return -1;
    }
    int s = sigmask(signal);
    return ((*set & s) != 0);
}

int sigprocmask(int how, const sigset_t* set, sigset_t* oldset) {
    long ret = sys_rt_sigprocmask(how, set, oldset, SIGSET_SIZE);
    WRAPPER_RETVAL(int);
}

int sigaction(int signal, struct sigaction* act, struct sigaction* old_act) {
    act->sa_flags = act->sa_flags | SA_RESTORER;
    act->sa_restorer = myrt;
    long ret = sys_rt_sigaction(signal, act, old_act, SIGSET_SIZE);
    WRAPPER_RETVAL(int);
}

sighandler_t signal(int signal, sighandler_t handler) {
    if(handler == SIG_ERR || signal < 0 || signal > SIGNAL_MAX){
        set_errno(EINVAL);
        return SIG_ERR;
    }
    struct sigaction act, old_act;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, signal);
    act.sa_handler = handler;
    act.sa_flags = SA_RESTART;
    if(sigaction(signal, &act, &old_act) < 0)
        return SIG_ERR;
    return old_act.sa_handler;
}

void sigsetjmp(jmp_buf env) {
    sigset_t old;
    long ret = sys_rt_sigprocmask(SIG_BLOCK, NULL, &old, SIGSET_SIZE);
    if(ret < 0) {
        set_errno(-ret);
        perror("signal mask");
    }
    env->mask = old;
    return;
}

void siglongjmp(jmp_buf env) {
    long ret = sys_rt_sigprocmask(SIG_SETMASK, &(env->mask), NULL, SIGSET_SIZE);
    if(ret < 0){
        set_errno(-ret);
        perror("signal mask");
    }
    return;
}

/*unsigned int sleep(unsigned int seconds) {*/
    /*long ret;*/
    /*struct timespec req, rem;*/
    /*req.tv_sec = seconds;*/
    /*req.tv_nsec = 0;*/
    /*ret = sys_nanosleep(&req, &rem);*/
    /*if(ret >= 0) return ret;*/
    /*if(ret == -EINTR) {*/
        /*return rem.tv_sec;*/
    /*}*/
    /*return 0;*/
/*}*/
