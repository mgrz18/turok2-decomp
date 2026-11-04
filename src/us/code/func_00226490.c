#include "common.h"
#include "structs/intelligence.h"
#include "structs/game.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ CIntelligence *def;
    /* 0x18 */ u8 pad18[0xD4 - 0x18];
    /* 0xD4 */ s32 flags;
} DrawActor;

typedef struct {
    /* 0x000 */ u8 pad0[0xBF8];
    /* 0xBF8 */ DrawActor *actors[0x80];
    /* 0xDF8 */ s32 actorCount;
} DrawWorld;


/* The global at D_80119870: what this reads of it. */
extern f32 D_800B6D28;          /* the frame time step */

void func_0021F1B4(DrawActor *actor);
void func_0021A344(DrawActor *actor);
void func_00251FC4(CPlayerList *list);
s32 func_00253460(CPlayerList *list);
void func_00430330(u8 *);
s32 func_00275F7C(void);
void func_00275E98(s32);

/* Draws one actor, keeping the frame time step. */
static __inline__ void drawActor(DrawActor *actor) {
    f32 step = D_800B6D28;

    func_0021F1B4(actor);
    if (actor->flags & 0x2000) {
        func_0021A344(actor);
    }
    D_800B6D28 = step;
}
/* Draws the leading run of type-2 actors, then the mode extras, then the rest.
 * One actor variable for both loops: its pseudo outlives the first loop, so
 * the first loop keeps the kind-test load ($a0) apart from it ($s0). */
void func_00226490(DrawWorld *world) {
    DrawActor *actor;
    s32 i = 0;
    s32 count = world->actorCount;
    DrawActor **actors = world->actors;

    while (i < count) {
        if (actors[i]->def->m_Type != 2) {
            break;
        }
        actor = actors[i++];
        drawActor(actor);
    }
    if (D_80119870.m_Settings.mode == 13) {
        func_00251FC4(&D_80119870.m_PlayerList);
    }
    if (func_00253460(&D_80119870.m_PlayerList)) {
        func_00430330(&D_800F6CB0);
    }
    while (i < count) {
        actor = actors[i++];
        drawActor(actor);
    }
    func_00275E98(func_00275F7C());
}
