#include "common.h"
#include "structs/game_object.h"

s32 func_00245A98(s32, s32, s32, s32);

extern s32 D_800AFF78;

typedef struct {
    /* 0x00 */ u8 pad0[0x82];
    /* 0x82 */ u16 animMask82; /* 5 bits per mode */
} ActorDef;

void func_0025A6DC(CGameObjectInstance *obj, s32 arg1) {
    func_00245A98((s32) obj, arg1, (s32) &D_800AFF78, ((ActorDef *) obj->header.m_pIntelligence)->animMask82);
}
