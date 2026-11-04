#include "common.h"
#include "structs/game_object.h"

s32 func_00245A98(s32, s32, s32, s32);

extern s32 D_800AF9C0;

typedef struct {
    /* 0x00 */ u8 pad0[0x98];
    /* 0x98 */ u16 animMask98; /* 5 bits per mode */
} ActorDef;

void func_0025A070(CGameObjectInstance *obj, s32 arg1) {
    func_00245A98((s32) obj, arg1, (s32) &D_800AF9C0, ((ActorDef *) obj->header.m_pIntelligence)->animMask98);
}
