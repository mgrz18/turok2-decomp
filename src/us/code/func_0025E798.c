#include "common.h"
#include "structs/game_object.h"

extern void func_00275544(s16 sound, CVector3 pos, s32, s32);

typedef struct {
    /* 0x00 */ u8 pad0[0x42];
    /* 0x42 */ u16 sound;
} ActorDef;

void func_0025E798(CGameObjectInstance *obj) {
    u16 temp_a0;

    temp_a0 = ((ActorDef *) obj->header.m_pIntelligence)->sound;
    if (temp_a0 != 0) {
        func_00275544((s16) temp_a0, obj->header.m_vPos, 0, -1);
    }
}
