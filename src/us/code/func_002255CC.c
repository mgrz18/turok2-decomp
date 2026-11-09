#include "common.h"
#include "structs/game_object.h"
#include "structs/intelligence.h"
#include "structs/world.h"
#include "protos/scene.h"


s32 GameObject_IsSolid(s32);
s32 func_002681BC(s32);

s32 CGameObjectInstance__ClearDrawMatrices(s32);                         /* extern */

/* Files a new actor into every actor list of the world it belongs to. */
void CScene__FileInstance(CScene *arg0, CGameObjectInstance *arg1) {
    s32 temp_a0;
    s32 temp_a0_2;
    s32 temp_a0_3;
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 temp_v1_3;
    s32 temp_v1_4;
    s32 temp_v1_5;

    CGameObjectInstance__ClearDrawMatrices((sptr) arg1);
    if (GameObject_IsSolid((sptr) arg1) != 0) {
        temp_v1 = arg0->m_nDrawActors;
        if (temp_v1 != 0x200) {
            arg0->m_pDrawActors[temp_v1] = (CGameObjectInstance *)arg1;
            arg0->m_nDrawActors = (s32) (temp_v1 + 1);
        }
    }
    if (func_002681BC((sptr) arg1) != 0) {
        temp_v1_2 = arg0->m_nMovers;
        if (temp_v1_2 != 0x80) {
            arg0->m_pMovers[temp_v1_2] = (CGameObjectInstance *)arg1;
            arg0->m_nMovers = (s32) (temp_v1_2 + 1);
        }
    }
    temp_v1_3 = arg0->m_nRegistered;
    if (temp_v1_3 < 0x80) {
        arg0->m_pRegistered[temp_v1_3] = (CGameObjectInstance *)arg1;
        arg0->m_nRegistered = (s32) (temp_v1_3 + 1);
    }
    temp_a0 = arg0->m_nKind1;
    if ((temp_a0 < 0x40) && (arg1->header.m_pIntelligence->m_Type == 1)) {
        arg0->m_pKind1[temp_a0] = (CGameObjectInstance *)arg1;
        temp_v0 = arg0->m_nKind1;
        arg1->m_AI.m_PathFindCount = (s8) temp_v0;
        arg0->m_nKind1 = (s32) (temp_v0 + 1);
    }
    temp_v1_4 = arg0->m_nFlagged;
    if ((temp_v1_4 < 0x20) && (arg1->m_dwFlags & 2)) {
        arg0->m_pFlagged[temp_v1_4] = (CGameObjectInstance *)arg1;
        arg0->m_nFlagged = (s32) (temp_v1_4 + 1);
    }
    temp_a0_2 = arg0->m_nKind5;
    if ((temp_a0_2 < 0x10) && (arg1->header.m_pIntelligence->m_Type == 5)) {
        arg0->m_pKind5[temp_a0_2] = (CGameObjectInstance *)arg1;
        arg0->m_nKind5 = (s32) (temp_a0_2 + 1);
    }
    temp_a0_3 = arg0->m_nList6;
    if (temp_a0_3 < 0x20) {
        if (arg1->m_nTypeFlag == 0x64F) {
            arg0->m_pList6[temp_a0_3] = (CGameObjectInstance *)arg1;
            arg0->m_nList6 = (s32) (temp_a0_3 + 1);
        }
    }
    if (arg1->m_nTypeFlag == 0x64D) {
        temp_v1_5 = arg0->m_nList7;
        if (temp_v1_5 < 4) {
            arg0->m_pList7[temp_v1_5] = (CGameObjectInstance *)arg1;
            arg0->m_nList7 = (s32) (temp_v1_5 + 1);
        }
    }
}
