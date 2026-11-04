#include "common.h"

/* Only the field read here. */
typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u32 unk10;
} CallbackArg;

extern s32 D_80130940;
extern s32 D_80130998;
extern s32 D_801309AC;

void func_00423130(s32 arg0, CallbackArg *arg1) {
    if ((D_801309AC == 0) && (D_80130998 != 0) && (arg1->unk10 < 2U)) {
        D_801309AC = 1;
        D_80130940 = 1;
    }
}
