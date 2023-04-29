#ifndef __ARG_STRUCT_H
#define __ARG_STRUCT_H

// TODO: make this an actual kernel uapi header

#include <stdint.h>
#include <sys/types.h>

#define populate_arg(args_addr, arg) {\
        args_addr->args[args_addr->num_args] = &arg;\
        args_addr->args_size[args_addr->num_args] = sizeof(arg);\
        args_addr->num_args++;\
    }

#define populate_ret(args_addr, arg) {\
        args_addr->ret = &arg;\
        args_addr->ret_size = sizeof(arg);\
    }

#define MAX_ARGS 100

struct var {
    void *arg;
    size_t size;
};

struct lib_enter_args {
    int num_args;
    void *args[MAX_ARGS];
    size_t args_size[MAX_ARGS];

    void *ret;
    size_t ret_size;

    struct var var_li[4 * 4096];
    size_t var_li_size;
};

// #define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#endif