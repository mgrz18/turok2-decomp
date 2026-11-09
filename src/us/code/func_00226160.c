#include "common.h"
#include "structs/world.h"
#include "protos/iset.h"


/* Entry arg2 of table arg1: a word giving the entry size, then the entries
 * from +8. */
void *func_00226160(CScene *arg0, s32 arg1, s32 arg2) {
    s32 *t = (s32 *) CISet__GetBlock(CISet__GetBlock(arg0->unk_68, arg1), 1);
    return (s8 *)t + (arg2 * *t + 8);
}
