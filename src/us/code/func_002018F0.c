#include "common.h"
#include "structs/table.h"

/* The table's address and total size in bytes. */
void CUSet__GetSetAndSize(ArrayTable *arg0, void **arg1, s32 *arg2) {
    *arg1 = arg0;
    *arg2 = (arg0->count * arg0->size) + 8;
}
