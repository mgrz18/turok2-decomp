#include "common.h"

typedef struct {
    u8 data[0x64];
} Light004037B0;

typedef struct {
    /* 0x000 */ s32 mode;
    /* 0x004 */ u8 pad004[0x10 - 0x4];
    /* 0x010 */ f32 unk010;
    /* 0x014 */ f32 unk014;
    /* 0x018 */ u8 pad018[0x24 - 0x18];
    /* 0x024 */ Light004037B0 lights[8];
    /* 0x344 */ u8 pad344[0x380 - 0x344];
    /* 0x380 */ s32 values[2][8];
    /* 0x3C0 */ s32 phase;
    /* 0x3C4 */ s32 mask;
    /* 0x3C8 */ u8 unk3C8;
    /* 0x3C9 */ u8 done;
    /* 0x3CA */ u8 unk3CA;
    /* 0x3CB */ u8 pad3CB[0x3D4 - 0x3CB];
    /* 0x3D4 */ s32 unk3D4;
    /* 0x3D8 */ u8 armed;
    /* 0x3D9 */ u8 pad3D9[0x3E4 - 0x3D9];
    /* 0x3E4 */ s32 unk3E4;
    /* 0x3E8 */ f32 fade;
} ModeState004037B0;

typedef struct {
    /* 0x00 */ u8 pad0[0x6C];
    /* 0x6C */ s32 mask;
} Source004037B0;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ s32 damage;
    /* 0x08 */ u8 pad1[0xC - 0x8];
    /* 0x0C */ s32 flags;
    /* 0x10 */ Source004037B0 *source;
} Hit004037B0;

extern ModeState004037B0 D_8012F608;
extern u8 D_00431A7C[];
extern u8 D_00431B0C[];
void func_00236314(void *light, void *arg);
void func_004070B4(s32 a0);
f32 func_002119FC(f32 lo, f32 hi);
void func_00243414(void *a0, void *a1, s32 event);

void func_004037B0(void *a0, void *a1, Hit004037B0 *hit) {
    s32 destroyed = 0;
    s32 bonus;
    s32 bit;
    s32 i;

    if (hit->source == NULL) {
        return;
    }
    if (D_8012F608.armed == 0) {
        return;
    }
    for (i = 0; i < 8; i++) {
        bit = 1 << i;
        if (!(hit->source->mask & bit)) {
            continue;
        }
        if (!(D_8012F608.mask & bit) && !D_8012F608.done) {
            continue;
        }
        if (!D_8012F608.done) {
            D_8012F608.values[0][i] -= hit->damage;
        }
        if (D_8012F608.done) {
            if (i != 1) {
                continue;
            }
            D_8012F608.unk3E4 += hit->damage;
        }
        if (D_8012F608.values[0][i] <= 0 && D_8012F608.mode != 3 && !D_8012F608.done) {
            func_00236314(&D_8012F608.lights[i], D_00431B0C);
            D_8012F608.phase = i;
            destroyed = 1;
            break;
        }
        func_00236314(&D_8012F608.lights[i], D_00431A7C);
    }
    bonus = 0;
    if ((hit->flags & 0x400000) && D_8012F608.mode == 1 && (hit->source->mask & 2)) {
        bonus = hit->damage / 2;
    }
    if (bonus != 0 && D_8012F608.mode == 1 && !D_8012F608.done) {
        for (i = 3; i < 7; i++) {
            if (D_8012F608.values[0][i] > 0) {
                D_8012F608.values[0][i] -= bonus;
                if (D_8012F608.values[0][i] <= 0) {
                    func_00236314(&D_8012F608.lights[i], D_00431B0C);
                    D_8012F608.phase = i;
                    destroyed = 1;
                    break;
                }
                func_00236314(&D_8012F608.lights[i], D_00431A7C);
            }
        }
    }
    if (!destroyed) {
        return;
    }
    if (D_8012F608.done) {
        return;
    }
    if (D_8012F608.mode == 2) {
        func_004070B4(4);
    }
    if (!D_8012F608.unk3CA) {
        D_8012F608.done = 1;
        D_8012F608.fade = 90.0f;
        D_8012F608.unk010 = func_002119FC(0.0f, 2.0f);
        D_8012F608.unk014 = func_002119FC(2.5f, 4.0f);
        D_8012F608.unk3D4 = 9;
        func_00243414(a0, a1, 0x12);
    } else {
        D_8012F608.done = 1;
        func_00243414(a0, a1, 0x18);
    }
}
