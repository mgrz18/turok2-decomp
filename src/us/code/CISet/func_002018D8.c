#include "common.h"
#include "structs/table.h"
#include "protos/iset.h"

/* Bytes taken by a table: count * element size plus the 8-byte header. */
s32 CUSet__GetTotalSize(ArrayTable *arg0) {
    return (arg0->count * arg0->size) + 8;
}
