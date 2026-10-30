/* NON-MATCHING: 27 of 95 words differ: the ROM reads the level between the time store and its
 * zero check without reloading the time; this C reloads it and schedules the time store first.
 * Everything from the slot loop on matches. */
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
    /* 0x000 */ u8 pad0[0x25];
    /* 0x025 */ s8 minutes;
    /* 0x026 */ s8 unk26;
    /* 0x027 */ s8 level;
    /* 0x028 */ u8 pad28[0x58];
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
extern void func_00430454(void *a, s32 level);

/* func_00417BB0 without its check of the time, inlined. */
static __inline__ void StartMatch(f32 minutes, s32 a1, s32 started, s32 a3) {
    MatchState *m = &D_8011AAD8.match;

    m->started = started;
    m->unk24 = a3;
    m->unk18 = a1;
    m->unk1C = 0;
    m->unk2C = 0;
    m->unk30 = 0;
    m->unk28 = 0;
    m->unk20 = 0;
    m->time = minutes * 900.0f;
}

s32 func_00417490(void) {
    GameSetup *setup = &D_8011AAD8;
    s32 level;
    s32 arg;
    s32 i;
    u8 *ctl;

    StartMatch(setup->minutes, setup->unk26, 1, 0);
    level = setup->level;
    if (D_8011AAD8.match.time == 0.0f) {
        D_8011AAD8.match.time = -1.0f;
    }
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
        if ((u32) (level - 1) < 6) {
            arg = level + 0x2336;
        } else {
            arg = func_002113A4() % 6 + 0x2337;
        }
    } else {
        arg = func_002113A4() % 6 + 0x2337;
    }
    func_00430454(D_800F6CB0, arg);
    return 1;
}
