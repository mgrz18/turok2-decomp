#include "common.h"
#include "structs/game_object.h"

/* The actor definition (header.m_pIntelligence) and the info block in it. */
typedef struct {
    /* 0x00 */ u32 flags;
    /* 0x04 */ u8 pad4[0x24 - 0x4];
    /* 0x24 */ s32 flags2;
    /* 0x28 */ u8 pad28[0x38 - 0x28];
    /* 0x38 */ f32 rate;
} ActorInfo;

typedef struct {
    /* 0x00 */ s32 kind;
    /* 0x04 */ u8 pad4[0x14 - 0x4];
    /* 0x14 */ ActorInfo info;
} ActorDef;

s32 func_00243414(s32, s32, s32);
extern f32 D_800B6D28;

void func_00264740(CGameObjectInstance *arg0, CAI *arg1) {
    s32 temp_a3;
    s32 var_v1;
    ActorInfo *temp_t0;

    temp_t0 = &((ActorDef *) arg0->header.m_pIntelligence)->info;
    temp_a3 = temp_t0->flags2;
    var_v1 = 1;
    if (temp_a3 & 0x20000) {
        var_v1 = (u32)(arg1->m_dwFlags & 0x20000) < (u32)var_v1;
    }
    if ((temp_a3 & 0x8000) && !(arg1->m_dwFlags & 0x20000)) {
        var_v1 = 0;
    }
    if (var_v1 != 0) {
        arg1->m_ConditionTimer = arg1->m_ConditionTimer - (D_800B6D28 / temp_t0->rate);
    }
    if (arg1->m_ConditionTimer <= 0.0f) {
        func_00243414((s32) arg0, (s32) arg1, 0);
    }
}
