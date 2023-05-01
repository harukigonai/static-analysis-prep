#include <unistd.h>
#include <sys/auxv.h>
#include "lib_wrapper/arg_struct.h"

int main(void) {
    syscall(543);
    syscall(666, getauxval(AT_SYSINFO_EHDR) - sysconf(_SC_PAGESIZE));
    syscall(777);

    /*
    struct lib_enter_args args = {0};
    syscall(888, &args);
    syscall(889);
    */

    char *argv[] = { "/root/static-analysis-prep/untrusted_lib_tester/main", 0 };
    char *envp[] = { "LD_PRELOAD=/root/static-analysis-prep/untrusted_lib_tester/lib_wrapper/libfunc.so", 0 };
    execve(argv[0], argv, envp);
}
