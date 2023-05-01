#define _GNU_SOURCE

#include <stdint.h>
#include "../fake_lib.h"
#include "../lib_input.h"
#include "arg_struct.h"

#include <assert.h>
#include <dlfcn.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <unistd.h>

#include "unwind_and_find_var_addrs.h"

extern void get_mallocd_vars(struct var *, size_t *);

void (*get_mallocd_vars_addr)(struct var *var_li, size_t *var_li_size) = &get_mallocd_vars;

/* Pretend this is the wrapper */
struct lib_output lib_func(int w, struct lib_input *x, double y, struct sub_input z)
{
    printf("entering lib_func\n");

    read_in_file();

    struct lib_output ret;

    struct lib_enter_args *args_addr = malloc(sizeof(struct lib_enter_args));
    populate_arg(args_addr, w);
    populate_arg(args_addr, x);
    populate_arg(args_addr, y);
    populate_arg(args_addr, z);
    populate_ret(args_addr, ret);

    printf("w: addr: %#lx. size: %d\n", &w, sizeof(w));
    printf("x: addr: %#lx. size: %d\n", &x, sizeof(x));
    printf("y: addr: %#lx. size: %d\n", &y, sizeof(y));
    printf("z: addr: %#lx. size: %d\n", &z, sizeof(z));
    printf("ret: addr: %#lx. size: %d\n", &ret, sizeof(ret));

    printf("\n---\n\n");
    printf("w: %d\n", w);
    printf("x: %#lx\n"
           " \\_ i_ptr: %#lx\n"
           " |   \\_ *i_ptr: %d\n"
           " \\_ d_ptr: %#lx\n"
           " |   \\_ *d_ptr: %lf\n"
           " \\_ f: %f\n"
           " \\_ sub\n"
           " |   \\_ s: %s\n"
           " |   \\_ f_sub: %#lx\n"
           " |   \\_ self: %#lx\n"
           " |   \\_ q: %d\n"
           " \\_ sub_ptr: %#lx\n"
           " \\_ func_ptr: %#lx\n",
           x, x->i_ptr, *(x->i_ptr), x->d_ptr, *(x->d_ptr), x->f, x->sub.s, x->sub.f_sub, x->sub.self, x->sub.q,
           x->sub_ptr, x->func_ptr);
    printf("y: %ld\n", y);
    printf("z:\n"
           " \\_ s: %s\n"
           " \\_ f_sub: %#lx\n"
           " |   \\_ *f_sub: %f\n"
           " \\_ self: %#lx\n"
           " \\_ q: %d\n", z.s, z.f_sub, *(z.f_sub), z.self, z.q);
    printf("addr of ret: %#lx\n", &ret);

    unwind_and_find_var_addrs(args_addr->var_li, &args_addr->var_li_size);
    args_addr->var_li_malloc_start_ind = args_addr->var_li_size;
    get_mallocd_vars(args_addr->var_li, &args_addr->var_li_size);

    printf("----VARIABLES----\n");
    printf("var_li_size: %ld\n", args_addr->var_li_size);
    printf("var_li_malloc_start_ind: %d\n", args_addr->var_li_malloc_start_ind);
    for (int i = 0; i < args_addr->var_li_size; i++) {
        struct var *var_addr = &args_addr->var_li[i];
        printf("<%#lx, %ld, %d>\n", var_addr->arg, var_addr->size, var_addr->untrusted);
    }
    printf("-----------------\n");

    printf("Size of lib_enter_args is %ld\n\n", sizeof(struct lib_enter_args));

    printf("Old i_addr: %#lx\n", (uint64_t)x->i_ptr);
    printf("Old d_addr: %#lx\n", (uint64_t)x->d_ptr);

    // invoke syscall 888
    struct lib_enter_args *new_args = (struct lib_enter_args *)(syscall(888, args_addr));

    printf("new_args is %#lx\n", new_args);

    new_args = (uint64_t)new_args - 4096 * 10;

    printf("updated new_args is %#lx. %ld (%#lx). %d (%#lx)\n", new_args,
        new_args->var_li_size, &new_args->var_li_size,
        new_args->var_li_malloc_start_ind, &new_args->var_li_malloc_start_ind);


    printf("-------IN_UNTRUSTED_LIB--------\n");
    printf("var_li_size: %ld\n", new_args->var_li_size);
    printf("var_li_malloc_start_ind: %d\n", new_args->var_li_malloc_start_ind);
    printf("printed var_li_data\n\n");


    int num_args = new_args->num_args;
    printf("num args is %d\n", num_args);
    assert(num_args == 4);

    int new_w = *((int *)new_args->args[0]);
    struct lib_input *new_x = *((struct lib_input **)new_args->args[1]);
    double new_y = *((double *)new_args->args[2]);
    struct sub_input new_z = *((struct sub_input *)new_args->args[3]);

    // Note that the ret val type is struct lib_output
    struct lib_output *new_ret_ptr = ((struct lib_output *)new_args->ret);

    printf("\n---\n\n");
    printf("new_w: %d\n", new_w);
    printf("new_x: %#lx\n"
           " \\_ i_ptr: %#lx\n"
           " |   \\_ *i_ptr: %d\n"
           " \\_ d_ptr: %#lx\n"
           " |   \\_ *d_ptr: %lf\n"
           " \\_ f: %f\n"
           " \\_ sub\n"
           " |   \\_ s: %s\n"
           " |   \\_ f_sub: %#lx\n"
           " |   \\_ self: %#lx\n"
           " |   \\_ q: %d\n"
           " \\_ sub_ptr: %#lx\n"
           " \\_ func_ptr: %#lx\n",
           new_x, new_x->i_ptr, *(new_x->i_ptr), new_x->d_ptr, *(new_x->d_ptr), new_x->f, new_x->sub.s,
           new_x->sub.f_sub, new_x->sub.self, new_x->sub.q, new_x->sub_ptr, new_x->func_ptr);
    printf("new_y: %ld\n", new_y);
    printf("new_z:\n"
           " \\_ s: %s\n"
           " \\_ f_sub: %#lx\n"
           " |   \\_ *f_sub: %f\n"
           " \\_ self: %#lx\n"
           " \\_ q: %d\n", new_z.s, new_z.f_sub, *(new_z.f_sub), new_z.self, new_z.q);
    printf("new addr of ret: %#lx\n", new_ret_ptr);

