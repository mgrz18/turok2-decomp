#include "common.h"
#include "structs/game_object.h"
#include "structs/intelligence.h"


#ifdef TARGET_PORT
/* the port (round 9 x1): the CAI by member: N64 0x130/0x131 are m_Enemy.m_Behavior[0]/[1] (the
 * host's variable buffer is elsewhere and CEnemyAIVars holds a pointer before them) */
typedef CAI State;
#define unk130 m_Enemy.m_Behavior[0]
#define unk131 m_Enemy.m_Behavior[1]
#else
typedef struct {
    /* 0x000 */ u8 pad0[0x130];
    /* 0x130 */ s8 unk130;
    /* 0x131 */ s8 unk131;
} State;
#endif

s32 EnemyAnim_Idle_AF6F0(CGameObjectInstance *actor, State *state);
s32 EnemyAnim_Idle_AF720(CGameObjectInstance *actor, State *state);
s32 EnemyAnim_Idle_AF750(CGameObjectInstance *actor, State *state);

s32 EnemyAnim_PickStart(CGameObjectInstance *actor, State *state) {
    CEnemyIntelligence *ov = &actor->header.m_pIntelligence->m_Enemy;

    if (ov->m_StartAnim != 0) {
        if (actor->m_nTypeFlag == 0x1F9 || actor->m_nTypeFlag == 0x385) {
            switch (ov->m_StartAnim) {
                case 0x2B16:
                case 0x2B20:
                case 0x2B2A:
                case 0x2B34:
                case 0x2B3E:
                    return ov->m_StartAnim + 1;
                default:
                    return ov->m_StartAnim;
            }
        }
        return ov->m_StartAnim;
    }
    if (state->unk130) {
        return EnemyAnim_Idle_AF6F0(actor, state);
    }
    if (state->unk131) {
        return EnemyAnim_Idle_AF720(actor, state);
    }
    return EnemyAnim_Idle_AF750(actor, state);
}
