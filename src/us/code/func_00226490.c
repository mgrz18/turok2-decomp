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
void CGameObjectInstance__UpdateAnimation(DrawActor *actor);
void CPlayerPool__Update(CPlayerList *list);
s32 CPlayerPool__CheckAllReady(CPlayerList *list);
void func_00430330(u8 *);
s32 LoopSounds_Get(void);
void LoopSounds_Update(s32);

/* Draws one actor, keeping the frame time step. */
static __inline__ void drawActor(DrawActor *actor) {
    f32 step = D_800B6D28;

    func_0021F1B4(actor);
    if (actor->flags & 0x2000) {
        CGameObjectInstance__UpdateAnimation(actor);
    }
    D_800B6D28 = step;
}
/* Draws the leading run of type-2 actors, then the mode extras, then the rest.
 * One actor variable for both loops: its pseudo outlives the first loop, so
 * the first loop keeps the kind-test load ($a0) apart from it ($s0). */
void CScene__AdvanceInstances(DrawWorld *world) {
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
        CPlayerPool__Update(&D_80119870.m_PlayerList);
    }
    if (CPlayerPool__CheckAllReady(&D_80119870.m_PlayerList)) {
        func_00430330(&D_800F6CB0);
    }
    while (i < count) {
        actor = actors[i++];
        drawActor(actor);
    }
    LoopSounds_Update(LoopSounds_Get());
}
