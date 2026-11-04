#include "common.h"

extern void *func_002972E0(void *, s16 *, s32, s32, void *); /* alSavePull */
extern s32 func_002973E0(s32, s32, s32);
extern void func_00296340(void *, void *, void *, s32);

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
} SaveFilter;

void func_002954F8(SaveFilter *arg0) {
    func_00296340(arg0, func_002972E0, func_002973E0, 3);
    arg0->unk14 = 0;
    arg0->unk18 = 1;
}
