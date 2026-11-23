#include "common.h"
#include "structs/intelligence.h"
#include "structs/game.h"
#include "structs/game_object.h"
#include "structs/world.h"
#include "globals/game.h"
#include "globals/engine.h"
#include "protos/game.h"



/* The global at gEngineApp_m_Game: what this reads of it. */

void CGameObjectInstance__UpdateAI(CGameObjectInstance *actor);
void CGameObjectInstance__UpdateAnimation(CGameObjectInstance *actor);
void CPlayerPool__Update(CPlayerPool *list);
s32 CPlayerPool__CheckAllReady(CPlayerPool *list);
void CEngineApp__QuitToLegal(u8 *);
void LoopSounds_Update(sptr);

/* Draws one actor, keeping the frame time step. */
static __inline__ void drawActor(CGameObjectInstance *actor) {
    f32 step = gFrameIncrement;

    CGameObjectInstance__UpdateAI(actor);
    if (actor->m_dwFlags & 0x2000) {
        CGameObjectInstance__UpdateAnimation(actor);
    }
    gFrameIncrement = step;
}
/* Draws the leading run of type-2 actors, then the mode extras, then the rest.
 * One actor variable for both loops: its pseudo outlives the first loop, so
 * the first loop keeps the kind-test load ($a0) apart from it ($s0). */
void CScene__AdvanceInstances(CScene *world) {
    CGameObjectInstance *actor;
    s32 i = 0;
    s32 count = world->m_nRegistered;
    CGameObjectInstance **actors = world->m_pRegistered;

    while (i < count) {
        if (actors[i]->header.m_pIntelligence->m_Type != 2) {
            break;
        }
        actor = actors[i++];
        drawActor(actor);
    }
    if (gEngineApp_m_Game.m_Settings.mode == 13) {
        CPlayerPool__Update(&gEngineApp_m_Game.m_PlayerList);
    }
    if (CPlayerPool__CheckAllReady(&gEngineApp_m_Game.m_PlayerList)) {
        CEngineApp__QuitToLegal(&gEngineApp);
    }
    while (i < count) {
        actor = actors[i++];
        drawActor(actor);
    }
    LoopSounds_Update(LoopSounds_Get());
}
