#include "common.h"
#include "structs/intelligence.h"
#include "structs/player.h"
#include "structs/camera.h"

s32 AI_IsLineOfSightBlocked(CPlayer *, void *, CGameObjectInstance *);

/* Whether the player's weapon may lock on to a target. */
s32 CPlayer__CanLockOn(CPlayer *player, CGameObjectInstance *t) {
    s32 weapon = player->m_Data.m_CurrentWeapon;
    s32 busy;
    s32 ok;

    if (weapon == 13 || weapon == 29) {
        if (t->m_dwFlags & 0x300000) {
            if (!(t->header.m_pIntelligence->m_dwTypeFlags & 0x20)) {
                return 0;
            }
            if (t->m_AI.m_Mode == 0x23) {
                return 0;
            }
            if (t->m_AI.m_Mode == 0xF) {
                return 0;
            }
            if (t->m_AI.m_Health == 0) {
                return 0;
            }
            busy = 0;
            if (((CPlayer *) t)->m_pCamera != NULL) {
                busy = ((CPlayer *) t)->m_pCamera->m_Menus.count != 0;
            }
            if (!busy) {
                goto visible;
            }
            return 0;
        }
        if (t->header.m_pIntelligence->m_Type == 1) {
            if (!(t->header.m_pIntelligence->m_dwTypeFlags & 0x2400)) {
                return 0;
            }
            if (t->m_AI.m_Mode == 0x21 || t->m_AI.m_Mode == 0x34 || t->m_AI.m_Mode == 0x3C || t->m_AI.m_Mode == 0x3D) {
                return 0;
            }
            if (t->m_AI.m_Health != 0) {
                return AI_IsLineOfSightBlocked(player, &player->m_CharacterActor.m_AI, t) == 0;
            }
            return 0;
        }
    }
    switch (t->header.m_pIntelligence->m_Type) {
    case 1:
        if (t->m_AI.m_Mode == 0x3C) {
            return 0;
        }
        if (weapon == 0x10) {
            goto visible;
        }
        return 1;
    case 4:
        if (t->m_AI.m_Health != 0) {
        visible:
            return AI_IsLineOfSightBlocked(player, &player->m_CharacterActor.m_AI, t) == 0;
        }
        return 0;
    case 7:
        return 0;
    case 11:
        ok = 0;
        if (t->m_AI.m_Mode != 0x23 && t->m_AI.m_Mode != 0xF && t->m_AI.m_Health != 0) {
            busy = 0;
            if (((CPlayer *) t)->m_pCamera != NULL) {
                busy = ((CPlayer *) t)->m_pCamera->m_Menus.count != 0;
            }
            if (!busy) {
                ok = 1;
            }
        }
        return ok;
    }
    return 0;
}
