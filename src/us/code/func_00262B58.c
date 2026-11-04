#include "common.h"
#include "structs/game_object.h"

s32 func_00210790(s32, s32);                    /* extern */
s32 func_00210B38(s32, f32);                    /* extern */

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

/* Effect callback context: the actor it runs for and the current value. */
typedef struct {
    /* 0x00 */ CGameObjectInstance *owner;
    /* 0x04 */ u8 pad4[0x90 - 0x4];
    /* 0x90 */ s32 value;
} Effect00262B58;

void func_00262B58(s32 arg0, Effect00262B58 *arg1) {
    CGameObjectInstance *temp_a0;
    IntelSub *temp_s1;
    CGameObjectInstance *temp_v0;

    temp_a0 = arg1->owner;
    temp_s1 = &((Intel *)temp_a0->header.m_pIntelligence)->sub;
    if (arg1->value == temp_s1->unk38) {
        func_00210790(arg0, temp_a0->m_AI.m_VarBuffer[0]);
    }
    if (arg1->value == temp_s1->unk3C) {
        temp_v0 = arg1->owner;
        func_00210B38(arg0, temp_v0->m_AI.m_InitialRotY - temp_v0->m_RotY);
    }
}
