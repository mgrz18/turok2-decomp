#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x00 */ s32 type;
    /* 0x04 */ u32 flags;
} CIntelligence;

s32 func_002682A4(CGameObjectInstance *obj) {
    return ((u32) ((CIntelligence *) obj->header.m_pIntelligence)->flags >> 2) & 1;
}
