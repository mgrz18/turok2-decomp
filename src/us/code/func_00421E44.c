#include "common.h"

/* Something whose table pointer at 0x14 this picks (D_800B49C4 or D_800B49D0). */
typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 *table;
} TableHolder;

extern s32 D_800B49C4;
extern s32 D_800B49D0;
extern s32 D_801309B8;

s32 func_00421E44(TableHolder *arg0) {
    if (D_801309B8 != 0) {
        arg0->table = &D_800B49C4;
    } else {
        arg0->table = &D_800B49D0;
    }
    return 0;
}
