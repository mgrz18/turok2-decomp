#include "common.h"
#include "structs/intelligence.h"
#include "structs/game.h"
#ifdef TARGET_PORT
#include "structs/game_object.h"
#include "structs/world.h"
#endif
#include "globals/game.h"
#include "globals/engine.h"
#include "protos/game.h"

#ifdef TARGET_PORT
typedef struct DrawActor {
/* the view at the host offsets of CGameObjectInstance (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_DrawActor(m) __builtin_offsetof(CGameObjectInstance, m)
    union {
        struct { u8 _hp_def[T2_HOFF_DrawActor(header.m_pIntelligence)]; CIntelligence *def; }; /* N64 0x14 */
        struct { u8 _hp_flags[T2_HOFF_DrawActor(m_dwFlags)]; s32 flags; }; /* N64 0xD4 */
    };
} DrawActor;
#else
typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ CIntelligence *def;
    /* 0x18 */ u8 pad18[0xD4 - 0x18];
    /* 0xD4 */ s32 flags;
} DrawActor;
#endif

#ifdef TARGET_PORT
typedef struct DrawWorld {
/* the view at the host offsets of CScene (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_DrawWorld(m) __builtin_offsetof(CScene, m)
    union {
        struct { u8 _hp_actors[T2_HOFF_DrawWorld(m_pRegistered)]; DrawActor *actors[0x80]; }; /* N64 0xBF8 */
        struct { u8 _hp_actorCount[T2_HOFF_DrawWorld(m_nRegistered)]; s32 actorCount; }; /* N64 0xDF8 */
    };
} DrawWorld;
#else
typedef struct {
    /* 0x000 */ u8 pad0[0xBF8];
    /* 0xBF8 */ DrawActor *actors[0x80];
    /* 0xDF8 */ s32 actorCount;
} DrawWorld;
#endif

/* The global at gEngineApp_m_Game: what this reads of it. */

void func_0021F1B4(DrawActor *actor);
void CGameObjectInstance__UpdateAnimation(DrawActor *actor);
void CPlayerPool__Update(CPlayerPool *list);
s32 CPlayerPool__CheckAllReady(CPlayerPool *list);
void func_00430330(u8 *);
void LoopSounds_Update(sptr);

/* Draws one actor, keeping the frame time step. */
static __inline__ void drawActor(DrawActor *actor) {
    f32 step = gFrameIncrement;

    func_0021F1B4(actor);
    if (actor->flags & 0x2000) {
        CGameObjectInstance__UpdateAnimation(actor);
    }
    gFrameIncrement = step;
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
    if (gEngineApp_m_Game.m_Settings.mode == 13) {
        CPlayerPool__Update(&gEngineApp_m_Game.m_PlayerList);
    }
    if (CPlayerPool__CheckAllReady(&gEngineApp_m_Game.m_PlayerList)) {
        func_00430330(&gEngineApp);
    }
    while (i < count) {
        actor = actors[i++];
        drawActor(actor);
    }
    LoopSounds_Update(LoopSounds_Get());
}
