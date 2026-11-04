#include "common.h"
#include "structs/game_object.h"

s32 func_00245A98(s32, s32, s32, s32);

extern s32 D_800AF750;

typedef struct {
    /* 0x00 */ u8 pad0[0x76];
    /* 0x76 */ u16 animMask76; /* 5 bits per mode */
} ActorDef;

void func_00259EB8(CGameObjectInstance *obj, s32 arg1) {
    func_00245A98((s32) obj, arg1, (s32) &D_800AF750, ((ActorDef *) obj->header.m_pIntelligence)->animMask76);
}
