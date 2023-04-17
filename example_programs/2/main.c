#include <stdio.h>

#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include <unistd.h>

struct hello {
    double a;
    float b;
    unsigned long c;
};

void func_4(struct hello hello);

void func_3(struct hello hello);

int func_2(int b);

void func_1(int a);

int main();

void func_4(struct hello hello)
{

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
}

void func_1(int a)
{
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
    int a = 10;
    func_1(a);
}
