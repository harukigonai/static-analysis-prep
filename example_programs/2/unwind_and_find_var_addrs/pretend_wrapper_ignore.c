#include "unwind_and_find_var_addrs.h"
#define _GNU_SOURCE

#include <stdio.h>
#include <dlfcn.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/pem.h>
#include <openssl/pkcs12.h>
#include <openssl/ui.h>
#include <openssl/safestack.h>
#include <openssl/ssl.h>
#include <openssl/e_os2.h>
#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/evp.h>
#include <openssl/bn.h>
#include <openssl/x509v3.h>
#include <openssl/ocsp.h>
#include <openssl/srp.h>

#include "malloc_2.h"
#include "unwind_and_find_var_addrs.h"
#include "arg_struct.h"

extern int file_loaded;

int bb_BIO_free(BIO * arg_a);

int BIO_free(BIO * arg_a) 
{
    unsigned long in_lib = syscall(890);
    printf("BIO_free called %lu\n", in_lib);
    if (!in_lib)
        return bb_BIO_free(arg_a);
    else {
        int (*orig_BIO_free)(BIO *);
        orig_BIO_free = dlsym(RTLD_NEXT, "BIO_free");
        return orig_BIO_free(arg_a);
    }
}

int bb_BIO_free(BIO * arg_a) 
{
    if (!file_loaded) {
        read_in_file();
        file_loaded = 1;
    }

    int ret;

    struct lib_enter_args args = { 0 };
    struct lib_enter_args *args_addr = &args;
    populate_arg(args_addr, arg_a);
    populate_ret(args_addr, ret);

    unwind_and_find_var_addrs(args.var_li, &args.var_li_size);
    get_mallocd_vars(args.var_li, &args.var_li_size);

    struct lib_enter_args *new_args = (struct lib_enter_args *)syscall(888, args_addr);

    BIO * new_arg_a = *((BIO * *)new_args->args[0]);

    int *new_ret_ptr = (int *)new_args->ret;

    int (*orig_BIO_free)(BIO *);
    orig_BIO_free = dlsym(RTLD_NEXT, "BIO_free");
    *new_ret_ptr = (*orig_BIO_free)(new_arg_a);

    syscall(889);

    return ret;
}
