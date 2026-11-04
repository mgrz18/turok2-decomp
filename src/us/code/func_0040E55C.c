#include "common.h"

typedef struct {
    /* 0x0 */ f32 x;
    /* 0x4 */ f32 y;
    /* 0x8 */ f32 z;
} Vec3f;

/* Only what this reads and writes. */
typedef struct {
    /* 0x000 */ u8 pad0[0x4F8];
    /* 0x4F8 */ Vec3f *target;
    /* 0x4FC */ u8 pad4FC[4];
    /* 0x500 */ s32 targetY;
    /* 0x504 */ u8 pad504[4];
    /* 0x508 */ s32 targetZ;
} Tracker;

s32 func_00225C84(s32);
extern u8 D_800F7078[];

void func_0040E55C(Tracker *arg0) {
    Vec3f *temp_v0;

    temp_v0 = arg0->target;
    arg0->targetY = (s32) temp_v0->y;
    arg0->targetZ = (s32) temp_v0->z;
    func_00225C84((s32) D_800F7078);
}
