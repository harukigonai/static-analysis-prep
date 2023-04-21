#define _GNU_SOURCE
#include "unwind_and_find_var_addrs.h"
#include <stdio.h>
#include <libunwind.h>
#include <ucontext.h>
#include <link.h>
#include "arg_struct.h"

#define UNW_LOCAL_ONLY

int file_loaded = 0;

struct node {
    struct node *prev, *next;
    unsigned long long from, to;
    unsigned short loc_atom;
    long var_addr, size;
};

struct node head;

u_long load_addresses[10] = {0};
size_t num_load_addresses;

int layer = 0;

static int reg_valid[31] = {1};

static int callback(struct dl_phdr_info *info, size_t size, void *data)
{
    printf("callback called\n");
    u_long load_address = (u_long)info->dlpi_addr;
    int load_address_in = 0;
    for (int i = 0; i < num_load_addresses; i++) {
        if (load_addresses[i] == load_address) {
            load_address_in = 1;
            break;
        }
    }
    if (!load_address_in) {
        printf("name: [%s]\n", info->dlpi_name);
        load_addresses[num_load_addresses] = load_address;
        num_load_addresses++;
    }
    return 0;
}

long long find_true_addr(const void* bctx, unsigned short loc_atom, long addr, u_long load_address) {
    const ucontext_t *ctx = (const ucontext_t *)(bctx);
    long long true_addr;
    switch(loc_atom) {
        case 145: {
            true_addr = ctx->uc_mcontext.regs[29] + addr;
            break;
        }
        case 3: {
            true_addr = addr + load_address;
            break;
        }
        default:
            true_addr = 0xdeadbeef;
    }
    return true_addr;
}

static int store_context(ucontext_t *dest, unw_cursor_t *cursor)
{
    //save register values from cursor
    int ret = 0;
    unw_word_t reg;
    for (int i = 0; i < 31; ++i) {
        ret = unw_get_reg(cursor, i, &reg);
        if (ret == UNW_ESUCCESS) {
            dest->uc_mcontext.regs[i] = (long long int)reg;
            reg_valid[i] = 1;
        } else {
            reg_valid[i] = 0;
        }
    }
    return ret;
}

static void
sample_variables(const ucontext_t *ctx, struct var *var_addr_li,
                 size_t *var_addr_li_size, unw_word_t ip, u_long load_address) {
    u_long pc = ip - load_address;
    printf("pc is %#lx\n", pc);

    for (struct node *node = head.next; node != &head; node = node->next) {
        if (!(node->from <= pc && pc <= node->to)) {
            continue;
        }

        int size = node->size;
        long long true_addr = find_true_addr(ctx, node->loc_atom, node->var_addr, load_address);

        int in_var_addr_li = 0;
        for (int i = 0; i < *var_addr_li_size; i++) {
            if (var_addr_li[i].size == size &&
                var_addr_li[i].arg == (void *)true_addr) {
                in_var_addr_li = 1;
            }
        }

        if (in_var_addr_li) {
            continue;
        }

        var_addr_li[*var_addr_li_size].size = size;
        var_addr_li[*var_addr_li_size].arg = (void *)true_addr;
        (*var_addr_li_size)++;

        printf("var: var_addr %#lx. size %lu. loc_atom %lu. %d\n", true_addr, size, node->loc_atom, layer);
    }
}

void unwind_and_find_var_addrs(struct var *var_li, size_t *var_li_size) {

    dl_iterate_phdr(callback, NULL);

    //init unwind context
    unw_context_t context;
    unw_cursor_t cursor;
    unw_getcontext(&context);
    unw_init_local(&cursor, &context);

    ucontext_t prev_ctx;
    unw_word_t ip;
    while (unw_step(&cursor) > 0) {
        unw_get_reg(&cursor, UNW_REG_IP, &ip);

        store_context(&prev_ctx, &cursor);

        for (int i = 0; i < num_load_addresses; i++) {
            u_long rel_addr = ip - load_addresses[i];
            sample_variables(&prev_ctx, var_li, var_li_size, ip, load_addresses[i]);
        }

        layer++;
    }

    unw_get_reg(&cursor, UNW_REG_IP, &ip);
    printf("ip was %#lx\n", ip);

    printf("var_addr_li_size is %lu\n", *var_li_size);
    for (int i = 0; i < *var_li_size; i++) {
        printf("%#lx -> %#lx (%lu)\n", var_li[i].arg, var_li[i].arg + var_li[i].size,
               var_li[i].size);
    }

    printf("Finished walk\n");
}

void read_in_file(void) {
    // FIX ME: file_loaded not atomic
    head.next = &head;
    head.prev = &head;

    FILE *fp = fopen("/tmp/vprof/var_info", "r");
    if (!fp) {
        exit(1);
    }

    unsigned long long from, to;
    unsigned short loc_atom;
    long var_addr, size;
    char line[4096];
    size_t n = 0;
    int callsite_index, prev_hash = 0;

    int i = 0;
    while (fgets(line, 4096, fp) != NULL) {
        if (line && line[0] == '#') {
            continue;
        }
        if (sscanf(line, "%llx:%llx:%hu:%ld:%ld\n", &from, &to, &loc_atom, &var_addr, &size) != 5 &&
            sscanf(line, "%llx:%llx:%hu:%lx:%ld\n", &from, &to, &loc_atom, &var_addr, &size) != 5) {
            printf("b\n");
            continue;
        }
        if (size != 0) {
            struct node *node = malloc(sizeof(struct node));
            struct node *prev = head.prev;
            prev->next = node;
            head.prev = node;
            node->prev = prev;
            node->next = &head;

            node->from = from;
            node->to = to;
            node->loc_atom = loc_atom;
            node->var_addr = var_addr;
            node->size = size;
            printf("Node %d: %llx, %llx, %hu, %ld, %ld\n", i, from, to, loc_atom, var_addr, size);
        } else {
            printf("uh oh\n");
        }

        i++;
    }
}
