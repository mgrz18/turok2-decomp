#include "common.h"
#include "globals/scene.h"
#include "protos/scene.h"
#include "protos/game.h"

extern u8 gEngineApp_m_SwooshSystem[];
void SwooshSet_FreeOwner(void *list, void *actor);

#ifdef TARGET_PORT
#include "structs/game_object.h"
#include "structs/intelligence.h"

/* The port: the intelligence (N64 0x14) and the flags (0xD4) by member of CGameObjectInstance;
 * the definition's flags (0x14) and go animation (0x28) as CActionIntelligence. Reached when a
 * level loads with an action already used (its saved bit set): the warp back into level 51
 * crashed here once the level triggers worked (round 7 merge). */
void Action_GoEnter(u8 *actor_) {
    CGameObjectInstance *actor = (CGameObjectInstance *) actor_;
    CIntelligence *def = actor->header.m_pIntelligence;

    CScene__SetInstanceSavedBit(&gScene, actor, 1);
    GameObject_FireTriggers(actor, 0x80000, (sptr) actor);
    SwooshSet_FreeOwner(gEngineApp_m_SwooshSystem, actor);
    if (gScene.m_Mode != 4) {
        actor->m_dwFlags |= 0x8000000;
        if (def->m_Action.m_dwFlags & 2) {
            actor->m_dwFlags &= ~0x2000;
            actor->m_dwFlags &= ~0x100;
        }
        if (def->m_Action.m_GoAnim == 0) {
            actor->m_dwFlags &= ~0x100;
        }
    }
}
#else
void Action_GoEnter(u8 *actor) {
    s32 *table = &gScene;
    u8 *def = *(u8 **) (actor + 0x14);

    CScene__SetInstanceSavedBit(table, actor, 1);
    GameObject_FireTriggers(actor, 0x80000, actor);
    SwooshSet_FreeOwner(gEngineApp_m_SwooshSystem, actor);
    if (*table != 4) {
        *(s32 *) (actor + 0xD4) |= 0x8000000;
        if (*(s32 *) (def + 0x14) & 2) {
            *(s32 *) (actor + 0xD4) &= ~0x2000;
            *(s32 *) (actor + 0xD4) &= ~0x100;
        }
        if (*(s16 *) (*(u8 **) (actor + 0x14) + 0x28) == 0) {
            *(s32 *) (actor + 0xD4) &= ~0x100;
        }
    }
}
#endif
