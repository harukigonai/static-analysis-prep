#ifndef __UNWIND_AND_FIND_VAR_ADDRS__
#define __UNWIND_AND_FIND_VAR_ADDRS__

#include "arg_struct.h"
#include <stddef.h>

void unwind_and_find_var_addrs(struct var *var_li, size_t *var_li_size);
void read_in_file(void);

#endif
