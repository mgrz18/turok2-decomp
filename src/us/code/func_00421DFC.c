#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 *tmpl;
} MenuItem;

extern s32 D_800B490C;
extern s32 D_80130990;

s32 func_00421DFC(MenuItem *arg0) {
    if (D_80130990 != 0) {
        arg0->tmpl = &D_800B490C;
    }
    return 0;
}
