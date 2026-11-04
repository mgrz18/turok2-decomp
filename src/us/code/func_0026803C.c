#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x00 */ s32 type;
    /* 0x04 */ u8 pad4[0x48 - 0x4];
    /* 0x48 */ s32 flags;
} ActorDef;

s32 func_0026803C(CGameObjectInstance *obj) {
    s32 var_v1;
    ActorDef *temp_a1;

    temp_a1 = (ActorDef *) obj->header.m_pIntelligence;
    if (temp_a1->type == 1) {
        var_v1 = 0;
        if ((obj->m_AI.m_Health <= 0) || (temp_a1->flags & 0x10)) {
            var_v1 = 1;
        }
        return var_v1;
    }
    return 0;
}
