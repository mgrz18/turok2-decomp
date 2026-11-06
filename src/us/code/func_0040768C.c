#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 animSpeed;
} Tuning;

extern Tuning D_800C0530;
extern f32 D_8012F9E4;

void Primagen_EnterMode23(CGameObjectInstance *obj) {
    f32 *speed = &D_800C0530.animSpeed;
    f32 *animSpeed = &obj->m_AI.m_AnimSpeed;
    s32 *flags = &obj->m_dwFlags;

    *animSpeed = *speed;
    D_8012F9E4 = *speed;
    *flags = (s32) (*flags | 0x2000);
}
