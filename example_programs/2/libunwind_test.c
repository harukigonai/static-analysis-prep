

#include "./unwind_and_find_var_addrs/unwind_and_find_var_addrs.h"
#include "./unwind_and_find_var_addrs/arg_struct.h"
#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include <unistd.h>
#include "libunwind_test.h"


void func_4(struct hello3 hello)
{
    printf("func_4: hello: %#lx\n", &hello);

    int b = 3;

    struct var var_li[4096];
    size_t var_li_size;

    unwind_and_find_var_addrs(var_li, &var_li_size);
}

void func_3(int b)
{
    printf("In shared lib\n");
    struct hello3 hello;

    printf("func_3: b: %#lx\n", &b);
    printf("func_3: hello: %#lx\n", &hello);
    func_4(hello);
}
