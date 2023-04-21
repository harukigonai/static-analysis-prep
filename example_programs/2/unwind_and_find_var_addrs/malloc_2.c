#define _GNU_SOURCE

#include <stdio.h>
#include <dlfcn.h>
#include "malloc_2.h"
#include "arg_struct.h"

static struct var mallocd_vars[4096];
static size_t mallocd_vars_size;

static int setting_up_untrusted_malloc = 0;

static void *(*trusted_malloc_ptr)(size_t) = NULL;
static void *(*untrusted_malloc_ptr)(size_t) = NULL;

static void trusted_malloc_init()
{
    trusted_malloc_ptr = dlsym(RTLD_NEXT, "malloc");
}

static void untrusted_malloc_init()
{
    setting_up_untrusted_malloc = 1;
    void *new_libc = dlmopen(LM_ID_NEWLM, "/lib/aarch64-linux-gnu/libc.so.6", RTLD_NOW);
    untrusted_malloc_ptr = dlsym(new_libc, "malloc");
    setting_up_untrusted_malloc = 0;
}

void get_mallocd_vars(struct var *var_li, size_t *var_li_size)
{
    for (size_t i = 0; i < mallocd_vars_size; i++) {
        var_li[*var_li_size + i] = mallocd_vars[i];
        (*var_li_size)++;
    }
}

void *malloc(size_t size)
{
    if (!trusted_malloc_ptr) {
        trusted_malloc_init();
    }
    if (!untrusted_malloc_ptr && !setting_up_untrusted_malloc) {
        untrusted_malloc_init();
    }

    int in_untrusted_lib = syscall(890);
    void *res;
    if (!in_untrusted_lib || setting_up_untrusted_malloc) {
        res = trusted_malloc_ptr(size);
    } else {
        res = untrusted_malloc_ptr(size);
    }

    if (res) {
        // FIX ME: not atomic
        mallocd_vars[mallocd_vars_size].arg = res;
        mallocd_vars[mallocd_vars_size].size = size;
        mallocd_vars_size++;
    }

    return res;
}
