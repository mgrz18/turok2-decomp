/* NON-MATCHING: 51 of 87 words differ: the ROM loads each actor into $a0 and copies it to $s0 (and bumps the pointer before the call); here the actor goes straight to $s0 */
#include "common.h"

typedef struct {
    /* 0x00 */ s32 type;
} ActorDef;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ ActorDef *def;
    /* 0x18 */ u8 pad18[0xD4 - 0x18];
    /* 0xD4 */ s32 flags;
} DrawActor;

typedef struct {
    /* 0x000 */ u8 pad0[0xBF8];
    /* 0xBF8 */ DrawActor *actors[0x80];
    /* 0xDF8 */ s32 actorCount;
} DrawWorld;

/* The global at D_80119870: what this reads of it. */
typedef struct {
    /* 0x0000 */ u8 _pad[0x1418];
    /* 0x1418 */ s32 m_Mode;           /* D_8011AC88: game mode */
} Game80119870;

extern Game80119870 D_80119870;
extern f32 D_800B6D28;          /* the frame's time step */
extern u8 D_800F6CB0[];

void func_0021F1B4(DrawActor *actor);
void func_0021A344(DrawActor *actor);
void func_00251FC4(Game80119870 *game);
s32 func_00253460(Game80119870 *game);
void func_00430330(u8 *);
s32 func_00275F7C(void);
void func_00275E98(s32);

/* Draws one actor, keeping the frame's time step. */
static __inline__ void drawActor(DrawActor *actor) {
    f32 step = D_800B6D28;

    func_0021F1B4(actor);
    if (actor->flags & 0x2000) {
        func_0021A344(actor);
    }
    D_800B6D28 = step;
}

/* Draws the type-2 actors at the head of the list, then the mode's extras,
 * then the remaining actors. */
void func_00226490(DrawWorld *world) {
    s32 i = 0;
    s32 count = world->actorCount;
    DrawActor **actors = world->actors;

    for (; i < count; i++) {
        if (actors[i]->def->type != 2) {
            break;
        }
        drawActor(actors[i]);
    }
    if (D_80119870.m_Mode == 13) {
        func_00251FC4(&D_80119870);
    }
    if (func_00253460(&D_80119870)) {
        func_00430330(D_800F6CB0);
    }
    for (; i < count; i++) {
        drawActor(actors[i]);
    }
    func_00275E98(func_00275F7C());
}
