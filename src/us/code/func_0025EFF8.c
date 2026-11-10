#include "common.h"
#include "structs/game_object.h"

extern s32 EnemyAnim_Idle_AF6F0(void *, sptr, void *);

s32 EnemyAnim_WaitForTrigger(CGameObjectInstance *arg0, sptr arg1) {
    u16 kind;
    CEnemyIntelligence *sub;

    kind = arg0->m_nTypeFlag;
    sub = &arg0->header.m_pIntelligence->m_Enemy;
    if (kind == 0x1F9 || kind == 0x385) {
        switch (sub->m_StartAnim) {
            case 0x2B16:
            case 0x2B20:
            case 0x2B2A:
            case 0x2B34:
            case 0x2B3E:
                break;
            default:
                return EnemyAnim_Idle_AF6F0(arg0, arg1, sub);
        }
    }
    return sub->m_StartAnim;
}
