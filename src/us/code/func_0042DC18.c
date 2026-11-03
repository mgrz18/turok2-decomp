/* Player code: func_0042DC18, 0x0042DC18-0x0042DE74 (splat had it inside the data D_0042D50C). */
#include "common.h"

typedef struct Options {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s16 unk8[4];
    /* 0x10 */ s16 unk10[4];
    /* 0x18 */ s16 unk18[4];
    /* 0x20 */ s16 unk20[4];
    /* 0x28 */ u8 pad28[0x30 - 0x28];
    /* 0x30 */ s8 character;
    /* 0x31 */ u8 pad31[0x3C - 0x31];
    /* 0x3C */ u8 dead;
    /* 0x3D */ u8 unk3D;
    /* 0x3E */ u8 tagMode;
    /* 0x3F */ u8 unk3F;
} Options;

typedef struct Player {
    /* 0x0000 */ u8 pad0[0x510];
    /* 0x0510 */ s32 active;
    /* 0x0514 */ s32 index;
    /* 0x0518 */ Options *options;
    /* 0x051C */ u8 *camera;
    /* 0x0520 */ s32 character;
    /* 0x0524 */ u8 pad524[0x528 - 0x524];
    /* 0x0528 */ s16 lives;
    /* 0x052A */ s16 keys;
    /* 0x052C */ u8 unk52C[0x400];
    /* 0x092C */ u8 pad92C[0x9A0 - 0x92C];
    /* 0x09A0 */ s32 unk9A0[6];
    /* 0x09B8 */ s32 unk9B8[6];
    /* 0x09D0 */ s32 unk9D0[10];
    /* 0x09F8 */ s32 unk9F8;
    /* 0x09FC */ u8 unk9FC[6];
    /* 0x0A02 */ u8 unkA02[6];
    /* 0x0A08 */ u8 padA08[0xA28 - 0xA08];
    /* 0x0A28 */ s32 unkA28;
    /* 0x0A2C */ s32 unkA2C;
    /* 0x0A30 */ u8 padA30[0xA50 - 0xA30];
    /* 0x0A50 */ s32 controller;
    /* 0x0A54 */ u8 padA54[0xBF8 - 0xA54];
    /* 0x0BF8 */ s32 unkBF8;
    /* 0x0BFC */ u8 padBFC[0xC04 - 0xBFC];
    /* 0x0C04 */ s32 unkC04;
    /* 0x0C08 */ s32 unkC08;
    /* 0x0C0C */ u8 padC0C[0xCD0 - 0xC0C];
    /* 0x0CD0 */ u8 strokes[0x12B4 - 0xCD0];
    /* 0x12B4 */ s32 unk12B4;
} Player;

typedef struct GameFlags {
    /* 0x000 */ u8 pad0[0x19];
    /* 0x019 */ u8 multiplayer;     /* D_8011AAF1 */
    /* 0x01A */ u8 pad1A[0x1FC - 0x1A];
    /* 0x1FC */ s32 unk1FC;
} GameFlags;

extern GameFlags D_8011AAD8;
extern u8 D_8011AAF1;   /* multiplayer */
extern u8 D_801198B0[];

extern s32 func_0027BC34(void);
extern s32 func_0028D0E0(void);
extern u8 *func_0042FF58(void *);
extern void func_00254C44(void *strokes);
extern void func_00248A8C(Player *p);
extern void func_0042EA00(Player *p);
extern void func_00416644(void *, void *callback, Player *p, s32 controller, s32 index);
extern void func_004388E0();
extern void func_00438808();
extern void func_0043916C();

void func_0042DC18(Player *p, s32 character, s32 arg2) {
    GameFlags *flags;
    s32 i;

    if (func_0027BC34()) {
        arg2 = 0;
    }
    if (func_0028D0E0()) {
        arg2 = 0;
    }
    p->active = 1;
    p->character = character;
    p->camera = func_0042FF58(D_801198B0);
    func_00254C44(p->strokes);
    for (i = 0; i < 0x400; i++) {
        p->unk52C[i] = 0;
    }
    p->lives = 0;
    for (i = 0; i < 6; i++) {
        p->unk9FC[i] = 0;
    }
    for (i = 0; i < 6; i++) {
        p->unkA02[i] = 0;
    }
    if (!D_8011AAF1) {
        p->keys = 3;
    } else {
        p->keys = 1;
    }
    func_00248A8C(p);
    p->unkA28 = 0;
    p->unkA2C = 0;
    p->unkC04 = -1;
    p->unkC08 = -1;
    for (i = 0; i < 4; i++) {
        p->options->unk18[i] = 0;
        p->options->unk20[i] = 0;
        p->options->unk8[i] = 0;
        p->options->unk10[i] = 0;
    }
    p->options->unk0 = 0;
    p->options->unk2 = 0;
    p->options->unk4 = 0;
    p->options->unk6 = 0;
    p->options->dead = 0;
    p->options->unk3D = 0;
    p->options->tagMode = 0;
    p->options->unk3F = 0;
    for (i = 0; i < 6; i++) {
        p->unk9A0[i] = 0;
        p->unk9B8[i] = 0;
    }
    p->unk9D0[0] = 0;
    p->unk9D0[1] = 0;
    p->unk9D0[2] = 0;
    p->unk9D0[3] = 0;
    p->unk9D0[4] = 0;
    p->unk9D0[5] = 0;
    p->unk9D0[6] = 0;
    p->unk9D0[7] = 0;
    p->unk9D0[8] = 0;
    p->unk9D0[9] = 0;
    p->unk9F8 |= 1;
    func_0042EA00(p);
    flags = &D_8011AAD8;
    if (flags->multiplayer) {
        p->unk12B4 = 0;
    }
    if (arg2) {
        if (flags->multiplayer) {
            p->options->character = 0;
            p->character = 0;
            p->unkBF8 = 1;
            if (flags->unk1FC) {
                func_00416644(p->camera + 0x53C, func_004388E0, p, p->controller, p->index);
            } else {
                func_00416644(p->camera + 0x53C, func_00438808, p, p->controller, p->index);
            }
        } else {
            func_00416644(p->camera + 0x53C, func_0043916C, p, p->controller, p->index);
        }
    }
}
