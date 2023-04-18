#include "./unwind_and_find_var_addrs/unwind_and_find_var_addrs.h"

#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include <unistd.h>

struct hello {
    double a;
    float b;
    unsigned long c;
};

typedef struct hello hello2;

void func_4(struct hello hello);

void func_3(struct hello hello);

int func_2(int b);

void func_1(int a);

int main();

void *void_guy;
hello2 global_guy;
unsigned long b;

void func_4(struct hello hello)
{
    b = 3;
    unwind_and_find_var_addrs();
}

void func_3(struct hello hello)
{
    hello.a = 0.4;
    hello.b = 0.1;
    hello.c = 1234;
    func_4(hello);
}

int func_2(int b)
{
    unsigned long d = b + 14;
    struct hello hello;
    hello.a = 0.34;
    hello.b = 1313;
    hello.c = d;
    func_3(hello);

    return 0;
}

void func_1(int a)
{
    int c = 10;
    if (c <= 9) {
        return;
    }

    unsigned long long b = 1234;
    int g = 123;
    for (int b = 0; b < 94; b++) {
        b += 20;
    }
    for (int c = 0; c < 30; c++) {
        g--;
    }
    func_2(g);
    struct hello hello;
}

int main()
{
    printf("main: %#lx\n", &main);
    printf("func_1: %#lx\n", &func_1);
    printf("func_2: %#lx\n", &func_1);
    printf("func_3: %#lx\n", &func_1);
    printf("func_4: %#lx\n", &func_1);

    read_in_file();

    int a = 10;
    func_1(a);
}
