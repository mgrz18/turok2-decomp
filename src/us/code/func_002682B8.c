#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x0 */ s32 type;
} CIntelligence;

s32 func_002682B8(CGameObjectInstance *obj) {
    return ((CIntelligence *) obj->header.m_pIntelligence)->type == 1;
}
