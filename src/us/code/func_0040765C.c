#include "common.h"
#include "structs/game_object.h"

/* Boss module state at 0x8012F9DC. */
typedef struct {
    /* 0x0 */ s32 mode;
    /* 0x4 */ s32 unk4;
    /* 0x8 */ f32 timer;
    /* 0xC */ f32 unkC;
} BossState;
extern BossState D_8012F9DC;

void func_0040765C(CGameObjectInstance *obj) {
    obj->m_dwFlags |= 0x2000;
    D_8012F9DC.unkC = 15.0f;
    D_8012F9DC.timer = 11.25f;
}
