#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"


extern s32 gScene_m_bStreamAll;

/* WRONG CUT: the ROM splits this function at 0x245E20 (func_00245E20 = its last 10 words,
   reached by branches from here). Compiled whole it is 0xF8 bytes; the first 0xD0 match
   AI_GetTargetKind and the tail matches func_00245E20 word for word. */

s32 AI_GetTargetKind(CGameObjectInstance *self, CAI *target, CGameObjectInstance *actor) {
    CPlayer *owner;

    if (target->m_Health == 0) {
        return 6;
    }
    if (actor == NULL) {
        if (target->m_PathTrack.m_Mode != 0) {
            return 4;
        }
        return 3;
    }
    if (actor == target->m_pParent) {
        return 2;
    }
    if (actor->header.m_pIntelligence->m_Type == 5) {
        return 5;
    }
    if (actor->m_nTypeFlag == 0x64F) {
        return 7;
    }
    if (gScene_m_bStreamAll == 0) {
        if (actor->m_dwFlags & 0x300000) {
            owner = ((CPlayer *)actor->m_AI.m_pParent);
            if (0.0f < owner->m_FleeTime) {
                if (self->m_nTypeFlag != 0xCA || owner->m_Data.m_CurrentWeapon != 0x14) {
                    return 1;
                }
            }
            if (owner->m_pCrossHairTarget == self && owner->m_CrossHairMode == 2) {
                return 1;
            }
        }
        if ((self->m_dwHotPointFlags & 2) && self->m_nTypeFlag != 0xCA) {
            return 1;
        }
    }
    return 0;
}
