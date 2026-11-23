#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
} CSunFrameData;

void CSunFrameData__Construct(CSunFrameData *arg0) {
    arg0->unk14 = 0;
    arg0->unk18 = 0;
}

#include "m2c_macros.h"

void Scheduler_RDPDoneHook(void) {

}

typedef struct { s32 x, y, z; } Vec3i;
extern f32 CVector3__Normalize(void *);

void CSun__SetSunDir(void *arg0, Vec3i *arg1) {
    *(Vec3i *)((s8 *)arg0 + 0x14) = *arg1;
    CVector3__Normalize((s8 *)arg0 + 0x14);
}

/* The sun: only what CSun__GetSunData hands out. */
typedef struct {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ u8 pad14[0x20 - 0x14];
    /* 0x20 */ s32 unk20;
} CSun;

void CSun__GetSunData(CSun *arg0, s32 *arg1, f32 *arg2, f32 *arg3, f32 *arg4) {
    *arg1 = arg0->unk20;
    *arg4 = arg0->unk10;
    *arg2 = arg0->unk8;
    *arg3 = arg0->unkC;
}

void func_00228C20(void) {

}

void func_00228C28(void) {

}

void func_00228C30(void) {

}
