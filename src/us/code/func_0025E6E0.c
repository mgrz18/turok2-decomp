#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ s32 anim;
} AnimOverride;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ AnimOverride override;
} ActorDef;


typedef struct {
    /* 0x000 */ u8 pad0[0x130];
    /* 0x130 */ s8 unk130;
    /* 0x131 */ s8 unk131;
} State;

s32 func_00259E60(CGameObjectInstance *actor, State *state);
s32 func_00259E8C(CGameObjectInstance *actor, State *state);
s32 func_00259EB8(CGameObjectInstance *actor, State *state);

s32 func_0025E6E0(CGameObjectInstance *actor, State *state) {
    AnimOverride *ov = &((ActorDef *)actor->header.m_pIntelligence)->override;

    if (ov->anim != 0) {
        if (actor->m_nTypeFlag == 0x1F9 || actor->m_nTypeFlag == 0x385) {
            switch (ov->anim) {
                case 0x2B16:
                case 0x2B20:
                case 0x2B2A:
                case 0x2B34:
                case 0x2B3E:
                    return ov->anim + 1;
                default:
                    return ov->anim;
            }
        }
        return ov->anim;
    }
    if (state->unk130) {
        return func_00259E60(actor, state);
    }
    if (state->unk131) {
        return func_00259E8C(actor, state);
    }
    return func_00259EB8(actor, state);
}
