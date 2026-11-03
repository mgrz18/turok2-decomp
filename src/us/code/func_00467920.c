/* Second VM module copy of func_0042EA00 (same source, linked 0x0042E920). */
#include "common.h"

typedef struct GameFlags {
    /* 0x00 */ u8 pad0[0x19];
    /* 0x19 */ u8 f19;
} GameFlags;

typedef struct Player {
    /* 0x0000 */ u8 pad0[0x144];
    /* 0x0144 */ s32 health;
    /* 0x0148 */ u8 pad148[0x524 - 0x148];
    /* 0x0524 */ s32 health2;       /* 8.8 */
    /* 0x0528 */ s16 lives;
    /* 0x052A */ s16 keys;
    /* 0x052C */ u8 pad52C[0xA28 - 0x52C];
    /* 0x0A28 */ f32 unkA28;
    /* 0x0A2C */ u8 padA2C[0xA50 - 0xA2C];
    /* 0x0A50 */ s32 controller;
    /* 0x0A54 */ u8 padA54[0x1298 - 0xA54];
    /* 0x1298 */ s32 unk1298;
    /* 0x129C */ u8 pad129C[0x12A4 - 0x129C];
    /* 0x12A4 */ s32 unk12A4;
    /* 0x12A8 */ s32 unk12A8;
    /* 0x12AC */ s32 unk12AC;
    /* 0x12B0 */ s32 unk12B0;
} Player;

/* D_8011AAF0: the multiplayer flag D_8011AAF1 and the game state D_8011AC88. */
typedef struct GameState {
    /* 0x000 */ u8 pad0;
    /* 0x001 */ u8 multiplayer;     /* D_8011AAF1 */
    /* 0x002 */ u8 pad2[0x198 - 0x2];
    /* 0x198 */ s32 state;          /* D_8011AC88 */
} GameState;

extern GameFlags D_8011AAD8;
extern GameState D_8011AAF0;
extern u8 D_8011AAF1; /* multiplayer */
extern u32 D_800B6D20;
extern s16 D_800AE2B4[];
extern f32 D_800A70D8; /* 60.0f, in the engine's .rodata */

extern void func_00285BD0(s32 controller, s32 state);
extern void func_002540E4(Player *p);
extern void func_00248340(Player *p);
extern void func_0027580C(s32 sound);
extern void func_00248A8C(Player *p);

void func_00467920(Player *p) {
    GameFlags *flags = &D_8011AAD8;

    if (flags->f19 != 0 || (func_00285BD0(p->controller, 1), flags->f19 != 0)) {
        func_002540E4(p);
    } else {
        p->health2 = 0x6400;
        p->health = 0x6400;
    }
    p->unkA28 = D_800A70D8;
    func_00248340(p);
    p->unk1298 = 0;
    p->unk12A8 = 0;
    p->unk12AC = 0;
    p->unk12B0 = 0;
    p->unk12A4 = 0;
    if (D_8011AAF0.state == 13 && D_8011AAF0.multiplayer == 0 && D_800B6D20 >= 11) {
        if (p->keys < 11) {
            func_0027580C(D_800AE2B4[p->keys]);
        }
    }
    if (D_8011AAF1 != 0) {
        func_00248A8C(p);
    }
}
