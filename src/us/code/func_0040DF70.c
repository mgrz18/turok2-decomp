#include "common.h"
#include "structs/game_object.h"
#include "globals/engine.h"

extern s32 gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead; /* current player */

/* Step callback of the Blind One's actors: while alive they target the
 * current player, run their mode's callback and count the hit flash down. */
void BlindOne_ActorUpdate(void *obj, CAI *ai) {
    if (ai->m_Health != 0 && gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead != 0) {
        ai->m_pSightTarget = (struct CGameObjectInstance *) gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead;
        ai->m_pPathTarget = (struct CGameObjectInstance *) gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead;
        ai->m_pTargetOverride = (struct CGameObjectInstance *) gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead;
        ai->m_TargetOverrideTime = 150.0f;
    }
    if (ai->m_pModeCurrent != NULL && ai->m_pModeCurrent->m_pfCallback != NULL) {
        ai->m_pModeCurrent->m_pfCallback(obj, ai);
    }
    ai->m_BlindOne.m_HitTimer = (ai->m_BlindOne.m_HitTimer - gFrameIncrement < 0.0f) ? 0.0f : ai->m_BlindOne.m_HitTimer - gFrameIncrement;
}
