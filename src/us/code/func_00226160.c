#include "common.h"

/* Only the field read here: a resource walked with func_002017D4. */
typedef struct {
    /* 0x00 */ u8 pad0[0x68];
    /* 0x68 */ s32 unk68;
} TableOwner;

extern s32 func_002017D4(s32, s32);

/* Entry arg2 of table arg1: a word giving the entry size, then the entries
 * from +8. */
void *func_00226160(TableOwner *arg0, s32 arg1, s32 arg2) {
    s32 *t = (s32 *) func_002017D4(func_002017D4(arg0->unk68, arg1), 1);
    return (s8 *)t + (arg2 * *t + 8);
}
