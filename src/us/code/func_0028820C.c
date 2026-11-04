#include "common.h"

s32 func_0029B060(s32, s32, s32, s32, s32, s32);
s32 func_0029BB10(s32);

s32 func_00266C5C(s32 *, s32, s32); /* extern */
extern s32 D_800B6E74;
extern s32 D_800C7C00;
extern s32 D_800F6840;
extern s32 func_00288838;

void CEngineApp__Boot(void) {
    func_00266C5C(&D_800C7C00, 0, 0x80);
    func_0029B060((s32) &D_800F6840, 0, (s32) &func_00288838, 0, (s32) ((void *)((s8 *)&D_800C7C00 + 0x80)), D_800B6E74);
    func_0029BB10((s32) &D_800F6840);
}
