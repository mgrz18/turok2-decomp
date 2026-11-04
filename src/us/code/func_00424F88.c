#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 *tmpl;
} MenuItem;

extern s32 D_800B436C;
extern s32 D_800B437C;
extern s32 D_80130A34;

s32 func_00424F88(MenuItem *arg0) {
    if (D_80130A34 != 0) {
        arg0->tmpl = &D_800B437C;
    } else {
        arg0->tmpl = &D_800B436C;
    }
    return 1;
}
