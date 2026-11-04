/* Player code: func_0042E430, 0x0042E430-0x0042E5AC (splat had it inside the data D_0042D50C). */
#include "common.h"
#include "structs/player.h"

typedef struct PlayerDef {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s16 kind;
    /* 0x0E */ u8 padE[0x18 - 0xE];
    /* 0x18 */ s32 health;
    /* 0x1C */ u8 pad1C[0x38 - 0x1C];
    /* 0x38 */ f32 scale[3];
} PlayerDef;

typedef struct Player {
    /* 0x0000 */ u8 pad0[0x14];
    /* 0x0014 */ PlayerDef *def;
    /* 0x0018 */ u8 pad18[0x34 - 0x18];
    /* 0x0034 */ f32 scale[3];
    /* 0x0040 */ u8 pad40[0x144 - 0x40];
    /* 0x0144 */ s32 health;
    /* 0x0148 */ u8 pad148[0x151 - 0x148];
    /* 0x0151 */ s8 unk151;
    /* 0x0152 */ u8 pad152[0x518 - 0x152];
    /* 0x0518 */ CPlayerOptions *options;
    /* 0x051C */ u8 pad51C[0x520 - 0x51C];
    /* 0x0520 */ s32 character;
    /* 0x0524 */ s32 health2;       /* 8.8 */
    /* 0x0528 */ u8 pad528[0xC04 - 0x528];
    /* 0x0C04 */ s32 unkC04;
} Player;

extern u8 D_800F7078[];  /* the world */
extern u8 D_8011AAF1;    /* multiplayer */
extern s32 D_8011ACE4;
extern s32 D_800ADB5C;
extern s32 D_800ADB30[];

extern PlayerDef *func_00225EBC(void *world, s32 kind, s32 character);
extern s32 func_002113A4(void);
extern void func_00248A8C(Player *p);
extern void func_002241D8(void *world, Player *p, s32, s32);

static __inline__ PlayerDef *findDef(s32 kind, s32 character) {
    return func_00225EBC(D_800F7078, kind, character);
}

void func_0042E430(Player *p, s32 character) {
    PlayerDef *def;
    s32 sound;

    p->character = character;
    if (p->options->m_TagMode == 1) {
        character = 11;
    }
    def = findDef(11, character);
    if (def == NULL) {
        def = findDef(11, -1);
        if (def == NULL) {
            def = findDef(-1, -1);
        }
    }
    p->def = def;
    p->scale[0] = def->scale[0];
    p->scale[1] = def->scale[1];
    p->scale[2] = def->scale[2];
    p->health2 = def->health << 8;
    p->health = p->def->health << 8;
    if (p->character == 7) {
        p->unk151 = func_002113A4() % 18;
    } else {
        p->unk151 = -1;
    }
    func_00248A8C(p);
    sound = 0x66;
    if (D_8011AAF1) {
        if (D_8011ACE4 != 0 && p->options->m_TagMode != 0) {
            sound = D_800ADB5C;
        } else {
            sound = D_800ADB30[p->def->kind];
        }
    }
    func_002241D8(D_800F7078, p, sound, p->unkC04);
}
