#include "common.h"
#include "structs/game_object.h"

/* The actor definition (header.m_pIntelligence). */
typedef struct {
    /* 0x00 */ u8 pad0[0x40];
    /* 0x40 */ s32 unk40;
} ActorDef;

s32 func_00265D04(CGameObjectInstance *arg0) {
    return ((ActorDef *) arg0->header.m_pIntelligence)->unk40;
}
