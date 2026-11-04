#include "common.h"
#include "structs/game_object.h"

s32 func_00245A98(s32, s32, s32, s32);

extern s32 D_800B0270;

typedef struct {
    /* 0x00 */ u8 pad0[0x8E];
    /* 0x8E */ u16 animMask8E; /* 5 bits per mode */
} ActorDef;

void func_0025A970(CGameObjectInstance *obj, s32 arg1) {
    func_00245A98((s32) obj, arg1, (s32) &D_800B0270, ((ActorDef *) obj->header.m_pIntelligence)->animMask8E);
}
