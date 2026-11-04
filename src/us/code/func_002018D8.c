#include "common.h"

typedef struct {
    /* 0x00 */ s32 count;
    /* 0x04 */ s32 size;
} TableHeader;

/* Bytes taken by a table: count * element size plus the 8-byte header. */
s32 func_002018D8(TableHeader *arg0) {
    return (arg0->count * arg0->size) + 8;
}
