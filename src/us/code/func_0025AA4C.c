#include "common.h"
#include "structs/game_object.h"

s32 func_00245A98(s32, s32, s32, s32);

extern s32 D_800B0360;

typedef struct {
    /* 0x00 */ u8 pad0[0x8A];
    /* 0x8A */ u16 animMask8A; /* 5 bits per mode */
} ActorDef;

void func_0025AA4C(CGameObjectInstance *obj, s32 arg1) {
    func_00245A98((s32) obj, arg1, (s32) &D_800B0360, ((ActorDef *) obj->header.m_pIntelligence)->animMask8A);
}
