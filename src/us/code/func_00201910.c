#include "common.h"

/* A table: element count and size, then the data at +8. */
typedef struct {
    /* 0x0 */ s32 count;
    /* 0x4 */ s32 size;
    /* 0x8 */ u8 data[1];
} Table;

/* The table data and its size in bytes. */
void *func_00201910(Table *arg0, s32 *arg1) {
    *arg1 = arg0->count * arg0->size;
    return arg0->data;
}
