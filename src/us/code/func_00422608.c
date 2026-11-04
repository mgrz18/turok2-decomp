#include "common.h"

/* Something whose table pointer at 0x14 this picks (D_800B49C4 or D_800B49D0). */
typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 *table;
} TableHolder;

extern s32 D_800B4B84;
extern s32 D_800B4B8C;
extern s32 D_800B4B94;
extern s32 D_80130960;
extern s32 D_801309A8;

s32 func_00422608(TableHolder *arg0) {
    if (D_80130960 != 0) {
        arg0->table = &D_800B4B84;
    } else if (D_801309A8 != 0) {
        arg0->table = &D_800B4B94;
    } else {
        arg0->table = &D_800B4B8C;
    }
    return 0;
}
