#include "common.h"
#include "structs/game_object.h"
#include "globals/scene.h"


typedef struct {
    /* 0x00 */ u8 pad00[0x44];
    /* 0x44 */ s16 limit;
} TargetDef;

typedef struct {
    /* 0x0 */ void *target;
    /* 0x4 */ TargetDef *def;
    /* 0x8 */ s32 mode;
} TargetInfo;

extern void *gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead;        /* current player */

/* Point the player actors at a target (mode 0), or back at the player. */
void TotemDefense_RetargetEnemies(TargetInfo *info, s32 force) {
    CListedInstance *actor;
    void *player;
    s32 count;

    if (info->mode == 1) {
        return;
    }
    actor = gScene.m_InstancePool.m_ActiveList.m_pHead;
    player = gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead;
    count = 0;
    for (; actor != NULL; actor = actor->m_pNext) {
        if (actor->m_Instance.header.m_pIntelligence->m_Type != 1 || actor->m_Instance.m_AI.m_pTargetOverride != 0) {
            continue;
        }
        if (info->mode == 0) {
            if (count < info->def->limit || force) {
                count++;
                actor->m_Instance.m_AI.m_pPathTarget = player;
                actor->m_Instance.m_AI.m_pSightTarget = player;
                actor->m_Instance.m_AI.m_dwFlags |= 0x20000000;
                actor->m_Instance.m_AI.m_dwFlags |= 0x40000000;
            } else {
                actor->m_Instance.m_AI.m_pSightTarget = info->target;
                actor->m_Instance.m_AI.m_pPathTarget = info->target;
                actor->m_Instance.m_AI.m_dwFlags |= 0x20000000;
                actor->m_Instance.m_AI.m_dwFlags |= 0x40000000;
            }
        } else if (actor->m_Instance.m_AI.m_pSightTarget == info->target) {
            actor->m_Instance.m_AI.m_pPathTarget = player;
            actor->m_Instance.m_AI.m_pSightTarget = player;
            actor->m_Instance.m_AI.m_dwFlags |= 0x20000000;
            actor->m_Instance.m_AI.m_dwFlags |= 0x40000000;
        }
    }
}
