#include "common.h"
#include "structs/game_object.h"

/* The actor definition (header.m_pIntelligence): its flags. */
typedef struct {
    /* 0x00 */ s32 kind;
    /* 0x04 */ u8 pad4[0x14 - 0x4];
    /* 0x14 */ s32 flags;
} ActorDef;

/* The AI fields this touches; 0x10C-0x13B is CAI::m_VarBuffer, the per-mode
 * scratch. */
typedef struct {
    /* 0x000 */ u8 pad0[4];
    /* 0x004 */ s32 m_Health;
    /* 0x008 */ u8 pad8[0x34 - 0x8];
    /* 0x034 */ s8 m_Mode;
    /* 0x035 */ u8 pad35[0x134 - 0x35];
    /* 0x134 */ f32 timer;
    /* 0x138 */ f32 timer2;
} AI;

s32 func_00243414(s32, s32, s32);
extern f32 D_800B6D28;

s32 func_0025C338(CGameObjectInstance *arg0, AI *arg1) {
    f32 temp_f0;
    f32 temp_f0_2;
    s8 temp_v1;

    if (((ActorDef *) arg0->header.m_pIntelligence)->flags & 0x100000) {
        temp_f0 = arg1->timer - D_800B6D28;
        arg1->timer = temp_f0;
        if (temp_f0 < 0.0f) {
            temp_f0_2 = arg1->timer2 - D_800B6D28;
            arg1->timer = 0.0f;
            arg1->timer2 = temp_f0_2;
            if (temp_f0_2 < 0.0f) {
                arg1->timer2 = 0.0f;
            }
        }
        if ((arg1->m_Health != 0) && (arg1->timer <= 0.0f)) {
            if (arg1->timer2 <= 0.0f) {
                temp_v1 = arg1->m_Mode;
                if ((temp_v1 == 4) || (temp_v1 == 0xE) || (temp_v1 == 0xF) || (temp_v1 == 0x10) || (temp_v1 == 0x11) || (temp_v1 == 0x12) || (temp_v1 == 0xA)) {
                    func_00243414((s32) arg0, (s32) arg1, 0x19);
                    return 1;
                }
                return 0;
            }
            goto block_15;
        }
        return 0;
    }
block_15:
    return 0;
}
