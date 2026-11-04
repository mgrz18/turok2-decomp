#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ u16 unk4;
    /* 0x06 */ u8 pad6[4];
    /* 0x0A */ u16 unkA;
} TriggerData;

s32 func_002245BC(s32 *, u16, u16, s32); /* extern */
extern s32 D_800F7078;

void func_00266A00(TriggerData *arg0) {
    func_002245BC(&D_800F7078, arg0->unkA, arg0->unk4, 0);
}
