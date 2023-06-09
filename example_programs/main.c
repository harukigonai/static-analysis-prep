#include "./unwind_and_find_var_addrs/unwind_and_find_var_addrs.h"

#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include <unistd.h>

struct hello {
    double a;
    float b;
    unsigned long c;
    int arr[4096];
};

typedef struct hello hello2;

hello2 hi;

struct hello hello_arr[14];

hello2 hello2_arr_arr[4][3];

int func_2(int b);

void func_1(int a);

int main();

void *void_guy;
hello2 global_guy;
unsigned long b;

int func_2(int b)
{
    unsigned long d = b + 14;
    struct hello hello;
    hello.a = 0.34;
    hello.b = 1313;
    hello.c = d;

    printf("func_2: b: %#lx\n", &b);
    printf("func_2: d: %#lx\n", &d);
    printf("func_2: hello: %#lx\n", &hello);

    func_3(b);

    return 0;
}

void func_1(int a)
{
    printf("func_1: a: %#lx\n", &a);

    int c = 10;
    if (c <= 9) {
        return;
    }

    printf("func_1: c: %#lx\n", &c);

    unsigned long long b = 1234;
    int g = 123;

    printf("func_1: b: %#lx\n", &b);
    printf("func_1: g: %#lx\n", &g);

    for (int b = 0; b < 94; b++) {
        printf("func_1: b: %#lx\n", &b);
        b += 20;
    }
    for (int c = 0; c < 30; c++) {
        printf("func_1: c: %#lx\n", &c);
        g--;
    }
    struct hello hello;
    printf("func_1: hello: %#lx\n", &hello);

    func_2(g);
}

int main()
{
    printf("main: %#lx\n", &main);
    printf("func_1: %#lx\n", &func_1);
    printf("func_2: %#lx\n", &func_2);

    read_in_file();

    printf("main: void_guy: %#lx\n", &void_guy);
    printf("main: global_guy: %#lx\n", &global_guy);
    printf("main: b: %#lx\n", &b);

    int a = 10;

    printf("main: a: %#lx\n", &a);

    func_1(a);
}
