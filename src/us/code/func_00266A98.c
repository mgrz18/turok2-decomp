#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x0 */ u8 pad0[7];
    /* 0x7 */ u8 value;
} ByteArg;

void func_00266A98(CGameObjectInstance *arg0, s32 arg1, ByteArg *arg2) {
    arg0->header.m_pIntelligence->m_nModel = (u8) arg2->value;
    arg0->header.m_pIntelligence->m_nShadowModel = (u8) arg2->value;
}
