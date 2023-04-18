#define _GNU_SOURCE
#include "unwind_and_find_var_addrs.h"
#include <stdio.h>
#include <libunwind.h>
#include <ucontext.h>
#include <link.h>

#define UNW_LOCAL_ONLY

u_long load_address = 0;

static int callback(struct dl_phdr_info *info, size_t size, void *data)
{
    printf("caled\n");
    if (info->dlpi_name[0] == '\0') {
        u_long *load_address = data;
        *load_address = (u_long) info->dlpi_addr;
        return 1;
    }
    return 0;
}

void unwind_and_find_var_addrs(void) {
    //init unwind context
    unw_context_t context;
    unw_cursor_t cursor;
    unw_getcontext(&context);
    unw_init_local(&cursor, &context);

    dl_iterate_phdr(callback, &load_address);

    printf("load address is %#lx\n", load_address);

    unw_word_t ip;
    while (unw_step(&cursor) > 0) {
        unw_get_reg(&cursor, UNW_REG_IP, &ip);
        u_long rel_addr = ip - load_address;
        printf("ip was %#lx\n", rel_addr);
        // printf("ip was %#lx\n", ip);
    }

    printf("Finished walk\n");
}

void read_in_file(void) {
    FILE *f = fopen("/tmp/vprof/var_info", "r");
    char ch;
    do {
        ch = fgetc(f);
        printf("%c", ch);
 
        // Checking if character is not EOF.
        // If it is EOF stop reading.
    } while (ch != EOF);
    printf("\n");
}
