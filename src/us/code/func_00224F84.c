#include "common.h"

/* Only what this reads: a table (its first word is the entry count) and
 * a value passed along with it. */
typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 unk14;
    /* 0x18 */ u8 pad18[0x40 - 0x18];
    /* 0x40 */ s32 *table;
} TableOwner;

extern s32 func_00201848(s32, s32, s32);

s32 func_00224F84(TableOwner *arg0, s32 arg1) {
    s32 *table;

    table = arg0->table;
    if (arg1 < *table) {
        return func_00201848((s32)table, arg0->unk14, arg1);
    }
    return 0;
}