    struct lib_output (*orig_lib_func)(int w, struct lib_input *x, double y, struct sub_input z);
    orig_lib_func = dlsym(RTLD_NEXT, "lib_func");
    *new_ret_ptr = (*orig_lib_func)(new_w, new_x, new_y, new_z);

    printf("actual_lib_func is updating some of the stuff...\n\n");

    printf("\n---\n\n");
    printf("new_w: %d\n", new_w);
    printf("new_x: %#lx\n"
           " \\_ i_ptr: %#lx\n"
           " |   \\_ *i_ptr: %d\n"
           " \\_ d_ptr: %#lx\n"
           " |   \\_ *d_ptr: %lf\n"
           " \\_ f: %f\n"
           " \\_ sub\n"
           " |   \\_ s: %s\n"
           " |   \\_ f_sub: %#lx\n"
           " |   \\_ self: %#lx\n"
           " |   \\_ q: %d\n"
           " \\_ sub_ptr: %#lx\n"
           " \\_ func_ptr: %#lx\n",
           new_x, new_x->i_ptr, *(new_x->i_ptr), new_x->d_ptr, *(new_x->d_ptr), new_x->f, new_x->sub.s,
           new_x->sub.f_sub, new_x->sub.self, new_x->sub.q, new_x->sub_ptr, new_x->func_ptr);
    printf("new_y: %ld\n", new_y);
    printf("new_z:\n"
           " \\_ s: %s\n"
           " \\_ f_sub: %#lx\n"
           " |   \\_ *f_sub: %f\n"
           " \\_ self: %#lx\n"
           " \\_ q: %d\n",
           new_z.s, new_z.f_sub, *(new_z.f_sub), new_z.self, new_z.q);
    printf("new_ret: %#lx\n"
           " \\_ i: %d\n"
           " \\_ i_ptr: %#lx\n"
           " |   \\_ *i_ptr: %d\n"
           " \\_ d_ptr: %#lx\n"
           " |   \\_ *d_ptr: %lf\n"
           " \\_ s_ptr: %s\n"
           " \\_ s: %s\n",
           new_ret_ptr, new_ret_ptr->i, new_ret_ptr->i_ptr, *new_ret_ptr->i_ptr, new_ret_ptr->d_ptr,
           *new_ret_ptr->d_ptr, new_ret_ptr->s_ptr, new_ret_ptr->s);

    new_args->var_li_size = new_args->var_li_malloc_start_ind;
    printf("var_li_size: %ld\n", new_args->var_li_size);
    get_mallocd_vars(new_args->var_li, &new_args->var_li_size);
    printf("var_li_size: %ld\n\n", new_args->var_li_size);

    printf("----VARIABLES----\n");
    printf("var_li_size: %ld\n", new_args->var_li_size);
    printf("var_li_malloc_start_ind: %d\n", new_args->var_li_malloc_start_ind);
    for (int i = 0; i < new_args->var_li_size; i++) {
        struct var *var_addr = &new_args->var_li[i];
        printf("<%#lx, %ld, %d>\n", var_addr->arg, var_addr->size, var_addr->untrusted);
    }
    printf("-----------------\n");

    printf("test\n");

    // Copies contents of new_args->ret into args_addr.ret
    syscall(889);

    printf("We exited 889. ret.i is %d\n\n", ret.i);

    printf("\n---\n\n");
    printf("w: %d\n", w);
    printf("x: %#lx\n"
           " \\_ i_ptr: %#lx\n"
           " |   \\_ *i_ptr: %d\n"
           " \\_ d_ptr: %#lx\n"
           " |   \\_ *d_ptr: %lf\n"
           " \\_ f: %f\n"
           " \\_ sub\n"
           " |   \\_ s: %s\n"
           " |   \\_ f_sub: %#lx\n"
           " |   \\_ self: %#lx\n"
           " |   \\_ q: %d\n"
           " \\_ sub_ptr: %#lx\n"
           " \\_ func_ptr: %#lx\n",
           x, x->i_ptr, *(x->i_ptr), x->d_ptr, *(x->d_ptr), x->f, x->sub.s, x->sub.f_sub, x->sub.self, x->sub.q,
           x->sub_ptr, x->func_ptr);
    printf("y: %ld\n", y);
    printf("z:\n"
           " \\_ s: %s\n"
           " \\_ f_sub: %#lx\n"
           " |   \\_ *f_sub: %f\n"
           " \\_ self: %#lx\n"
           " \\_ q: %d\n", z.s, z.f_sub, *(z.f_sub), z.self, z.q);
    printf("ret: %#lx\n"
           " \\_ i: %d\n"
           " \\_ i_ptr: %#lx\n"
           " |   \\_ *i_ptr: %d\n"
           " \\_ d_ptr: %#lx\n"
           " |   \\_ *d_ptr: %lf\n"
           " \\_ s_ptr: %s\n"
           " \\_ s: %s\n",
           &ret, ret.i, ret.i_ptr, *ret.i_ptr, ret.d_ptr, *ret.d_ptr, ret.s_ptr, ret.s);

    return ret;
}
