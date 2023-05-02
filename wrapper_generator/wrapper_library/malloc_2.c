#define _GNU_SOURCE

#include <stdio.h>
#include <dlfcn.h>
#include <pthread.h>
#include "malloc_2.h"
#include "arg_struct.h"

static struct var mallocd_vars[4096];
static size_t mallocd_vars_size;

static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static int setting_up_untrusted_malloc = 0;

static void (*trusted_free_ptr)(void *) = NULL;
static void (*untrusted_free_ptr)(void *) = NULL;
static void *(*trusted_malloc_ptr)(size_t) = NULL;
static void *(*untrusted_malloc_ptr)(size_t) = NULL;

static void trusted_malloc_and_free_init()
{
    trusted_malloc_ptr = dlsym(RTLD_NEXT, "malloc");
    trusted_free_ptr = dlsym(RTLD_NEXT, "free");
}

static void untrusted_malloc_and_free_init()
{
    setting_up_untrusted_malloc = 1;
    void *new_libc = dlmopen(LM_ID_NEWLM, "/lib/aarch64-linux-gnu/libc.so.6", RTLD_NOW);
    untrusted_malloc_ptr = dlsym(new_libc, "malloc");
    untrusted_free_ptr = dlsym(new_libc, "free");
    setting_up_untrusted_malloc = 0;
}

void get_mallocd_vars(struct var *var_li, size_t *var_li_size)
{
    printf("get_mallocd_vars called\n");
    pthread_mutex_lock(&lock);
    for (size_t i = 0; i < mallocd_vars_size; i++) {
        var_li[*var_li_size + i] = mallocd_vars[i];
    }
    *var_li_size += mallocd_vars_size;
    pthread_mutex_unlock(&lock);
}

void *malloc(size_t size)
{
    if (!trusted_malloc_ptr) {
        trusted_malloc_and_free_init();
    }
    if (!untrusted_malloc_ptr && !setting_up_untrusted_malloc) {
        untrusted_malloc_and_free_init();
    }

    int in_untrusted_lib = syscall(890);
    void *res;
    if (!in_untrusted_lib || setting_up_untrusted_malloc) {
        res = trusted_malloc_ptr(size);
    } else {
        res = untrusted_malloc_ptr(size);
    }

    if (res) {
        int found_empty = 0;
        pthread_mutex_lock(&lock);
        for (int i = 0; i < mallocd_vars_size; i++) {
            if (mallocd_vars[i].arg == 0) {
                mallocd_vars[i].arg = res;
                mallocd_vars[i].size = size;
                mallocd_vars[i].untrusted = in_untrusted_lib;
                found_empty = 1;
            }
        }

        if (!found_empty) {
            mallocd_vars[mallocd_vars_size].arg = res;
            mallocd_vars[mallocd_vars_size].size = size;
            mallocd_vars[mallocd_vars_size].untrusted = in_untrusted_lib;
            mallocd_vars_size++;
        }
        pthread_mutex_unlock(&lock);
    }

    return res;
}

void free(void *ptr)
{
    int is_mallocd_var = 0;
    pthread_mutex_lock(&lock);
    for (int i = 0; i < mallocd_vars_size; i++) {
        if (mallocd_vars[i].arg == ptr) {
            is_mallocd_var = 1;
            memset(&mallocd_vars[i], sizeof(struct var), 0);
        }
    }
    pthread_mutex_unlock(&lock);

    if (!is_mallocd_var) {
        return;
    }

    int in_untrusted_lib = syscall(890);
    if (!in_untrusted_lib || setting_up_untrusted_malloc) {
        trusted_free_ptr(ptr);
    } else {
        untrusted_free_ptr(ptr);
    }
}

