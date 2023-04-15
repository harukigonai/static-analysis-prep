#include <stdio.h>

#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include <unistd.h>
#include <backtrace.h>

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
#define BT_BUF_SIZE 1024

struct backtrace_state *state;

static void error_callback (void *data, const char *message, int error_number) {
  if (error_number == -1) {
    fprintf(stderr, "If you want backtraces, you have to compile with -g\n\n");
    _Exit(1);
  } else {
    fprintf(stderr, "Backtrace error %d: %s\n", error_number, message);
  };
};

static int full_callback (void *data, uintptr_t pc, const char *pathname, int line_number, const char *function) {
  static int unknown_count = 0;
  if (pathname != NULL || function != NULL || line_number != 0) {
    if (unknown_count) {
      fprintf(stderr, "    ...\n");
      unknown_count = 0;
    };
    const char *filename = rindex(pathname, '/');
    if (filename) filename++; else filename = pathname;
    printf("%s\n", function);
    fprintf(stderr, "  %s:%d -- %s. %#lx\n", filename, line_number, function, pc - (unsigned long)&func_4);
  } else {
    unknown_count++;
  };
  return 0;
};

void func_4(struct hello hello)
{
    printf("func_4: %#lx\n", &func_4);
    printf("func_3: %#lx\n", &func_3);
    printf("func_2: %#lx\n", &func_2);
    printf("func_1: %#lx\n", &func_1);
    printf("main: %#lx\n", &main);

    state = backtrace_create_state(NULL, 0, error_callback, NULL);
    backtrace_full(state, 0, full_callback, error_callback, NULL);
    // printf("\n");
    // backtrace_print(state, 0, stderr);
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
