#include "common.h"
#include "structs/game_object.h"

s32 func_00243414(s32, s32, s32);

/* The actor definition the object points at (header.m_pIntelligence). */
typedef struct {
    /* 0x00 */ s32 kind;
    /* 0x04 */ u8 pad4[0x14 - 0x4];
    /* 0x14 */ u32 flags;
} ActorDef;

void func_0025E658(CGameObjectInstance *arg0, s32 arg1) {
    ActorDef *temp_a2;

    temp_a2 = (ActorDef *) arg0->header.m_pIntelligence;
    if ((temp_a2->kind == 1) && !(temp_a2->flags & 0x800)) {
        func_00243414((s32) arg0, arg1, 2);
    }
}
