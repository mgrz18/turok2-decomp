/* Second-module copy of func_0041760C (build/manual/W/func_0041760C.c), with its first-module addresses 0xE0 lower. */
#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x28];
    /* 0x28 */ s8 active;
    /* 0x29 */ u8 pad29[6];
    /* 0x2F */ s8 index;
    /* 0x30 */ s8 unk30;
    /* 0x31 */ u8 pad31[0xF];
} SetupSlot; /* 0x40 bytes */

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ f32 time;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 started;
} MatchState;

typedef struct {
    /* 0x000 */ u8 pad0[0x20];
    /* 0x020 */ s8 minutes;
    /* 0x021 */ s8 unk26;
    /* 0x022 */ s8 level;
    /* 0x023 */ u8 pad23[0x80 - 0x23];
    /* 0x080 */ SetupSlot slots[4];
    /* 0x180 */ u8 pad180[0x58];
    /* 0x1D8 */ MatchState match;
} GameSetup;

extern GameSetup D_8011AAD8;
extern SetupSlot D_8011AB58[4]; /* D_8011AAD8.slots */
extern u8 D_800F5EC0[];         /* controllers, 0x224 bytes each */
extern u8 D_800F6CB0[];

extern s32 func_002855E8(void *ctl);
extern s32 func_002113A4(void);
extern void func_00430374(void *a, s32 level);

/* func_00417BB0, inlined; here written on the global (the copies in func_00450490/func_0045078C go
 * through a MatchState pointer, which this one does not match with). */
static __inline__ void StartMatch(f32 minutes, s32 a1, s32 started, s32 a3) {
    D_8011AAD8.match.started = started;
    D_8011AAD8.match.unk24 = a3;
    D_8011AAD8.match.unk18 = a1;
    D_8011AAD8.match.unk1C = 0;
    D_8011AAD8.match.unk2C = 0;
    D_8011AAD8.match.unk30 = 0;
    D_8011AAD8.match.unk28 = 0;
    D_8011AAD8.match.unk20 = 0;
    D_8011AAD8.match.time = minutes * 900.0f;
    if (D_8011AAD8.match.time == 0.0f) {
        D_8011AAD8.match.time = -1.0f;
    }
}

s32 func_0045060C(void) {
    GameSetup *setup = &D_8011AAD8;
    s32 level;
    s32 arg;
    s32 i;
    u8 *ctl;

    level = setup->level;
    StartMatch(setup->minutes, setup->unk26, 0, 0);
    for (i = 3; i >= 0; i--) {
        setup->slots[i].unk30 = 0;
    }
    for (i = 0, ctl = D_800F5EC0; i < 4; i++, ctl += 0x224) {
        if (func_002855E8(ctl)) {
            SetupSlot *slot = &D_8011AB58[i];

            slot->active = 1;
            slot->index = i;
            break;
        }
    }
    if (level != 0) {
        if ((u32) (level - 1) < 13) {
            arg = level + 0x2328;
        } else {
            arg = func_002113A4() % 13 + 0x2329;
        }
    } else {
        arg = func_002113A4() % 13 + 0x2329;
    }
    func_00430374(D_800F6CB0, arg);
    return 1;
}
