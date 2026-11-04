#include "common.h"
#include "structs/game_object.h"

/* The actor's intelligence block (header.m_pIntelligence): a type word,
 * then a sub-block at 0x14. */
typedef struct {
    /* 0x00 */ s32 flags;
    /* 0x04 */ u8 pad4[0x8 - 0x4];
    /* 0x08 */ s32 unk8;
    /* 0x0C */ u8 padC[0x18 - 0xC];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 pad1C[0x34 - 0x1C];
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ u8 pad44[0x58 - 0x44];
    /* 0x58 */ s16 unk58;
} IntelSub;

typedef struct {
    /* 0x00 */ s32 type;
    /* 0x04 */ u8 pad4[0x14 - 0x4];
    /* 0x14 */ IntelSub sub;
} Intel;

s32 func_00267F64(CGameObjectInstance *arg0) {
    s32 var_v1;
    Intel *temp_a1;

    temp_a1 = (Intel *) arg0->header.m_pIntelligence;
    if (temp_a1->type == 1) {
        var_v1 = 0;
        if ((arg0->m_AI.m_Health <= 0) || (temp_a1->sub.unk34 & 4)) {
            var_v1 = 1;
        }
        return var_v1;
    }
    return 0;
}
