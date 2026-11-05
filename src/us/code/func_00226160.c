#include "common.h"
#include "structs/world.h"

extern s32 func_002017D4(s32, s32);

/* Entry arg2 of table arg1: a word giving the entry size, then the entries
 * from +8. */
void *func_00226160(CWorld *arg0, s32 arg1, s32 arg2) {
    s32 *t = (s32 *) func_002017D4(func_002017D4((s32)arg0->unk_68, arg1), 1);
    return (s8 *)t + (arg2 * *t + 8);
}
