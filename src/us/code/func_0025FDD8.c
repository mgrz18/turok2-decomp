#include "common.h"
#include "structs/game_object.h"

/* The actor definition (header.m_pIntelligence). */
typedef struct {
    /* 0x00 */ u8 pad0[0x6E];
    /* 0x6E */ s16 unk6E;
} ActorDef;

s16 func_0025FDD8(CGameObjectInstance *arg0) {
    return ((ActorDef *) arg0->header.m_pIntelligence)->unk6E;
}
