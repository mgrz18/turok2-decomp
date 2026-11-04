/* Second VM module copy of func_0042DC18 (same source, linked 0x0042DB38). */
#include "common.h"
#include "structs/player.h"
#include "structs/game.h"

typedef struct Player {
    /* 0x0000 */ u8 pad0[0x510];
    /* 0x0510 */ s32 active;
    /* 0x0514 */ s32 index;
    /* 0x0518 */ CPlayerOptions *options;
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

extern u8 D_8011AAF1;   /* multiplayer */
extern u8 D_801198B0[];

extern s32 func_0027BC34(void);
extern s32 func_0028D0E0(void);
extern u8 *func_0042FE78(void *);
extern void func_00254C44(void *strokes);
extern void func_00248A8C(Player *p);
extern void func_0042E920(Player *p);
extern void func_00416644(void *, void *callback, Player *p, s32 controller, s32 index);
extern void func_00438800();
extern void func_00438728();
extern void func_0043908C();

void func_00466B38(Player *p, s32 character, s32 arg2) {
    GameSettings *flags;
    s32 i;

    if (func_0027BC34()) {
        arg2 = 0;
    }
    if (func_0028D0E0()) {
        arg2 = 0;
    }
    p->active = 1;
    p->character = character;
    p->camera = func_0042FE78(D_801198B0);
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
        p->options->m_KilledBy[i] = 0;
        p->options->m_Killed[i] = 0;
        p->options->m_TaggedBy[i] = 0;
        p->options->m_Tagged[i] = 0;
    }
    p->options->m_Suicides = 0;
    p->options->m_Deaths = 0;
    p->options->m_Kills = 0;
    p->options->m_Tags = 0;
    p->options->m_bDead = 0;
    p->options->unk_0x3D = 0;
    p->options->m_TagMode = 0;
    p->options->m_LastTagMode = 0;
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
    func_0042E920(p);
    flags = &D_8011AAD8;
    if (flags->multiplayer) {
        p->unk12B4 = 0;
    }
    if (arg2) {
        if (flags->multiplayer) {
            p->options->m_nCharacter = 0;
            p->character = 0;
            p->unkBF8 = 1;
            if (flags->match.teams) {
                func_00416644(p->camera + 0x53C, func_00438800, p, p->controller, p->index);
            } else {
                func_00416644(p->camera + 0x53C, func_00438728, p, p->controller, p->index);
            }
        } else {
            func_00416644(p->camera + 0x53C, func_0043908C, p, p->controller, p->index);
        }
    }
}
