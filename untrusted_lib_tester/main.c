#include <stdio.h>
#include "lib_input.h"
#include "fake_lib.h"

float glob = 10.2;

int myfunc(double x, int *y) {
    printf("Wow! %lf, %d\n", x, *y);
    return 400;
}

int main(void)
{
    int i = 1;
    double d = 1.1;
    float sub_f = 6.0;

    struct lib_input x = {
        .sub_ptr = NULL,
        .sub = {
            .s = "test",
	        .f_sub = &sub_f,
	        .self = &x.sub,
	        .q = 420
    	},
        .f = 2.2,
        .i_ptr = &i,
        .d_ptr = &d,
        .func_ptr = myfunc
    };
    
    struct sub_input z = {
        .s = "hello!",
        .f_sub = &glob,
        .self = &z,
        .q = 69
    };

    printf("Going to run lib_func\n");

    lib_func(13, &x, 101.5, z);

    printf("We returned from main\n");

    return 0;
}
