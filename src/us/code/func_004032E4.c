#include "common.h"

void func_00246108(s32 a0, s32 a1, void *out, s32 a3, s32 a4);
void func_00245BAC(s32 a0, s32 a1, f32 a2, s32 a3);

void func_004032E4(s32 a0, s32 a1) {
    u8 buf[0x88];

    func_00246108(a0, a1, buf, 1, 1);
    func_00245BAC(a0, a1, 0.0174532961f, *(s32 *)(buf + 0x4C));
}
