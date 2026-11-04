#include "common.h"
#include "structs/game_object.h"

s32 func_00245A98(s32, s32, s32, s32);

extern s32 D_800AF8D0;

typedef struct {
    /* 0x00 */ u8 pad0[0x94];
    /* 0x94 */ u16 animMask94; /* 5 bits per mode */
} ActorDef;

void func_00259F94(CGameObjectInstance *obj, s32 arg1) {
    func_00245A98((s32) obj, arg1, (s32) &D_800AF8D0, ((ActorDef *) obj->header.m_pIntelligence)->animMask94);
}
