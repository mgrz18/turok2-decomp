#include "common.h"
#include "structs/game_object.h"

/* The actor definition (header.m_pIntelligence). */
typedef struct {
    /* 0x00 */ s32 kind;
    /* 0x04 */ u8 pad4[0x24 - 0x4];
    /* 0x24 */ s32 unk24;
} ActorDef;

/* The definition value at 0x24, or 0x5334 when it is zero. */
s32 func_00265B2C(CGameObjectInstance *arg0) {
    s32 v = ((ActorDef *) arg0->header.m_pIntelligence)->unk24;
    if (v != 0) {
        return v;
    }
    return 0x5334;
}
