#include "common.h"
#include "structs/game_object.h"

/* The actor definition (header.m_pIntelligence): its flags. */
typedef struct {
    /* 0x00 */ s32 kind;
    /* 0x04 */ u8 pad4[0x14 - 0x4];
    /* 0x14 */ s32 flags;
} ActorDef;

/* The hit being applied. */
typedef struct {
    /* 0x0 */ u8 pad0[4];
    /* 0x4 */ s32 damage;
    /* 0x8 */ u8 pad8[4];
    /* 0xC */ u32 flags;
} Hit;

s32 func_00243414(s32, s32, s32);

void func_00265EEC(CGameObjectInstance *arg0, CAI *arg1, Hit *arg2) {
    s32 temp_v1;
    s32 var_a2;
    s32 var_a3;
    s32 var_v1;

    temp_v1 = ((ActorDef *) arg0->header.m_pIntelligence)->flags;
    var_a3 = 0;
    if (temp_v1 & 1) {
        var_a3 = (((u32) arg2->flags >> 0x16) ^ 1) & 1;
    }
    if ((temp_v1 & 2) && (arg2->flags & 0x400000)) {
        var_a3 = 1;
    }
    if (var_a3 != 0) {
        var_v1 = arg1->m_Health - arg2->damage;
        if (var_v1 < 0) {
            var_v1 = 0;
        }
        arg1->m_Health = var_v1;
        if (var_v1 == 0) {
            var_a2 = 1;
            if (((ActorDef *) arg0->header.m_pIntelligence)->flags & 0x40) {
                var_a2 = 2;
            }
            func_00243414((s32) arg0, (s32) arg1, var_a2);
        }
    }
}
