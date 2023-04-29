#include <stdio.h>
#include <stdlib.h>
#include "fake_lib.h"
#include "lib_input.h"

const char *some_string = "hope this works";

struct lib_output lib_func(int w, struct lib_input *x, double y, struct sub_input z)
{
    printf("In actual lib_func\n");

    *x->i_ptr = 1000;
    *x->d_ptr = 1100.0;
    x->f = 2200.0;
    strcpy(x->sub.s, "test 2");

    struct lib_output out = {0};
    out.i = 100;
    out.i_ptr = malloc(100);
    *out.i_ptr = 1234;
    out.d_ptr = malloc(300);
    *out.d_ptr = 1234.5;
    out.s_ptr = some_string;
    strcpy(out.s, "yeah");

    printf("we got return from func_ptr: %d\n", x->func_ptr(y, x->i_ptr));

    return out;
}


