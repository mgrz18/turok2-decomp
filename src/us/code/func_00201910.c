#include "common.h"
#include "structs/table.h"

/* The table data and its size in bytes. */
void *func_00201910(ArrayTable *arg0, s32 *arg1) {
    *arg1 = arg0->count * arg0->size;
    return arg0->data;
}
