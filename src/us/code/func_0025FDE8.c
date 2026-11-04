#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x70];
    /* 0x70 */ s16 unk70;
} ActorDef;

s16 func_0025FDE8(CGameObjectInstance *obj) {
    return ((ActorDef *) obj->header.m_pIntelligence)->unk70;
}
