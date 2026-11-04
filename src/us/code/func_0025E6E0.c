#include "common.h"
#include "structs/game_object.h"
#include "structs/intelligence.h"


typedef struct {
    /* 0x000 */ u8 pad0[0x130];
    /* 0x130 */ s8 unk130;
    /* 0x131 */ s8 unk131;
} State;

s32 func_00259E60(CGameObjectInstance *actor, State *state);
s32 func_00259E8C(CGameObjectInstance *actor, State *state);
s32 func_00259EB8(CGameObjectInstance *actor, State *state);

s32 func_0025E6E0(CGameObjectInstance *actor, State *state) {
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
        return func_00259E60(actor, state);
    }
    if (state->unk131) {
        return func_00259E8C(actor, state);
    }
    return func_00259EB8(actor, state);
}
