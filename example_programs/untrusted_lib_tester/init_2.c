#include <unistd.h>
#include <sys/auxv.h>
#include "lib_wrapper/arg_struct.h"

int main(void) {
	syscall(543);
	syscall(666, getauxval(AT_SYSINFO_EHDR) - sysconf(_SC_PAGESIZE));
	syscall(777);

	/* Maybe not 100% correct but seems to fix some page faults */
	struct lib_enter_args args = {0};
	syscall(888, &args);
	syscall(889);

	char *argv[] = { "/root/static-analysis-prep/example_programs/2/untrusted_lib_tester/main", 0 };
	char *envp[] = { 0 };
        execve(argv[0], argv, envp);
}
