#include "common.h"

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

typedef struct EventParams {
    s32 unk0;
    s32 unk4;
} EventParams;

extern s32 D_80110048;
extern void func_004028F4(s32 arg0, s32 arg1, s32 arg2, Vec3 pos, EventParams params);
extern void func_00406C4C(s32 arg0, s32 arg1, s32 arg2, Vec3 pos, EventParams params);
extern void func_00409D4C(s32 arg0, s32 arg1, s32 arg2, Vec3 pos, EventParams params);

void func_00238DF8(s32 arg0, s32 arg1, s32 arg2, Vec3 pos, EventParams params) {
    switch (D_80110048) {
        case 0x1D4D:
            func_004028F4(arg0, arg1, arg2, pos, params);
            break;
        case 0x1DB2:
            func_00406C4C(arg0, arg1, arg2, pos, params);
            break;
        case 0x1DB1:
            func_00409D4C(arg0, arg1, arg2, pos, params);
            break;
    }
}
