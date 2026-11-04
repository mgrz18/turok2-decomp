#include "common.h"
#include "structs/game_object.h"

s32 func_00245A98(s32, s32, s32, s32);

extern s32 D_800B0450;

typedef struct {
    /* 0x00 */ u8 pad0[0x7C];
    /* 0x7C */ u16 animMask7C; /* 5 bits per mode */
} ActorDef;

void func_0025AB28(CGameObjectInstance *obj, s32 arg1) {
    func_00245A98((s32) obj, arg1, (s32) &D_800B0450, ((ActorDef *) obj->header.m_pIntelligence)->animMask7C);
}
