#include "common.h"

/* A table: element count and size, then the data at +8. */
typedef struct {
    /* 0x0 */ s32 count;
    /* 0x4 */ s32 size;
} Table;

/* The table's address and total size in bytes. */
void func_002018F0(Table *arg0, void **arg1, s32 *arg2) {
    *arg1 = arg0;
    *arg2 = (arg0->count * arg0->size) + 8;
}
