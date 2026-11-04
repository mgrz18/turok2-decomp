#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x38];
    /* 0x38 */ s32 flags;
} ActorDef;

void func_0026444C(CGameObjectInstance *obj, s32 *arg1) {
    if (!(((ActorDef *) obj->header.m_pIntelligence)->flags & 0x40)) {
        *arg1 &= 0xF7FFFFFF;
    }
}
