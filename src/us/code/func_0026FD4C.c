#include "common.h"

extern s32 D_800F22C8;
typedef struct {
    /* 0x0 */ u8 initialized;
    /* 0x1 */ u8 pad1[3];
    /* 0x4 */ s32 unk4;
    /* 0x8 */ s32 *data;
} Unk800F2790;
extern Unk800F2790 D_800F2790;
extern s32 func_002700D0;

s32 *func_0026FD4C(Unk800F2790 **arg0) {
    if (D_800F2790.initialized == 0) {
        D_800F2790.data = &D_800F22C8;
        D_800F2790.unk4 = 0;
        D_800F2790.initialized = 1U;
    }
    *arg0 = &D_800F2790;
    return &func_002700D0;
}
